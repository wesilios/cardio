#include "DataStructures/Stack/ListStack.h"
#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void fill(ds::ListStack<int> &stack, const std::vector<int> &values) {
    for (int value : values) {
        stack.push(value);
    }
}

// printStack() writes to stdout; capture it so it can be compared as a string.
template <typename T> std::string printed(const ds::ListStack<T> &stack) {
    testing::internal::CaptureStdout();
    stack.printStack();
    return testing::internal::GetCapturedStdout();
}

// Pops everything and returns the values in pop order (top first).
std::vector<int> drain(ds::ListStack<int> &stack) {
    std::vector<int> values;
    while (!stack.isEmpty()) {
        values.push_back(stack.pop());
    }
    return values;
}

} // namespace

// ---------- Empty stack ----------

TEST(ListStack, NewStackIsEmpty) {
    ds::ListStack<int> stack;
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0u);
}

TEST(ListStack, PopOnEmptyThrows) {
    ds::ListStack<int> stack;
    EXPECT_THROW(stack.pop(), ds::EmptyContainer);
}

TEST(ListStack, PeekOnEmptyThrows) {
    ds::ListStack<int> stack;
    EXPECT_THROW(stack.peek(), ds::EmptyContainer);
}

TEST(ListStack, EmptyContainerIsAnOutOfRange) {
    ds::ListStack<int> stack;
    EXPECT_THROW(stack.pop(), std::out_of_range);
}

TEST(ListStack, PopOnEmptyLeavesStackUsable) {
    ds::ListStack<int> stack;
    EXPECT_THROW(stack.pop(), ds::EmptyContainer);
    stack.push(1);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_EQ(stack.pop(), 1);
}

// ---------- Push / peek ----------

TEST(ListStack, PushIncreasesSize) {
    ds::ListStack<int> stack;
    stack.push(1);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_FALSE(stack.isEmpty());
    stack.push(2);
    stack.push(3);
    EXPECT_EQ(stack.size(), 3u);
}

TEST(ListStack, PeekReturnsLastPushed) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    EXPECT_EQ(stack.peek(), 3);
}

TEST(ListStack, PeekDoesNotRemove) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.peek();
    stack.peek();
    EXPECT_EQ(stack.size(), 3u);
    EXPECT_EQ(stack.peek(), 3);
}

TEST(ListStack, PeekOnConstStack) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2});
    const ds::ListStack<int> &constStack = stack;
    EXPECT_EQ(constStack.peek(), 2);
    EXPECT_EQ(constStack.size(), 2u);
    EXPECT_FALSE(constStack.isEmpty());
}

TEST(ListStack, PeekFollowsTheTop) {
    ds::ListStack<int> stack;
    stack.push(1);
    EXPECT_EQ(stack.peek(), 1);
    stack.push(2);
    EXPECT_EQ(stack.peek(), 2);
    stack.pop();
    EXPECT_EQ(stack.peek(), 1);
}

// ---------- Pop (LIFO) ----------

TEST(ListStack, PopReturnsLastPushed) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2});
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(ListStack, PopsInLastInFirstOutOrder) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3, 4});
    EXPECT_EQ(drain(stack), std::vector<int>({4, 3, 2, 1}));
    EXPECT_TRUE(stack.isEmpty());
}

TEST(ListStack, PushThenPopSingleElement) {
    ds::ListStack<int> stack;
    stack.push(7);
    EXPECT_EQ(stack.peek(), 7);
    EXPECT_EQ(stack.pop(), 7);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0u);
}

TEST(ListStack, PopUntilEmptyThenPopThrows) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2});
    stack.pop();
    stack.pop();
    EXPECT_THROW(stack.pop(), ds::EmptyContainer);
    EXPECT_THROW(stack.peek(), ds::EmptyContainer);
}

TEST(ListStack, PopUntilEmptyThenReuse) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    drain(stack);
    fill(stack, {4, 5});
    EXPECT_EQ(stack.size(), 2u);
    EXPECT_EQ(drain(stack), std::vector<int>({5, 4}));
}

TEST(ListStack, InterleavedPushAndPop) {
    ds::ListStack<int> stack;
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.pop(), 2);
    stack.push(3);
    stack.push(4);
    EXPECT_EQ(stack.pop(), 4);
    stack.push(5);
    EXPECT_EQ(drain(stack), std::vector<int>({5, 3, 1}));
}

TEST(ListStack, KeepsDuplicates) {
    ds::ListStack<int> stack;
    fill(stack, {2, 2, 1, 2});
    EXPECT_EQ(stack.size(), 4u);
    EXPECT_EQ(drain(stack), std::vector<int>({2, 1, 2, 2}));
}

TEST(ListStack, ManyValuesPopInReverseOrder) {
    ds::ListStack<int> stack;
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

TEST(ListStack, ClearEmptiesStack) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0u);
    EXPECT_THROW(stack.peek(), ds::EmptyContainer);
}

TEST(ListStack, ClearOnEmptyStackIsSafe) {
    ds::ListStack<int> stack;
    stack.clear();
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST(ListStack, ClearThenReuse) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.clear();
    fill(stack, {4, 5});
    EXPECT_EQ(drain(stack), std::vector<int>({5, 4}));
}

// ---------- Printing ----------
// Top of the stack first, "[  ]" when empty, and the line always ends with a newline.

TEST(ListStack, PrintStackListsTopFirst) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    EXPECT_EQ(printed(stack), "[ 3, 2, 1 ]\n");
}

TEST(ListStack, PrintStackOnEmptyStack) {
    ds::ListStack<int> stack;
    EXPECT_EQ(printed(stack), "[  ]\n");
}

TEST(ListStack, PrintStackOnSingleElement) {
    ds::ListStack<int> stack;
    stack.push(7);
    EXPECT_EQ(printed(stack), "[ 7 ]\n");
}

TEST(ListStack, PrintStackAfterPop) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    stack.pop();
    EXPECT_EQ(printed(stack), "[ 2, 1 ]\n");
}

TEST(ListStack, PrintStackAfterPoppingEverything) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2});
    drain(stack);
    EXPECT_EQ(printed(stack), "[  ]\n");
}

TEST(ListStack, PrintStackDoesNotChangeStack) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    printed(stack);
    EXPECT_EQ(stack.size(), 3u);
    EXPECT_EQ(drain(stack), std::vector<int>({3, 2, 1}));
}

// ---------- Other element types ----------

TEST(ListStack, StoresStrings) {
    ds::ListStack<std::string> stack;
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

TEST(ListStack, WorksThroughStackReference) {
    ds::ListStack<int> concrete;
    ds::Stack<int> &stack = concrete;
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.peek(), 2);
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(ListStack, DeleteThroughStackPointerFreesEverything) {
    // Needs a virtual destructor in Stack; ASan reports a leak otherwise.
    std::unique_ptr<ds::Stack<int>> stack(new ds::ListStack<int>());
    stack->push(1);
    stack->push(2);
    EXPECT_EQ(stack->size(), 2u);
}

// ---------- Copy ----------
// A copy is independent: changing one stack never changes the other.

TEST(ListStack, CopyConstructorCopiesValues) {
    ds::ListStack<int> original;
    fill(original, {1, 2, 3});
    ds::ListStack<int> copy(original);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(drain(copy), std::vector<int>({3, 2, 1}));
    EXPECT_EQ(drain(original), std::vector<int>({3, 2, 1}));
}

TEST(ListStack, CopyOfEmptyStack) {
    ds::ListStack<int> original;
    ds::ListStack<int> copy(original);
    EXPECT_TRUE(copy.isEmpty());
}

TEST(ListStack, ChangingCopyLeavesOriginalUnchanged) {
    ds::ListStack<int> original;
    fill(original, {1, 2, 3});
    ds::ListStack<int> copy(original);
    copy.pop();
    copy.push(9);
    EXPECT_EQ(drain(copy), std::vector<int>({9, 2, 1}));
    EXPECT_EQ(drain(original), std::vector<int>({3, 2, 1}));
}

TEST(ListStack, CopyOutlivesOriginal) {
    auto *original = new ds::ListStack<int>();
    fill(*original, {1, 2});
    ds::ListStack<int> copy(*original);
    delete original;
    EXPECT_EQ(drain(copy), std::vector<int>({2, 1}));
}

TEST(ListStack, CopyAssignmentReplacesContents) {
    ds::ListStack<int> source;
    fill(source, {1, 2});
    ds::ListStack<int> target;
    fill(target, {7, 8, 9});
    target = source;
    EXPECT_EQ(drain(target), std::vector<int>({2, 1}));
    EXPECT_EQ(drain(source), std::vector<int>({2, 1}));
}

TEST(ListStack, SelfCopyAssignmentKeepsContents) {
    ds::ListStack<int> stack;
    fill(stack, {1, 2, 3});
    ds::ListStack<int> &alias = stack;
    stack = alias;
    EXPECT_EQ(drain(stack), std::vector<int>({3, 2, 1}));
}

TEST(ListStack, MoveConstructorKeepsValues) {
    ds::ListStack<int> source;
    fill(source, {1, 2, 3});
    ds::ListStack<int> moved(std::move(source));
    EXPECT_EQ(drain(moved), std::vector<int>({3, 2, 1}));
}

TEST(ListStack, MoveAssignmentKeepsValues) {
    ds::ListStack<int> source;
    fill(source, {1, 2});
    ds::ListStack<int> target;
    fill(target, {9});
    target = std::move(source);
    EXPECT_EQ(drain(target), std::vector<int>({2, 1}));
}

// ---------- Move (rule of zero) ----------
// The class declares none of the five special members, so the compiler
// generates moves that forward to the member's own. A user-declared destructor
// would suppress them and std::move would silently copy.

TEST(ListStack, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::ListStack<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::ListStack<int>>::value);
}

TEST(ListStack, MoveConstructorLeavesSourceEmpty) {
    ds::ListStack<int> source;
    fill(source, {1, 2, 3});
    ds::ListStack<int> moved(std::move(source));
    EXPECT_TRUE(source.isEmpty());
}

TEST(ListStack, MoveAssignmentLeavesSourceEmpty) {
    ds::ListStack<int> source;
    fill(source, {1, 2});
    ds::ListStack<int> target;
    target = std::move(source);
    EXPECT_TRUE(source.isEmpty());
}
