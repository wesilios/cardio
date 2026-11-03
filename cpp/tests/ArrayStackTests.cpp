#include "DataStructures/Stack/ArrayStack.h"
#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void fill(ds::ArrayStack<int> &stack, const std::vector<int> &values) {
    for (int value : values) {
        stack.push(value);
    }
}

// printStack() writes to stdout; capture it so it can be compared as a string.
template <typename T> std::string printed(const ds::ArrayStack<T> &stack) {
    testing::internal::CaptureStdout();
    stack.printStack();
    return testing::internal::GetCapturedStdout();
}

// Pops everything and returns the values in pop order (top first).
std::vector<int> drain(ds::ArrayStack<int> &stack) {
    std::vector<int> values;
    while (!stack.isEmpty()) {
        values.push_back(stack.pop());
    }
    return values;
}

} // namespace

// ---------- Empty stack ----------

TEST(ArrayStack, NewStackIsEmpty) {
    ds::ArrayStack<int> stack;
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0u);
}

TEST(ArrayStack, PopOnEmptyThrows) {
    ds::ArrayStack<int> stack;
    EXPECT_THROW(stack.pop(), ds::EmptyContainer);
}

TEST(ArrayStack, PeekOnEmptyThrows) {
    ds::ArrayStack<int> stack;
    EXPECT_THROW(stack.peek(), ds::EmptyContainer);
}

TEST(ArrayStack, EmptyContainerIsAnOutOfRange) {
    ds::ArrayStack<int> stack;
    EXPECT_THROW(stack.pop(), std::out_of_range);
}

TEST(ArrayStack, PopOnEmptyLeavesStackUsable) {
    ds::ArrayStack<int> stack;
    EXPECT_THROW(stack.pop(), ds::EmptyContainer);
    stack.push(1);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_EQ(stack.pop(), 1);
}

// ---------- Push / peek ----------

TEST(ArrayStack, PushIncreasesSize) {
    ds::ArrayStack<int> stack;
    stack.push(1);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_FALSE(stack.isEmpty());
    stack.push(2);
    stack.push(3);
    EXPECT_EQ(stack.size(), 3u);
}

TEST(ArrayStack, PeekReturnsLastPushed) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    EXPECT_EQ(stack.peek(), 3);
}

TEST(ArrayStack, PeekDoesNotRemove) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.peek();
    stack.peek();
    EXPECT_EQ(stack.size(), 3u);
    EXPECT_EQ(stack.peek(), 3);
}

TEST(ArrayStack, PeekOnConstStack) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2});
    const ds::ArrayStack<int> &constStack = stack;
    EXPECT_EQ(constStack.peek(), 2);
    EXPECT_EQ(constStack.size(), 2u);
    EXPECT_FALSE(constStack.isEmpty());
}

TEST(ArrayStack, PeekFollowsTheTop) {
    ds::ArrayStack<int> stack;
    stack.push(1);
    EXPECT_EQ(stack.peek(), 1);
    stack.push(2);
    EXPECT_EQ(stack.peek(), 2);
    stack.pop();
    EXPECT_EQ(stack.peek(), 1);
}

// ---------- Pop (LIFO) ----------

TEST(ArrayStack, PopReturnsLastPushed) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2});
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(ArrayStack, PopsInLastInFirstOutOrder) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3, 4});
    EXPECT_EQ(drain(stack), std::vector<int>({4, 3, 2, 1}));
    EXPECT_TRUE(stack.isEmpty());
}

TEST(ArrayStack, PushThenPopSingleElement) {
    ds::ArrayStack<int> stack;
    stack.push(7);
    EXPECT_EQ(stack.peek(), 7);
    EXPECT_EQ(stack.pop(), 7);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0u);
}

TEST(ArrayStack, PopUntilEmptyThenPopThrows) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2});
    stack.pop();
    stack.pop();
    EXPECT_THROW(stack.pop(), ds::EmptyContainer);
    EXPECT_THROW(stack.peek(), ds::EmptyContainer);
}

TEST(ArrayStack, PopUntilEmptyThenReuse) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    drain(stack);
    fill(stack, {4, 5});
    EXPECT_EQ(stack.size(), 2u);
    EXPECT_EQ(drain(stack), std::vector<int>({5, 4}));
}

TEST(ArrayStack, InterleavedPushAndPop) {
    ds::ArrayStack<int> stack;
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.pop(), 2);
    stack.push(3);
    stack.push(4);
    EXPECT_EQ(stack.pop(), 4);
    stack.push(5);
    EXPECT_EQ(drain(stack), std::vector<int>({5, 3, 1}));
}

TEST(ArrayStack, KeepsDuplicates) {
    ds::ArrayStack<int> stack;
    fill(stack, {2, 2, 1, 2});
    EXPECT_EQ(stack.size(), 4u);
    EXPECT_EQ(drain(stack), std::vector<int>({2, 1, 2, 2}));
}

TEST(ArrayStack, ManyValuesPopInReverseOrder) {
    ds::ArrayStack<int> stack;
    for (int i = 0; i < 1000; ++i) {
        stack.push(i);
    }
    EXPECT_EQ(stack.size(), 1000u);
    for (int i = 999; i >= 0; --i) {
        ASSERT_EQ(stack.pop(), i);
    }
    EXPECT_TRUE(stack.isEmpty());
}

// ---------- Clear ----------

TEST(ArrayStack, ClearEmptiesStack) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0u);
    EXPECT_THROW(stack.peek(), ds::EmptyContainer);
}

TEST(ArrayStack, ClearOnEmptyStackIsSafe) {
    ds::ArrayStack<int> stack;
    stack.clear();
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST(ArrayStack, ClearThenReuse) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.clear();
    fill(stack, {4, 5});
    EXPECT_EQ(drain(stack), std::vector<int>({5, 4}));
}

// ---------- Printing ----------
// Top of the stack first, "[  ]" when empty, and the line always ends with a newline.

TEST(ArrayStack, PrintStackListsTopFirst) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    EXPECT_EQ(printed(stack), "[ 3, 2, 1 ]\n");
}

TEST(ArrayStack, PrintStackOnEmptyStack) {
    ds::ArrayStack<int> stack;
    EXPECT_EQ(printed(stack), "[  ]\n");
}

TEST(ArrayStack, PrintStackOnSingleElement) {
    ds::ArrayStack<int> stack;
    stack.push(7);
    EXPECT_EQ(printed(stack), "[ 7 ]\n");
}

TEST(ArrayStack, PrintStackAfterPop) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.pop();
    EXPECT_EQ(printed(stack), "[ 2, 1 ]\n");
}

TEST(ArrayStack, PrintStackAfterPoppingEverything) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2});
    drain(stack);
    EXPECT_EQ(printed(stack), "[  ]\n");
}

TEST(ArrayStack, PrintStackDoesNotChangeStack) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    printed(stack);
    EXPECT_EQ(stack.size(), 3u);
    EXPECT_EQ(drain(stack), std::vector<int>({3, 2, 1}));
}

// ---------- Other element types ----------

TEST(ArrayStack, StoresStrings) {
    ds::ArrayStack<std::string> stack;
    stack.push("a");
    stack.push("b");
    stack.push("c");
    EXPECT_EQ(stack.peek(), "c");
    EXPECT_EQ(printed(stack), "[ c, b, a ]\n");
    EXPECT_EQ(stack.pop(), "c");
    EXPECT_EQ(stack.pop(), "b");
    EXPECT_EQ(stack.size(), 1u);
}

// ---------- Through the Stack interface ----------

TEST(ArrayStack, WorksThroughStackReference) {
    ds::ArrayStack<int> concrete;
    ds::Stack<int> &stack = concrete;
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.peek(), 2);
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(ArrayStack, DeleteThroughStackPointerFreesEverything) {
    // Needs a virtual destructor in Stack; ASan reports a leak otherwise.
    std::unique_ptr<ds::Stack<int>> stack(new ds::ArrayStack<int>());
    stack->push(1);
    stack->push(2);
    EXPECT_EQ(stack->size(), 2u);
}

// ---------- Copy ----------
// A copy is independent: changing one stack never changes the other.

TEST(ArrayStack, CopyConstructorCopiesValues) {
    ds::ArrayStack<int> original;
    fill(original, {1, 2, 3});
    ds::ArrayStack<int> copy(original);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(drain(copy), std::vector<int>({3, 2, 1}));
    EXPECT_EQ(drain(original), std::vector<int>({3, 2, 1}));
}

TEST(ArrayStack, CopyOfEmptyStack) {
    ds::ArrayStack<int> original;
    ds::ArrayStack<int> copy(original);
    EXPECT_TRUE(copy.isEmpty());
}

TEST(ArrayStack, ChangingCopyLeavesOriginalUnchanged) {
    ds::ArrayStack<int> original;
    fill(original, {1, 2, 3});
    ds::ArrayStack<int> copy(original);
    copy.pop();
    copy.push(9);
    EXPECT_EQ(drain(copy), std::vector<int>({9, 2, 1}));
    EXPECT_EQ(drain(original), std::vector<int>({3, 2, 1}));
}

TEST(ArrayStack, CopyOutlivesOriginal) {
    auto *original = new ds::ArrayStack<int>();
    fill(*original, {1, 2});
    ds::ArrayStack<int> copy(*original);
    delete original;
    EXPECT_EQ(drain(copy), std::vector<int>({2, 1}));
}

TEST(ArrayStack, CopyAssignmentReplacesContents) {
    ds::ArrayStack<int> source;
    fill(source, {1, 2});
    ds::ArrayStack<int> target;
    fill(target, {7, 8, 9});
    target = source;
    EXPECT_EQ(drain(target), std::vector<int>({2, 1}));
    EXPECT_EQ(drain(source), std::vector<int>({2, 1}));
}

TEST(ArrayStack, SelfCopyAssignmentKeepsContents) {
    ds::ArrayStack<int> stack;
    fill(stack, {1, 2, 3});
    ds::ArrayStack<int> &alias = stack;
    stack = alias;
    EXPECT_EQ(drain(stack), std::vector<int>({3, 2, 1}));
}

TEST(ArrayStack, MoveConstructorKeepsValues) {
    ds::ArrayStack<int> source;
    fill(source, {1, 2, 3});
    ds::ArrayStack<int> moved(std::move(source));
    EXPECT_EQ(drain(moved), std::vector<int>({3, 2, 1}));
}

TEST(ArrayStack, MoveAssignmentKeepsValues) {
    ds::ArrayStack<int> source;
    fill(source, {1, 2});
    ds::ArrayStack<int> target;
    fill(target, {9});
    target = std::move(source);
    EXPECT_EQ(drain(target), std::vector<int>({2, 1}));
}

// ---------- Move (rule of zero) ----------
// The class declares none of the five special members, so the compiler
// generates moves that forward to the member's own. A user-declared destructor
// would suppress them and std::move would silently copy.

TEST(ArrayStack, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::ArrayStack<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::ArrayStack<int>>::value);
}

TEST(ArrayStack, MoveConstructorLeavesSourceEmpty) {
    ds::ArrayStack<int> source;
    fill(source, {1, 2, 3});
    ds::ArrayStack<int> moved(std::move(source));
    EXPECT_TRUE(source.isEmpty());
}

TEST(ArrayStack, MoveAssignmentLeavesSourceEmpty) {
    ds::ArrayStack<int> source;
    fill(source, {1, 2});
    ds::ArrayStack<int> target;
    target = std::move(source);
    EXPECT_TRUE(source.isEmpty());
}
