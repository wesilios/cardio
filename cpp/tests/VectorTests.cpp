#include "DataStructures/Array/Vector.h"
#include <gtest/gtest.h>

#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void fill(ds::Vector<int> &vector, const std::vector<int> &values) {
    for (int value : values) {
        vector.push_back(value);
    }
}

// Checks size and every element through at(); capacity must always cover size.
::testing::AssertionResult HasContents(const ds::Vector<int> &vector, const std::vector<int> &expected) {
    if (vector.size() != expected.size()) {
        return ::testing::AssertionFailure() << "size " << vector.size() << ", expected " << expected.size();
    }
    if (vector.empty() != expected.empty()) {
        return ::testing::AssertionFailure() << "empty() is " << vector.empty();
    }
    if (vector.capacity() < vector.size()) {
        return ::testing::AssertionFailure() << "capacity " << vector.capacity() << " < size " << vector.size();
    }
    for (std::size_t index = 0; index < expected.size(); ++index) {
        if (vector.at(index) != expected[index]) {
            return ::testing::AssertionFailure()
                   << "at(" << index << ") is " << vector.at(index) << ", expected " << expected[index];
        }
    }
    return ::testing::AssertionSuccess();
}

} // namespace

// ---------- Empty vector ----------

TEST(Vector, NewVectorIsEmpty) {
    ds::Vector<int> vector;
    EXPECT_EQ(vector.size(), 0u);
    EXPECT_TRUE(vector.empty());
    EXPECT_EQ(vector.capacity(), 0u);
}

TEST(Vector, AtOnEmptyThrows) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.at(0), std::out_of_range);
}

TEST(Vector, FrontOnEmptyThrows) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.front(), ds::EmptyContainer);
}

TEST(Vector, BackOnEmptyThrows) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.back(), ds::EmptyContainer);
}

TEST(Vector, PopBackOnEmptyThrows) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.pop_back(), ds::EmptyContainer);
}

TEST(Vector, EmptyVectorStillUsableAfterThrow) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.pop_back(), ds::EmptyContainer);
    vector.push_back(5);
    EXPECT_TRUE(HasContents(vector, {5}));
}

// ---------- push_back and element access ----------

TEST(Vector, PushBackOneElement) {
    ds::Vector<int> vector;
    vector.push_back(7);
    EXPECT_TRUE(HasContents(vector, {7}));
    EXPECT_EQ(vector.front(), 7);
    EXPECT_EQ(vector.back(), 7);
    EXPECT_EQ(vector[0], 7);
}

TEST(Vector, PushBackKeepsOrder) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5});
    EXPECT_TRUE(HasContents(vector, {1, 2, 3, 4, 5}));
    EXPECT_EQ(vector.front(), 1);
    EXPECT_EQ(vector.back(), 5);
}

TEST(Vector, PushBackDuplicates) {
    ds::Vector<int> vector;
    fill(vector, {2, 2, 2});
    EXPECT_TRUE(HasContents(vector, {2, 2, 2}));
}

TEST(Vector, PushBackThousandValues) {
    ds::Vector<int> vector;
    std::vector<int> expected;
    for (int value = 0; value < 1000; ++value) {
        vector.push_back(value);
        expected.push_back(value);
    }
    EXPECT_TRUE(HasContents(vector, expected));
}

TEST(Vector, PushBackStoresACopy) {
    ds::Vector<int> vector;
    int value = 1;
    vector.push_back(value);
    value = 99;
    EXPECT_EQ(vector[0], 1);
}

TEST(Vector, PushBackOwnElementWhileFull) {
    // The argument refers into the vector's own storage. Growing must not free
    // that storage before the value has been copied out of it.
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    ASSERT_EQ(vector.size(), vector.capacity());
    vector.push_back(vector[0]);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3, 4, 1}));
}

TEST(Vector, AtLastIndexDoesNotThrow) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_EQ(vector.at(2), 3);
}

TEST(Vector, AtSizeThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_THROW(vector.at(3), std::out_of_range);
}

TEST(Vector, AtIndexWithinCapacityButPastSizeThrows) {
    ds::Vector<int> vector;
    vector.reserve(10);
    vector.push_back(1);
    EXPECT_THROW(vector.at(5), std::out_of_range);
}

TEST(Vector, AtNegativeIndexThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_THROW(vector.at(static_cast<std::size_t>(-1)), std::out_of_range);
}

TEST(Vector, AtReturnsReferenceForWriting) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.at(1) = 42;
    EXPECT_TRUE(HasContents(vector, {1, 42, 3}));
}

TEST(Vector, BracketReturnsReferenceForWriting) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector[0] = 10;
    vector[2] = 30;
    EXPECT_TRUE(HasContents(vector, {10, 2, 30}));
}

TEST(Vector, FrontAndBackReturnReferencesForWriting) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.front() = 100;
    vector.back() = 300;
    EXPECT_TRUE(HasContents(vector, {100, 2, 300}));
}

TEST(Vector, AccessorsReferToTheSameElements) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_EQ(&vector.at(0), &vector[0]);
    EXPECT_EQ(&vector.front(), &vector[0]);
    EXPECT_EQ(&vector.back(), &vector[2]);
}

// ---------- Const access (read-only through a const vector) ----------

TEST(Vector, NonConstAccessorsReturnWritableReferences) {
    ds::Vector<int> vector;
    EXPECT_TRUE((std::is_same<decltype(vector.at(0)), int &>::value));
    EXPECT_TRUE((std::is_same<decltype(vector[0]), int &>::value));
    EXPECT_TRUE((std::is_same<decltype(vector.front()), int &>::value));
    EXPECT_TRUE((std::is_same<decltype(vector.back()), int &>::value));
}

TEST(Vector, ConstAccessorsReturnReadOnlyReferences) {
    // Code that receives a const Vector can read but not write: assigning
    // through these references would not compile.
    const ds::Vector<int> vector;
    EXPECT_TRUE((std::is_same<decltype(vector.at(0)), const int &>::value));
    EXPECT_TRUE((std::is_same<decltype(vector[0]), const int &>::value));
    EXPECT_TRUE((std::is_same<decltype(vector.front()), const int &>::value));
    EXPECT_TRUE((std::is_same<decltype(vector.back()), const int &>::value));
}

TEST(Vector, ReadThroughConstReference) {
    ds::Vector<int> vector;
    fill(vector, {4, 5, 6});
    const ds::Vector<int> &view = vector;
    EXPECT_EQ(view.size(), 3u);
    EXPECT_EQ(view.at(1), 5);
    EXPECT_EQ(view[2], 6);
    EXPECT_EQ(view.front(), 4);
    EXPECT_EQ(view.back(), 6);
}

TEST(Vector, ConstReferenceSeesWrites) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    const ds::Vector<int> &view = vector;
    vector[1] = 20;
    EXPECT_EQ(view[1], 20);
    EXPECT_EQ(view.at(1), 20);
}

TEST(Vector, ConstAtOutOfRangeThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    const ds::Vector<int> &view = vector;
    EXPECT_THROW(view.at(3), std::out_of_range);
}

TEST(Vector, ConstFrontAndBackOnEmptyThrow) {
    const ds::Vector<int> vector;
    EXPECT_THROW(vector.front(), ds::EmptyContainer);
    EXPECT_THROW(vector.back(), ds::EmptyContainer);
}

TEST(Vector, ConstAccessDoesNotCopyElements) {
    ds::Vector<std::string> vector;
    vector.push_back("one");
    vector.push_back("two");
    const ds::Vector<std::string> &view = vector;
    EXPECT_EQ(&view.at(0), &view[0]);
    EXPECT_EQ(&view.front(), &view[0]);
    EXPECT_EQ(&view.back(), &view[0] + 1);
}

// ---------- Capacity ----------

TEST(Vector, CapacityAlwaysCoversSize) {
    ds::Vector<int> vector;
    for (int value = 0; value < 100; ++value) {
        vector.push_back(value);
        ASSERT_GE(vector.capacity(), vector.size()) << "after pushing " << value;
    }
}

TEST(Vector, PushBackGrowsGeometrically) {
    // Growing by a constant factor keeps push_back amortised O(1). Growing by
    // one slot each time would reallocate on every push: 1000 times here.
    ds::Vector<int> vector;
    std::size_t reallocations = 0;
    std::size_t lastCapacity = vector.capacity();
    for (int value = 0; value < 1000; ++value) {
        vector.push_back(value);
        if (vector.capacity() != lastCapacity) {
            ++reallocations;
            lastCapacity = vector.capacity();
        }
    }
    EXPECT_LE(reallocations, 20u);
}

TEST(Vector, ReserveIncreasesCapacity) {
    ds::Vector<int> vector;
    vector.reserve(10);
    EXPECT_GE(vector.capacity(), 10u);
    EXPECT_EQ(vector.size(), 0u);
    EXPECT_TRUE(vector.empty());
}

TEST(Vector, ReserveKeepsValues) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.reserve(50);
    EXPECT_GE(vector.capacity(), 50u);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, ReserveSmallerIsNoOp) {
    ds::Vector<int> vector;
    vector.reserve(10);
    fill(vector, {1, 2});
    const std::size_t capacity = vector.capacity();
    vector.reserve(1);
    EXPECT_EQ(vector.capacity(), capacity);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

TEST(Vector, PushBackWithinReservedCapacityDoesNotReallocate) {
    ds::Vector<int> vector;
    vector.reserve(8);
    const std::size_t capacity = vector.capacity();
    fill(vector, {1, 2, 3, 4, 5, 6, 7, 8});
    EXPECT_EQ(vector.capacity(), capacity);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3, 4, 5, 6, 7, 8}));
}

TEST(Vector, ShrinkToFitMatchesSize) {
    ds::Vector<int> vector;
    vector.reserve(20);
    fill(vector, {1, 2, 3});
    vector.shrink_to_fit();
    EXPECT_EQ(vector.capacity(), 3u);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, ShrinkToFitAfterPopBack) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5});
    vector.pop_back();
    vector.pop_back();
    vector.shrink_to_fit();
    EXPECT_EQ(vector.capacity(), 3u);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, ShrinkToFitOnEmptyReleasesStorage) {
    ds::Vector<int> vector;
    vector.reserve(10);
    vector.shrink_to_fit();
    EXPECT_EQ(vector.capacity(), 0u);
    vector.push_back(1);
    EXPECT_TRUE(HasContents(vector, {1}));
}

TEST(Vector, ShrinkToFitWhenAlreadyFullIsNoOp) {
    ds::Vector<int> vector;
    vector.reserve(2);
    fill(vector, {1, 2});
    vector.shrink_to_fit();
    EXPECT_EQ(vector.capacity(), 2u);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

// ---------- pop_back ----------

TEST(Vector, PopBackRemovesLast) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.pop_back();
    EXPECT_TRUE(HasContents(vector, {1, 2}));
    EXPECT_EQ(vector.back(), 2);
}

TEST(Vector, PopBackOnlyElement) {
    ds::Vector<int> vector;
    vector.push_back(1);
    vector.pop_back();
    EXPECT_TRUE(HasContents(vector, {}));
}

TEST(Vector, PopBackUntilEmptyThenThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.pop_back();
    vector.pop_back();
    vector.pop_back();
    EXPECT_TRUE(HasContents(vector, {}));
    EXPECT_THROW(vector.pop_back(), ds::EmptyContainer);
}

TEST(Vector, PopBackThenPushBack) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.pop_back();
    vector.push_back(9);
    EXPECT_TRUE(HasContents(vector, {1, 2, 9}));
}

TEST(Vector, PopBackDoesNotGrowCapacity) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    const std::size_t capacity = vector.capacity();
    vector.pop_back();
    EXPECT_LE(vector.capacity(), capacity);
}

// ---------- clear ----------

TEST(Vector, ClearEmptiesVector) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.clear();
    EXPECT_TRUE(HasContents(vector, {}));
    EXPECT_THROW(vector.at(0), std::out_of_range);
}

TEST(Vector, ClearOnEmptyIsNoOp) {
    ds::Vector<int> vector;
    vector.clear();
    EXPECT_TRUE(HasContents(vector, {}));
}

TEST(Vector, ClearTwice) {
    ds::Vector<int> vector;
    fill(vector, {1, 2});
    vector.clear();
    vector.clear();
    EXPECT_TRUE(HasContents(vector, {}));
}

TEST(Vector, PushBackAfterClear) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.clear();
    fill(vector, {4, 5});
    EXPECT_TRUE(HasContents(vector, {4, 5}));
}

TEST(Vector, ClearThenDestroy) {
    // The destructor runs after clear(); ASan reports a double free if clear()
    // left a dangling pointer behind.
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.clear();
}

// ---------- insert(index, value) ----------

TEST(Vector, InsertIntoEmpty) {
    ds::Vector<int> vector;
    int value = 1;
    vector.insert(0, value);
    EXPECT_TRUE(HasContents(vector, {1}));
}

TEST(Vector, InsertAtFront) {
    ds::Vector<int> vector;
    fill(vector, {2, 3});
    int value = 1;
    vector.insert(0, value);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, InsertInMiddle) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 4, 5});
    int value = 3;
    vector.insert(2, value);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3, 4, 5}));
}

TEST(Vector, InsertAtSizeAppends) {
    ds::Vector<int> vector;
    fill(vector, {1, 2});
    int value = 3;
    vector.insert(2, value);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, InsertPastSizeThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2});
    int value = 3;
    EXPECT_THROW(vector.insert(3, value), std::out_of_range);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

TEST(Vector, InsertWhenFullGrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    ASSERT_EQ(vector.size(), vector.capacity());
    int value = 0;
    vector.insert(0, value);
    EXPECT_TRUE(HasContents(vector, {0, 1, 2, 3, 4}));
}

TEST(Vector, InsertManyAtFront) {
    ds::Vector<int> vector;
    for (int value = 0; value < 100; ++value) {
        vector.insert(0, value);
    }
    std::vector<int> expected;
    for (int value = 99; value >= 0; --value) {
        expected.push_back(value);
    }
    EXPECT_TRUE(HasContents(vector, expected));
}

TEST(Vector, InsertOwnElementWithSpareCapacity) {
    // The argument refers into the vector's own storage. Shifting elements
    // right must not overwrite it before it has been copied.
    ds::Vector<int> vector;
    vector.reserve(10);
    fill(vector, {1, 2, 3});
    vector.insert(0, vector[2]);
    EXPECT_TRUE(HasContents(vector, {3, 1, 2, 3}));
}

TEST(Vector, InsertOwnElementWhileFull) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    ASSERT_EQ(vector.size(), vector.capacity());
    vector.insert(1, vector[3]);
    EXPECT_TRUE(HasContents(vector, {1, 4, 2, 3, 4}));
}

// ---------- erase(index) ----------

TEST(Vector, EraseFirst) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(0);
    EXPECT_TRUE(HasContents(vector, {2, 3}));
}

TEST(Vector, EraseMiddle) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    vector.erase(1);
    EXPECT_TRUE(HasContents(vector, {1, 3, 4}));
}

TEST(Vector, EraseLast) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(2);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

TEST(Vector, EraseOnlyElement) {
    ds::Vector<int> vector;
    vector.push_back(1);
    vector.erase(0);
    EXPECT_TRUE(HasContents(vector, {}));
}

TEST(Vector, EraseAtSizeThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_THROW(vector.erase(3), std::out_of_range);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, EraseOnEmptyThrows) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.erase(0), std::out_of_range);
}

// ---------- erase(start, end): removes [start, end) ----------

TEST(Vector, EraseRangeInMiddle) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5});
    vector.erase(1, 3);
    EXPECT_TRUE(HasContents(vector, {1, 4, 5}));
}

TEST(Vector, EraseRangeFromStart) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    vector.erase(0, 2);
    EXPECT_TRUE(HasContents(vector, {3, 4}));
}

TEST(Vector, EraseRangeToEnd) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    vector.erase(2, 4);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

TEST(Vector, EraseRangeEverything) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(0, 3);
    EXPECT_TRUE(HasContents(vector, {}));
}

TEST(Vector, EraseEmptyRangeIsNoOp) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(1, 1);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, EraseEmptyRangeAtLastIndexIsNoOp) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(2, 2);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, EraseEmptyRangeAtSizeIsNoOp) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(3, 3);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, EraseEmptyRangeOnEmptyIsNoOp) {
    ds::Vector<int> vector;
    vector.erase(0, 0);
    EXPECT_TRUE(HasContents(vector, {}));
}

TEST(Vector, EraseRangeShorterThanRemainingTail) {
    // More elements follow the range than the range removes, so the tail
    // has to slide further than the width of the gap.
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5, 6});
    vector.erase(0, 1);
    EXPECT_TRUE(HasContents(vector, {2, 3, 4, 5, 6}));
}

TEST(Vector, EraseOneElementRangeFromMiddleOfLongVector) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5, 6, 7, 8});
    vector.erase(2, 3);
    EXPECT_TRUE(HasContents(vector, {1, 2, 4, 5, 6, 7, 8}));
}

TEST(Vector, EraseRangeMatchesStdVectorForEveryRange) {
    const std::vector<int> values = {10, 20, 30, 40, 50, 60, 70, 80};
    for (std::size_t start = 0; start <= values.size(); ++start) {
        for (std::size_t end = start; end <= values.size(); ++end) {
            ds::Vector<int> vector;
            fill(vector, values);
            std::vector<int> expected = values;

            vector.erase(start, end);
            expected.erase(expected.begin() + start, expected.begin() + end);

            EXPECT_TRUE(HasContents(vector, expected)) << "erase(" << start << ", " << end << ")";
        }
    }
}

TEST(Vector, EraseRangeLastElementOnly) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(2, 3);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

TEST(Vector, EraseRangePastSizeThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_THROW(vector.erase(1, 4), std::out_of_range);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, EraseRangeStartAfterEndThrows) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_THROW(vector.erase(2, 1), std::out_of_range);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

// ---------- swap ----------

TEST(Vector, DISABLED_SwapExchangesContentsAndCapacity) {
    ds::Vector<int> first;
    fill(first, {1, 2, 3});
    ds::Vector<int> second;
    second.reserve(10);
    second.push_back(9);
    const std::size_t firstCapacity = first.capacity();
    const std::size_t secondCapacity = second.capacity();

    first.swap(second);

    EXPECT_TRUE(HasContents(first, {9}));
    EXPECT_TRUE(HasContents(second, {1, 2, 3}));
    EXPECT_EQ(first.capacity(), secondCapacity);
    EXPECT_EQ(second.capacity(), firstCapacity);
}

TEST(Vector, DISABLED_SwapWithEmpty) {
    ds::Vector<int> first;
    fill(first, {1, 2});
    ds::Vector<int> second;
    first.swap(second);
    EXPECT_TRUE(HasContents(first, {}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
}

TEST(Vector, DISABLED_SwapWithSelf) {
    ds::Vector<int> vector;
    fill(vector, {1, 2});
    vector.swap(vector);
    EXPECT_TRUE(HasContents(vector, {1, 2}));
}

// ---------- Other element types ----------

TEST(Vector, StoresStrings) {
    ds::Vector<std::string> vector;
    vector.push_back("one");
    vector.push_back("two");
    vector.push_back("three");
    EXPECT_EQ(vector.size(), 3u);
    EXPECT_EQ(vector.front(), "one");
    EXPECT_EQ(vector.at(1), "two");
    EXPECT_EQ(vector.back(), "three");
}

TEST(Vector, StringsSurviveManyReallocations) {
    ds::Vector<std::string> vector;
    for (int value = 0; value < 200; ++value) {
        vector.push_back("value-" + std::to_string(value));
    }
    for (int value = 0; value < 200; ++value) {
        EXPECT_EQ(vector[value], "value-" + std::to_string(value));
    }
}

TEST(Vector, DISABLED_VectorOfVectors) {
    // Growing the outer vector copies the inner ones, which only works if
    // copying a Vector is a deep copy.
    ds::Vector<ds::Vector<int>> outer;
    for (int row = 0; row < 10; ++row) {
        ds::Vector<int> inner;
        for (int column = 0; column <= row; ++column) {
            inner.push_back(row * 10 + column);
        }
        outer.push_back(inner);
    }
    ASSERT_EQ(outer.size(), 10u);
    for (std::size_t row = 0; row < outer.size(); ++row) {
        ASSERT_EQ(outer[row].size(), row + 1);
        EXPECT_EQ(outer[row].back(), static_cast<int>(row * 10 + row));
    }
}

// ---------- Copy (rule of three) ----------

TEST(Vector, DISABLED_CopyConstructorCopiesValues) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> copy(source);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(Vector, DISABLED_CopyConstructorOfEmpty) {
    ds::Vector<int> source;
    ds::Vector<int> copy(source);
    EXPECT_TRUE(HasContents(copy, {}));
    copy.push_back(1);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(Vector, DISABLED_CopyConstructorIsDeep) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> copy(source);
    copy[0] = 100;
    copy.push_back(4);
    source[2] = 300;
    EXPECT_TRUE(HasContents(source, {1, 2, 300}));
    EXPECT_TRUE(HasContents(copy, {100, 2, 3, 4}));
}

TEST(Vector, DISABLED_CopyAssignmentCopiesValues) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(Vector, DISABLED_CopyAssignmentReplacesExistingValues) {
    ds::Vector<int> source;
    fill(source, {1, 2});
    ds::Vector<int> target;
    fill(target, {7, 8, 9, 10, 11});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(Vector, DISABLED_CopyAssignmentIsDeep) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> target;
    target = source;
    target[1] = 200;
    source.push_back(4);
    EXPECT_TRUE(HasContents(source, {1, 2, 3, 4}));
    EXPECT_TRUE(HasContents(target, {1, 200, 3}));
}

TEST(Vector, DISABLED_CopyAssignmentFromEmpty) {
    ds::Vector<int> source;
    ds::Vector<int> target;
    fill(target, {1, 2});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(Vector, DISABLED_SelfCopyAssignmentKeepsValues) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    ds::Vector<int> &alias = vector;
    vector = alias;
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, DISABLED_ChainedCopyAssignment) {
    ds::Vector<int> source;
    fill(source, {1, 2});
    ds::Vector<int> first;
    ds::Vector<int> second;
    first = second = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
}

// ---------- Move (rule of five) ----------

TEST(Vector, DISABLED_MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::Vector<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::Vector<int>>::value);
}

TEST(Vector, DISABLED_MoveConstructorTransfersValues) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(Vector, DISABLED_MoveConstructorLeavesSourceEmpty) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
    EXPECT_EQ(source.capacity(), 0u);
}

TEST(Vector, DISABLED_MoveConstructorStealsStorage) {
    // A move takes over the buffer instead of copying it.
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    const int *buffer = &source[0];
    ds::Vector<int> moved(std::move(source));
    EXPECT_EQ(&moved[0], buffer);
}

TEST(Vector, DISABLED_MovedFromSourceIsReusable) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> moved(std::move(source));
    fill(source, {4, 5});
    EXPECT_TRUE(HasContents(source, {4, 5}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(Vector, DISABLED_MoveAssignmentTransfersValues) {
    ds::Vector<int> source;
    fill(source, {1, 2, 3});
    ds::Vector<int> target;
    fill(target, {9, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(Vector, DISABLED_MoveAssignmentIntoEmpty) {
    ds::Vector<int> source;
    fill(source, {1, 2});
    ds::Vector<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(Vector, DISABLED_SelfMoveAssignmentKeepsValues) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    ds::Vector<int> &alias = vector;
    vector = std::move(alias);
    EXPECT_TRUE(HasContents(vector, {1, 2, 3}));
}

TEST(Vector, DISABLED_MoveIntoStdVector) {
    // std::vector only moves its elements when it grows if their move
    // constructor is noexcept; otherwise it copies them.
    std::vector<ds::Vector<int>> vectors;
    for (int value = 0; value < 20; ++value) {
        ds::Vector<int> inner;
        inner.push_back(value);
        vectors.push_back(std::move(inner));
    }
    for (int value = 0; value < 20; ++value) {
        EXPECT_TRUE(HasContents(vectors[value], {value}));
    }
}
