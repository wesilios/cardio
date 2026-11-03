#include "DataStructures/Queue/ListQueue.h"
#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void fill(ds::ListQueue<int> &queue, const std::vector<int> &values) {
    for (int value : values) {
        queue.enqueue(value);
    }
}

// printQueue() writes to stdout; capture it so it can be compared as a string.
template <typename T> std::string printed(const ds::ListQueue<T> &queue) {
    testing::internal::CaptureStdout();
    queue.printQueue();
    return testing::internal::GetCapturedStdout();
}

// Dequeues everything and returns the values in dequeue order (front first).
std::vector<int> drain(ds::ListQueue<int> &queue) {
    std::vector<int> values;
    while (!queue.isEmpty()) {
        values.push_back(queue.dequeue());
    }
    return values;
}

} // namespace

// ---------- Empty queue ----------

TEST(ListQueue, NewQueueIsEmpty) {
    ds::ListQueue<int> queue;
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(ListQueue, DequeueOnEmptyThrows) {
    ds::ListQueue<int> queue;
    EXPECT_THROW(queue.dequeue(), ds::EmptyContainer);
}

TEST(ListQueue, FrontOnEmptyThrows) {
    ds::ListQueue<int> queue;
    EXPECT_THROW(queue.front(), ds::EmptyContainer);
}

TEST(ListQueue, EmptyContainerIsAnOutOfRange) {
    ds::ListQueue<int> queue;
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
}

TEST(ListQueue, DequeueOnEmptyLeavesQueueUsable) {
    ds::ListQueue<int> queue;
    EXPECT_THROW(queue.dequeue(), ds::EmptyContainer);
    queue.enqueue(1);
    EXPECT_EQ(queue.size(), 1u);
    EXPECT_EQ(queue.dequeue(), 1);
}

// ---------- Enqueue / front ----------

TEST(ListQueue, EnqueueIncreasesSize) {
    ds::ListQueue<int> queue;
    queue.enqueue(1);
    EXPECT_EQ(queue.size(), 1u);
    EXPECT_FALSE(queue.isEmpty());
    queue.enqueue(2);
    queue.enqueue(3);
    EXPECT_EQ(queue.size(), 3u);
}

TEST(ListQueue, FrontReturnsFirstEnqueued) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    EXPECT_EQ(queue.front(), 1);
}

TEST(ListQueue, FrontDoesNotRemove) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.front();
    queue.front();
    EXPECT_EQ(queue.size(), 3u);
    EXPECT_EQ(queue.front(), 1);
}

TEST(ListQueue, FrontOnConstQueue) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2});
    const ds::ListQueue<int> &constQueue = queue;
    EXPECT_EQ(constQueue.front(), 1);
    EXPECT_EQ(constQueue.size(), 2u);
    EXPECT_FALSE(constQueue.isEmpty());
}

TEST(ListQueue, FrontFollowsTheFront) {
    ds::ListQueue<int> queue;
    queue.enqueue(1);
    EXPECT_EQ(queue.front(), 1);
    queue.enqueue(2);
    EXPECT_EQ(queue.front(), 1);
    queue.dequeue();
    EXPECT_EQ(queue.front(), 2);
}

// ---------- Dequeue (FIFO) ----------

TEST(ListQueue, DequeueReturnsFirstEnqueued) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2});
    EXPECT_EQ(queue.dequeue(), 1);
    EXPECT_EQ(queue.size(), 1u);
}

TEST(ListQueue, DequeuesInFirstInFirstOutOrder) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3, 4});
    EXPECT_EQ(drain(queue), std::vector<int>({1, 2, 3, 4}));
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ListQueue, EnqueueThenDequeueSingleElement) {
    ds::ListQueue<int> queue;
    queue.enqueue(7);
    EXPECT_EQ(queue.front(), 7);
    EXPECT_EQ(queue.dequeue(), 7);
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(ListQueue, DequeueUntilEmptyThenDequeueThrows) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2});
    queue.dequeue();
    queue.dequeue();
    EXPECT_THROW(queue.dequeue(), ds::EmptyContainer);
    EXPECT_THROW(queue.front(), ds::EmptyContainer);
}

TEST(ListQueue, DequeueUntilEmptyThenReuse) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    drain(queue);
    fill(queue, {4, 5});
    EXPECT_EQ(queue.size(), 2u);
    EXPECT_EQ(drain(queue), std::vector<int>({4, 5}));
}

TEST(ListQueue, InterleavedEnqueueAndDequeue) {
    ds::ListQueue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    EXPECT_EQ(queue.dequeue(), 1);
    queue.enqueue(3);
    queue.enqueue(4);
    EXPECT_EQ(queue.dequeue(), 2);
    queue.enqueue(5);
    EXPECT_EQ(drain(queue), std::vector<int>({3, 4, 5}));
}

TEST(ListQueue, KeepsDuplicates) {
    ds::ListQueue<int> queue;
    fill(queue, {2, 2, 1, 2});
    EXPECT_EQ(queue.size(), 4u);
    EXPECT_EQ(drain(queue), std::vector<int>({2, 2, 1, 2}));
}

TEST(ListQueue, ManyValuesDequeueInOrder) {
    ds::ListQueue<int> queue;
    for (int i = 0; i < 1000; ++i) {
        queue.enqueue(i);
    }
    EXPECT_EQ(queue.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(queue.dequeue(), i);
    }
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ListQueue, SlidingWindowKeepsOrder) {
    // Keeps the queue short while many values pass through it, as a queue
    // backed by a circular buffer would wrap around.
    ds::ListQueue<int> queue;
    fill(queue, {0, 1, 2});
    for (int i = 3; i < 100; ++i) {
        queue.enqueue(i);
        ASSERT_EQ(queue.dequeue(), i - 3);
        ASSERT_EQ(queue.size(), 3u);
    }
    EXPECT_EQ(drain(queue), std::vector<int>({97, 98, 99}));
}

// ---------- Clear ----------

TEST(ListQueue, ClearEmptiesQueue) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.clear();
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
    EXPECT_THROW(queue.front(), ds::EmptyContainer);
}

TEST(ListQueue, ClearOnEmptyQueueIsSafe) {
    ds::ListQueue<int> queue;
    queue.clear();
    queue.clear();
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ListQueue, ClearThenReuse) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.clear();
    fill(queue, {4, 5});
    EXPECT_EQ(drain(queue), std::vector<int>({4, 5}));
}

// ---------- Printing ----------
// Front of the queue first, "[  ]" when empty, and the line always ends with a newline.

TEST(ListQueue, PrintQueueListsFrontFirst) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    EXPECT_EQ(printed(queue), "[ 1, 2, 3 ]\n");
}

TEST(ListQueue, PrintQueueOnEmptyQueue) {
    ds::ListQueue<int> queue;
    EXPECT_EQ(printed(queue), "[  ]\n");
}

TEST(ListQueue, PrintQueueOnSingleElement) {
    ds::ListQueue<int> queue;
    queue.enqueue(7);
    EXPECT_EQ(printed(queue), "[ 7 ]\n");
}

TEST(ListQueue, PrintQueueAfterDequeue) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.dequeue();
    EXPECT_EQ(printed(queue), "[ 2, 3 ]\n");
}

TEST(ListQueue, PrintQueueAfterDequeuingEverything) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2});
    drain(queue);
    EXPECT_EQ(printed(queue), "[  ]\n");
}

TEST(ListQueue, PrintQueueDoesNotChangeQueue) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    printed(queue);
    EXPECT_EQ(queue.size(), 3u);
    EXPECT_EQ(drain(queue), std::vector<int>({1, 2, 3}));
}

// ---------- Other element types ----------

TEST(ListQueue, StoresStrings) {
    ds::ListQueue<std::string> queue;
    queue.enqueue("a");
    queue.enqueue("b");
    queue.enqueue("c");
    EXPECT_EQ(queue.front(), "a");
    EXPECT_EQ(printed(queue), "[ a, b, c ]\n");
    EXPECT_EQ(queue.dequeue(), "a");
    EXPECT_EQ(queue.dequeue(), "b");
    EXPECT_EQ(queue.size(), 1u);
}

// ---------- Through the Queue interface ----------

TEST(ListQueue, WorksThroughQueueReference) {
    ds::ListQueue<int> concrete;
    ds::Queue<int> &queue = concrete;
    queue.enqueue(1);
    queue.enqueue(2);
    EXPECT_EQ(queue.front(), 1);
    EXPECT_EQ(queue.dequeue(), 1);
    EXPECT_EQ(queue.size(), 1u);
}

TEST(ListQueue, DeleteThroughQueuePointerFreesEverything) {
    // Needs a virtual destructor in Queue; ASan reports a leak otherwise.
    std::unique_ptr<ds::Queue<int>> queue(new ds::ListQueue<int>());
    queue->enqueue(1);
    queue->enqueue(2);
    EXPECT_EQ(queue->size(), 2u);
}

// ---------- Copy ----------
// A copy is independent: changing one queue never changes the other.

TEST(ListQueue, CopyConstructorCopiesValues) {
    ds::ListQueue<int> original;
    fill(original, {1, 2, 3});
    ds::ListQueue<int> copy(original);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(drain(copy), std::vector<int>({1, 2, 3}));
    EXPECT_EQ(drain(original), std::vector<int>({1, 2, 3}));
}

TEST(ListQueue, CopyOfEmptyQueue) {
    ds::ListQueue<int> original;
    ds::ListQueue<int> copy(original);
    EXPECT_TRUE(copy.isEmpty());
}

TEST(ListQueue, ChangingCopyLeavesOriginalUnchanged) {
    ds::ListQueue<int> original;
    fill(original, {1, 2, 3});
    ds::ListQueue<int> copy(original);
    copy.dequeue();
    copy.enqueue(9);
    EXPECT_EQ(drain(copy), std::vector<int>({2, 3, 9}));
    EXPECT_EQ(drain(original), std::vector<int>({1, 2, 3}));
}

TEST(ListQueue, CopyOutlivesOriginal) {
    auto *original = new ds::ListQueue<int>();
    fill(*original, {1, 2});
    ds::ListQueue<int> copy(*original);
    delete original;
    EXPECT_EQ(drain(copy), std::vector<int>({1, 2}));
}

TEST(ListQueue, CopyAssignmentReplacesContents) {
    ds::ListQueue<int> source;
    fill(source, {1, 2});
    ds::ListQueue<int> target;
    fill(target, {7, 8, 9});
    target = source;
    EXPECT_EQ(drain(target), std::vector<int>({1, 2}));
    EXPECT_EQ(drain(source), std::vector<int>({1, 2}));
}

TEST(ListQueue, SelfCopyAssignmentKeepsContents) {
    ds::ListQueue<int> queue;
    fill(queue, {1, 2, 3});
    ds::ListQueue<int> &alias = queue;
    queue = alias;
    EXPECT_EQ(drain(queue), std::vector<int>({1, 2, 3}));
}

TEST(ListQueue, MoveConstructorKeepsValues) {
    ds::ListQueue<int> source;
    fill(source, {1, 2, 3});
    ds::ListQueue<int> moved(std::move(source));
    EXPECT_EQ(drain(moved), std::vector<int>({1, 2, 3}));
}

TEST(ListQueue, MoveAssignmentKeepsValues) {
    ds::ListQueue<int> source;
    fill(source, {1, 2});
    ds::ListQueue<int> target;
    fill(target, {9});
    target = std::move(source);
    EXPECT_EQ(drain(target), std::vector<int>({1, 2}));
}

// ---------- Move (rule of zero) ----------
// The class declares none of the five special members, so the compiler
// generates moves that forward to the member's own. A user-declared destructor
// would suppress them and std::move would silently copy.

TEST(ListQueue, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::ListQueue<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::ListQueue<int>>::value);
}

TEST(ListQueue, MoveConstructorLeavesSourceEmpty) {
    ds::ListQueue<int> source;
    fill(source, {1, 2, 3});
    ds::ListQueue<int> moved(std::move(source));
    EXPECT_TRUE(source.isEmpty());
}

TEST(ListQueue, MoveAssignmentLeavesSourceEmpty) {
    ds::ListQueue<int> source;
    fill(source, {1, 2});
    ds::ListQueue<int> target;
    target = std::move(source);
    EXPECT_TRUE(source.isEmpty());
}
