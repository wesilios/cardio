#include "DataStructures/Array/Vector.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace {

void fill(ds::Vector<int> &vector, const std::vector<int> &values) {
    for (int value : values) {
        vector.push_back(value);
    }
}

// Collects the values in iteration order using a range-for.
std::vector<int> visited(const ds::Vector<int> &vector) {
    std::vector<int> values;
    for (const auto &value : vector) {
        values.push_back(value);
    }
    return values;
}

} // namespace

// ---------- begin / end ----------

TEST(VectorIteration, NewVectorBeginEqualsEnd) {
    ds::Vector<int> vector;
    EXPECT_TRUE(vector.begin() == vector.end());
}

TEST(VectorIteration, RangeForOverEmptyVisitsNothing) {
    ds::Vector<int> vector;
    EXPECT_TRUE(visited(vector).empty());
}

TEST(VectorIteration, RangeForOverSingleElement) {
    ds::Vector<int> vector;
    vector.push_back(7);
    EXPECT_EQ(visited(vector), std::vector<int>({7}));
}

TEST(VectorIteration, RangeForVisitsInOrder) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5});
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(VectorIteration, ManualLoopVisitsInOrder) {
    ds::Vector<int> vector;
    fill(vector, {3, 1, 2});
    std::vector<int> values;
    for (auto it = vector.begin(); it != vector.end(); ++it) {
        values.push_back(*it);
    }
    EXPECT_EQ(values, std::vector<int>({3, 1, 2}));
}

TEST(VectorIteration, EndMinusBeginIsSize) {
    ds::Vector<int> vector;
    EXPECT_EQ(vector.end() - vector.begin(), 0);
    fill(vector, {1, 2, 3, 4});
    EXPECT_EQ(static_cast<std::size_t>(vector.end() - vector.begin()), vector.size());
}

TEST(VectorIteration, BeginRefersToFront) {
    ds::Vector<int> vector;
    fill(vector, {4, 5, 6});
    EXPECT_EQ(*vector.begin(), 4);
    EXPECT_EQ(&*vector.begin(), &vector.front());
}

TEST(VectorIteration, EndIsOnePastBack) {
    ds::Vector<int> vector;
    fill(vector, {4, 5, 6});
    EXPECT_EQ(*(vector.end() - 1), 6);
    EXPECT_EQ(&*(vector.end() - 1), &vector.back());
}

TEST(VectorIteration, IteratesOverThousandValues) {
    ds::Vector<int> vector;
    std::vector<int> expected;
    for (int value = 0; value < 1000; ++value) {
        vector.push_back(value);
        expected.push_back(value);
    }
    EXPECT_EQ(visited(vector), expected);
}

// ---------- Writing through iterators ----------

TEST(VectorIteration, NonConstIteratorGivesWritableElements) {
    ds::Vector<int> vector;
    EXPECT_TRUE((std::is_same<decltype(*vector.begin()), int &>::value));
}

TEST(VectorIteration, WriteThroughIterator) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    *vector.begin() = 10;
    EXPECT_EQ(vector[0], 10);
}

TEST(VectorIteration, RangeForByReferenceModifiesElements) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    for (auto &value : vector) {
        value *= 2;
    }
    EXPECT_EQ(visited(vector), std::vector<int>({2, 4, 6}));
}

TEST(VectorIteration, RangeForByValueLeavesElementsUnchanged) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    for (auto value : vector) {
        value *= 2;
        (void)value;
    }
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2, 3}));
}

// ---------- Const iteration ----------

TEST(VectorIteration, ConstIteratorGivesReadOnlyElements) {
    // Iterating a const Vector must not allow writes: assigning through
    // *begin() would not compile.
    const ds::Vector<int> vector;
    EXPECT_TRUE((std::is_same<decltype(*vector.begin()), const int &>::value));
    EXPECT_TRUE((std::is_same<decltype(*vector.end()), const int &>::value));
}

TEST(VectorIteration, RangeForOverConstVector) {
    ds::Vector<int> vector;
    fill(vector, {7, 8, 9});
    const ds::Vector<int> &view = vector;
    std::vector<int> values;
    for (const auto &value : view) {
        values.push_back(value);
    }
    EXPECT_EQ(values, std::vector<int>({7, 8, 9}));
}

TEST(VectorIteration, ConstAndNonConstBeginReferToSameElement) {
    ds::Vector<int> vector;
    fill(vector, {1, 2});
    const ds::Vector<int> &view = vector;
    EXPECT_EQ(&*view.begin(), &*vector.begin());
}

// ---------- Iteration after modifiers ----------

TEST(VectorIteration, SeesPushBackAfterGrowth) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    vector.push_back(5);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(VectorIteration, SeesInsert) {
    ds::Vector<int> vector;
    fill(vector, {1, 3});
    int value = 2;
    vector.insert(1, value);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2, 3}));
}

TEST(VectorIteration, SeesEraseByIndex) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(1);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 3}));
}

TEST(VectorIteration, SeesEraseRange) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5});
    vector.erase(1, 4);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 5}));
}

TEST(VectorIteration, StopsBeforePoppedElement) {
    // pop_back leaves the old value in the buffer; end() must exclude it.
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.pop_back();
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2}));
}

TEST(VectorIteration, ReservedCapacityIsNotIterated) {
    ds::Vector<int> vector;
    vector.reserve(10);
    fill(vector, {1, 2});
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2}));
}

TEST(VectorIteration, EmptyAfterClear) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.clear();
    EXPECT_TRUE(vector.begin() == vector.end());
    EXPECT_TRUE(visited(vector).empty());
}

TEST(VectorIteration, EmptyAfterShrinkToFitOnEmpty) {
    ds::Vector<int> vector;
    vector.reserve(5);
    vector.shrink_to_fit();
    EXPECT_TRUE(vector.begin() == vector.end());
}

// ---------- Standard algorithms ----------

TEST(VectorIteration, StdSortMatchesStdVector) {
    const std::vector<int> values = {5, -3, 9, 0, 2, 2, 7, -8, 1, 4};
    ds::Vector<int> vector;
    fill(vector, values);
    std::vector<int> expected = values;

    std::sort(vector.begin(), vector.end());
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(visited(vector), expected);
}

TEST(VectorIteration, StdFindLocatesElement) {
    ds::Vector<int> vector;
    fill(vector, {4, 8, 15, 16, 23, 42});
    auto found = std::find(vector.begin(), vector.end(), 16);
    ASSERT_TRUE(found != vector.end());
    EXPECT_EQ(found - vector.begin(), 3);
}

TEST(VectorIteration, StdFindMissingReturnsEnd) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_TRUE(std::find(vector.begin(), vector.end(), 99) == vector.end());
}

TEST(VectorIteration, StdAccumulateSumsElements) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    EXPECT_EQ(std::accumulate(vector.begin(), vector.end(), 0), 10);
}

TEST(VectorIteration, StdVectorCanBeBuiltFromRange) {
    ds::Vector<int> vector;
    fill(vector, {3, 1, 4, 1, 5});
    std::vector<int> copy(vector.begin(), vector.end());
    EXPECT_EQ(copy, std::vector<int>({3, 1, 4, 1, 5}));
}

// ---------- Other element types ----------

TEST(VectorIteration, IteratesOverStrings) {
    ds::Vector<std::string> vector;
    vector.push_back("one");
    vector.push_back("two");
    std::string joined;
    for (const auto &value : vector) {
        joined += value + ";";
    }
    EXPECT_EQ(joined, "one;two;");
}

TEST(VectorIteration, ModifiesStringsThroughReference) {
    ds::Vector<std::string> vector;
    vector.push_back("a");
    vector.push_back("b");
    for (auto &value : vector) {
        value += "!";
    }
    EXPECT_EQ(vector[0], "a!");
    EXPECT_EQ(vector[1], "b!");
}

// ---------- cbegin / cend and iterator conversion ----------

TEST(VectorIteration, CbeginGivesReadOnlyElements) {
    ds::Vector<int> vector;
    EXPECT_TRUE((std::is_same<decltype(*vector.cbegin()), const int &>::value));
    EXPECT_TRUE((std::is_same<decltype(*vector.cend()), const int &>::value));
}

TEST(VectorIteration, CbeginToCendVisitsInOrder) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    std::vector<int> values;
    for (auto it = vector.cbegin(); it != vector.cend(); ++it) {
        values.push_back(*it);
    }
    EXPECT_EQ(values, std::vector<int>({1, 2, 3}));
}

TEST(VectorIteration, CbeginEqualsCendWhenEmpty) {
    ds::Vector<int> vector;
    EXPECT_TRUE(vector.cbegin() == vector.cend());
}

TEST(VectorIteration, IteratorConvertsToConstIteratorButNotBack) {
    // A writable position may be used where a read-only one is expected, but a
    // read-only position must never become writable.
    using Iterator = ds::Vector<int>::iterator;
    using ConstIterator = ds::Vector<int>::const_iterator;
    EXPECT_TRUE((std::is_convertible<Iterator, ConstIterator>::value));
    EXPECT_FALSE((std::is_convertible<ConstIterator, Iterator>::value));
}

TEST(VectorIteration, ConvertedIteratorRefersToSameElement) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    ds::Vector<int>::const_iterator position = vector.begin() + 1;
    EXPECT_EQ(*position, 2);
    EXPECT_EQ(&*position, &vector[1]);
}

// ---------- erase(const_iterator) ----------

TEST(VectorIteration, EraseIteratorReturnsWritableIterator) {
    ds::Vector<int> vector;
    EXPECT_TRUE((std::is_same<decltype(vector.erase(vector.cbegin())), ds::Vector<int>::iterator>::value));
}

TEST(VectorIteration, EraseFirstThroughIterator) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    auto next = vector.erase(vector.begin());
    EXPECT_EQ(visited(vector), std::vector<int>({2, 3}));
    ASSERT_TRUE(next == vector.begin());
    EXPECT_EQ(*next, 2);
}

TEST(VectorIteration, EraseMiddleThroughIterator) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    auto next = vector.erase(vector.begin() + 1);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 3, 4}));
    EXPECT_EQ(next - vector.begin(), 1);
    EXPECT_EQ(*next, 3);
}

TEST(VectorIteration, EraseLastThroughIteratorReturnsEnd) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    auto next = vector.erase(vector.end() - 1);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2}));
    EXPECT_TRUE(next == vector.end());
}

TEST(VectorIteration, EraseOnlyElementThroughIterator) {
    ds::Vector<int> vector;
    vector.push_back(1);
    auto next = vector.erase(vector.begin());
    EXPECT_TRUE(visited(vector).empty());
    EXPECT_EQ(vector.size(), 0u);
    EXPECT_TRUE(next == vector.end());
}

TEST(VectorIteration, EraseThroughConstIterator) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    auto next = vector.erase(vector.cbegin() + 1);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 3}));
    EXPECT_EQ(*next, 3);
}

TEST(VectorIteration, EraseAtEndThrows) {
    // end() is one past the last element; there is nothing there to erase.
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    EXPECT_THROW(vector.erase(vector.cend()), std::out_of_range);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2, 3}));
}

TEST(VectorIteration, EraseAtEndOfEmptyThrows) {
    ds::Vector<int> vector;
    EXPECT_THROW(vector.erase(vector.cend()), std::out_of_range);
}

TEST(VectorIteration, ReturnedIteratorCanBeWrittenThrough) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    auto next = vector.erase(vector.cbegin());
    *next = 20;
    EXPECT_EQ(visited(vector), std::vector<int>({20, 3}));
}

TEST(VectorIteration, EraseRepeatedlyFromFrontUntilEmpty) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4});
    auto it = vector.begin();
    while (it != vector.end()) {
        it = vector.erase(it);
    }
    EXPECT_EQ(vector.size(), 0u);
    EXPECT_TRUE(vector.begin() == vector.end());
}

TEST(VectorIteration, EraseWhileIteratingRemovesEvens) {
    // The classic pattern: erase invalidates the iterator, so continue from the
    // iterator it returns instead of incrementing the old one.
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3, 4, 5, 6, 8, 9});
    for (auto it = vector.begin(); it != vector.end();) {
        if (*it % 2 == 0) {
            it = vector.erase(it);
        } else {
            ++it;
        }
    }
    EXPECT_EQ(visited(vector), std::vector<int>({1, 3, 5, 9}));
}

TEST(VectorIteration, EraseThenPushBackReusesSlot) {
    ds::Vector<int> vector;
    fill(vector, {1, 2, 3});
    vector.erase(vector.end() - 1);
    vector.push_back(4);
    EXPECT_EQ(visited(vector), std::vector<int>({1, 2, 4}));
}

TEST(VectorIteration, EraseIteratorMatchesStdVectorAtEveryPosition) {
    const std::vector<int> values = {10, 20, 30, 40, 50, 60};
    for (std::size_t index = 0; index < values.size(); ++index) {
        ds::Vector<int> vector;
        fill(vector, values);
        std::vector<int> expected = values;

        auto next = vector.erase(vector.cbegin() + index);
        auto expectedNext = expected.erase(expected.cbegin() + index);

        EXPECT_EQ(visited(vector), expected) << "erase at " << index;
        EXPECT_EQ(next - vector.begin(), expectedNext - expected.begin()) << "erase at " << index;
    }
}

TEST(VectorIteration, EraseStringThroughIterator) {
    // Strings own heap memory: destroying an element that the buffer will
    // destroy again (on delete[]) is a double free.
    ds::Vector<std::string> vector;
    vector.push_back(std::string(100, 'a'));
    vector.push_back(std::string(100, 'b'));
    vector.push_back(std::string(100, 'c'));
    auto next = vector.erase(vector.cbegin());
    ASSERT_EQ(vector.size(), 2u);
    EXPECT_EQ(*next, std::string(100, 'b'));
    EXPECT_EQ(vector[1], std::string(100, 'c'));
}

TEST(VectorIteration, EraseStringThenPushBack) {
    ds::Vector<std::string> vector;
    vector.push_back("one");
    vector.push_back("two");
    vector.erase(vector.cend() - 1);
    vector.push_back("three");
    EXPECT_EQ(vector[0], "one");
    EXPECT_EQ(vector[1], "three");
}
