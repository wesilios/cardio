#include "DataStructures/Array/Array.h"
#include <gtest/gtest.h>

#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>

// ---------- Size ----------

TEST(Array, SizeIsN) {
    ds::Array<int, 5> array{};
    EXPECT_EQ(array.size(), 5u);
}

TEST(Array, NonZeroSizeIsNotEmpty) {
    ds::Array<int, 1> array{};
    EXPECT_FALSE(array.empty());
}

TEST(Array, ZeroSizeIsEmpty) {
    ds::Array<int, 0> array{};
    EXPECT_EQ(array.size(), 0u);
    EXPECT_TRUE(array.empty());
}

TEST(Array, SizeIsKnownAtCompileTime) {
    static_assert(ds::Array<int, 3>{}.size() == 3, "size() should be constexpr");
    static_assert(!ds::Array<int, 3>{}.empty(), "empty() should be constexpr");
}

TEST(Array, HasNoOverheadOverRawArray) {
    // The wrapper should store exactly N elements and nothing else.
    EXPECT_EQ(sizeof(ds::Array<int, 5>), sizeof(int[5]));
    EXPECT_EQ(sizeof(ds::Array<double, 3>), sizeof(double[3]));
}

// ---------- Initialisation ----------

TEST(Array, AggregateInitialisationSetsValues) {
    ds::Array<int, 3> array{1, 2, 3};
    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 3);
}

TEST(Array, PartialInitialisationZeroFillsTheRest) {
    ds::Array<int, 4> array{7, 8};
    EXPECT_EQ(array[0], 7);
    EXPECT_EQ(array[1], 8);
    EXPECT_EQ(array[2], 0);
    EXPECT_EQ(array[3], 0);
}

TEST(Array, EmptyBracesZeroFill) {
    ds::Array<int, 3> array{};
    for (std::size_t index = 0; index < array.size(); ++index) {
        EXPECT_EQ(array[index], 0) << "index " << index;
    }
}

// ---------- Element access: at ----------

TEST(Array, AtReadsEveryIndex) {
    ds::Array<int, 4> array{10, 20, 30, 40};
    EXPECT_EQ(array.at(0), 10);
    EXPECT_EQ(array.at(1), 20);
    EXPECT_EQ(array.at(2), 30);
    EXPECT_EQ(array.at(3), 40);
}

TEST(Array, AtReturnsReferenceForWriting) {
    ds::Array<int, 3> array{1, 2, 3};
    array.at(1) = 42;
    EXPECT_EQ(array[1], 42);
}

TEST(Array, AtLastIndexDoesNotThrow) {
    ds::Array<int, 3> array{1, 2, 3};
    EXPECT_NO_THROW(array.at(2));
}

TEST(Array, AtSizeThrows) {
    ds::Array<int, 3> array{1, 2, 3};
    EXPECT_THROW(array.at(3), std::out_of_range);
}

TEST(Array, AtFarOutOfRangeThrows) {
    ds::Array<int, 3> array{1, 2, 3};
    EXPECT_THROW(array.at(1000), std::out_of_range);
}

TEST(Array, AtNegativeIndexThrows) {
    // -1 converts to the largest std::size_t, so it must be caught as out of range.
    ds::Array<int, 3> array{1, 2, 3};
    EXPECT_THROW(array.at(static_cast<std::size_t>(-1)), std::out_of_range);
}

TEST(Array, AtOnZeroSizeThrows) {
    ds::Array<int, 0> array{};
    EXPECT_THROW(array.at(0), std::out_of_range);
}

TEST(Array, FailedAtLeavesValuesUnchanged) {
    ds::Array<int, 2> array{1, 2};
    EXPECT_THROW(array.at(2), std::out_of_range);
    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 2);
}

// ---------- Element access: operator[] ----------

TEST(Array, BracketReadsEveryIndex) {
    ds::Array<int, 3> array{4, 5, 6};
    EXPECT_EQ(array[0], 4);
    EXPECT_EQ(array[1], 5);
    EXPECT_EQ(array[2], 6);
}

TEST(Array, BracketReturnsReferenceForWriting) {
    ds::Array<int, 3> array{};
    array[0] = 9;
    array[2] = 7;
    EXPECT_EQ(array[0], 9);
    EXPECT_EQ(array[1], 0);
    EXPECT_EQ(array[2], 7);
}

TEST(Array, FillThroughLoop) {
    ds::Array<int, 100> array{};
    for (std::size_t index = 0; index < array.size(); ++index) {
        array[index] = static_cast<int>(index * index);
    }
    for (std::size_t index = 0; index < array.size(); ++index) {
        EXPECT_EQ(array.at(index), static_cast<int>(index * index));
    }
}

TEST(Array, AtAndBracketReferToTheSameElement) {
    ds::Array<int, 3> array{1, 2, 3};
    EXPECT_EQ(&array.at(1), &array[1]);
}

// ---------- Const access (read-only through a const array) ----------

TEST(Array, NonConstAccessorsReturnWritableReferences) {
    ds::Array<int, 3> array{};
    EXPECT_TRUE((std::is_same<decltype(array.at(0)), int &>::value));
    EXPECT_TRUE((std::is_same<decltype(array[0]), int &>::value));
}

TEST(Array, ConstAccessorsReturnReadOnlyReferences) {
    // Code that receives a const Array can read but not write: assigning
    // through these references would not compile.
    const ds::Array<int, 3> array{};
    EXPECT_TRUE((std::is_same<decltype(array.at(0)), const int &>::value));
    EXPECT_TRUE((std::is_same<decltype(array[0]), const int &>::value));
}

TEST(Array, ReadThroughConstArray) {
    const ds::Array<int, 3> array{4, 5, 6};
    EXPECT_EQ(array.at(0), 4);
    EXPECT_EQ(array[1], 5);
    EXPECT_EQ(array.at(2), 6);
}

TEST(Array, ReadThroughConstReferenceSeesWrites) {
    ds::Array<int, 3> array{1, 2, 3};
    const ds::Array<int, 3> &view = array;
    array[1] = 20;
    EXPECT_EQ(view[1], 20);
    EXPECT_EQ(view.at(1), 20);
}

TEST(Array, ConstAtSizeThrows) {
    const ds::Array<int, 3> array{1, 2, 3};
    EXPECT_THROW(array.at(3), std::out_of_range);
}

TEST(Array, ConstAtOnZeroSizeThrows) {
    const ds::Array<int, 0> array{};
    EXPECT_THROW(array.at(0), std::out_of_range);
}

TEST(Array, ConstAccessDoesNotCopyElements) {
    const ds::Array<std::string, 2> array{"one", "two"};
    EXPECT_EQ(&array.at(0), &array[0]);
    EXPECT_EQ(&array[1], &array[0] + 1);
}

// ---------- Other element types ----------

TEST(Array, StoresStrings) {
    ds::Array<std::string, 3> array{"one", "two", "three"};
    EXPECT_EQ(array.at(0), "one");
    EXPECT_EQ(array[1], "two");
    array.at(2) += "!";
    EXPECT_EQ(array[2], "three!");
}

TEST(Array, DefaultStringsAreEmpty) {
    ds::Array<std::string, 2> array{};
    EXPECT_TRUE(array[0].empty());
    EXPECT_TRUE(array[1].empty());
}

// ---------- Copy (value semantics) ----------

TEST(Array, CopyConstructorCopiesValues) {
    ds::Array<int, 3> source{1, 2, 3};
    ds::Array<int, 3> copy(source);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST(Array, CopyIsIndependentOfSource) {
    ds::Array<int, 3> source{1, 2, 3};
    ds::Array<int, 3> copy(source);
    copy[0] = 100;
    source[2] = 300;
    EXPECT_EQ(source[0], 1);
    EXPECT_EQ(copy[2], 3);
}

TEST(Array, CopyAssignmentReplacesValues) {
    ds::Array<std::string, 2> source{"a", "b"};
    ds::Array<std::string, 2> target{"x", "y"};
    target = source;
    EXPECT_EQ(target[0], "a");
    EXPECT_EQ(target[1], "b");
    target[0] = "changed";
    EXPECT_EQ(source[0], "a");
}
