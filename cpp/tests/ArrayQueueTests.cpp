#include "DataStructures/Queue/ArrayQueue.h"
#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void fill(ds::ArrayQueue<int> &queue, const std::vector<int> &values) {
    for (int value : values) {
        queue.enqueue(value);
    }
}

// printQueue() writes to stdout; capture it so it can be compared as a string.
template <typename T> std::string printed(const ds::ArrayQueue<T> &queue) {
    testing::internal::CaptureStdout();
    queue.printQueue();
    return testing::internal::GetCapturedStdout();
}

// Dequeues everything and returns the values in dequeue order (front first).
std::vector<int> drain(ds::ArrayQueue<int> &queue) {
    std::vector<int> values;
    while (!queue.isEmpty()) {
        values.push_back(queue.dequeue());
    }
    return values;
}

} // namespace

// ---------- Empty queue ----------

TEST(ArrayQueue, NewQueueIsEmpty) {
    ds::ArrayQueue<int> queue;
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(ArrayQueue, DequeueOnEmptyThrows) {
    ds::ArrayQueue<int> queue;
    EXPECT_THROW(queue.dequeue(), ds::EmptyContainer);
}

TEST(ArrayQueue, FrontOnEmptyThrows) {
    ds::ArrayQueue<int> queue;
    EXPECT_THROW(queue.front(), ds::EmptyContainer);
}

TEST(ArrayQueue, EmptyContainerIsAnOutOfRange) {
    ds::ArrayQueue<int> queue;
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
}

TEST(ArrayQueue, DequeueOnEmptyLeavesQueueUsable) {
    ds::ArrayQueue<int> queue;
    EXPECT_THROW(queue.dequeue(), ds::EmptyContainer);
    queue.enqueue(1);
    EXPECT_EQ(queue.size(), 1u);
    EXPECT_EQ(queue.dequeue(), 1);
}

// ---------- Enqueue / front ----------

TEST(ArrayQueue, EnqueueIncreasesSize) {
    ds::ArrayQueue<int> queue;
    queue.enqueue(1);
    EXPECT_EQ(queue.size(), 1u);
    EXPECT_FALSE(queue.isEmpty());
    queue.enqueue(2);
    queue.enqueue(3);
    EXPECT_EQ(queue.size(), 3u);
}

TEST(ArrayQueue, FrontReturnsFirstEnqueued) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    EXPECT_EQ(queue.front(), 1);
}

TEST(ArrayQueue, FrontDoesNotRemove) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.front();
    queue.front();
    EXPECT_EQ(queue.size(), 3u);
    EXPECT_EQ(queue.front(), 1);
}

TEST(ArrayQueue, FrontOnConstQueue) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2});
    const ds::ArrayQueue<int> &constQueue = queue;
    EXPECT_EQ(constQueue.front(), 1);
    EXPECT_EQ(constQueue.size(), 2u);
    EXPECT_FALSE(constQueue.isEmpty());
}

TEST(ArrayQueue, FrontFollowsTheFront) {
    ds::ArrayQueue<int> queue;
    queue.enqueue(1);
    EXPECT_EQ(queue.front(), 1);
    queue.enqueue(2);
    EXPECT_EQ(queue.front(), 1);
    queue.dequeue();
    EXPECT_EQ(queue.front(), 2);
}

// ---------- Dequeue (FIFO) ----------

TEST(ArrayQueue, DequeueReturnsFirstEnqueued) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2});
    EXPECT_EQ(queue.dequeue(), 1);
    EXPECT_EQ(queue.size(), 1u);
}

TEST(ArrayQueue, DequeuesInFirstInFirstOutOrder) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3, 4});
    EXPECT_EQ(drain(queue), std::vector<int>({1, 2, 3, 4}));
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ArrayQueue, EnqueueThenDequeueSingleElement) {
    ds::ArrayQueue<int> queue;
    queue.enqueue(7);
    EXPECT_EQ(queue.front(), 7);
    EXPECT_EQ(queue.dequeue(), 7);
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(ArrayQueue, DequeueUntilEmptyThenDequeueThrows) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2});
    queue.dequeue();
    queue.dequeue();
    EXPECT_THROW(queue.dequeue(), ds::EmptyContainer);
    EXPECT_THROW(queue.front(), ds::EmptyContainer);
}

TEST(ArrayQueue, DequeueUntilEmptyThenReuse) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    drain(queue);
    fill(queue, {4, 5});
    EXPECT_EQ(queue.size(), 2u);
    EXPECT_EQ(drain(queue), std::vector<int>({4, 5}));
}

TEST(ArrayQueue, InterleavedEnqueueAndDequeue) {
    ds::ArrayQueue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    EXPECT_EQ(queue.dequeue(), 1);
    queue.enqueue(3);
    queue.enqueue(4);
    EXPECT_EQ(queue.dequeue(), 2);
    queue.enqueue(5);
    EXPECT_EQ(drain(queue), std::vector<int>({3, 4, 5}));
}

TEST(ArrayQueue, KeepsDuplicates) {
    ds::ArrayQueue<int> queue;
    fill(queue, {2, 2, 1, 2});
    EXPECT_EQ(queue.size(), 4u);
    EXPECT_EQ(drain(queue), std::vector<int>({2, 2, 1, 2}));
}

TEST(ArrayQueue, ManyValuesDequeueInOrder) {
    ds::ArrayQueue<int> queue;
    for (int i = 0; i < 1000; ++i) {
        queue.enqueue(i);
    }
    EXPECT_EQ(queue.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(queue.dequeue(), i);
    }
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ArrayQueue, SlidingWindowKeepsOrder) {
    // Keeps the queue short while many values pass through it, as a queue
    // backed by a circular buffer would wrap around.
    ds::ArrayQueue<int> queue;
    fill(queue, {0, 1, 2});
    for (int i = 3; i < 100; ++i) {
        queue.enqueue(i);
        ASSERT_EQ(queue.dequeue(), i - 3);
        ASSERT_EQ(queue.size(), 3u);
    }
    EXPECT_EQ(drain(queue), std::vector<int>({97, 98, 99}));
}

// ---------- Clear ----------

TEST(ArrayQueue, ClearEmptiesQueue) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.clear();
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0u);
    EXPECT_THROW(queue.front(), ds::EmptyContainer);
}

TEST(ArrayQueue, ClearOnEmptyQueueIsSafe) {
    ds::ArrayQueue<int> queue;
    queue.clear();
    queue.clear();
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ArrayQueue, ClearThenReuse) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.clear();
    fill(queue, {4, 5});
    EXPECT_EQ(drain(queue), std::vector<int>({4, 5}));
}

// ---------- Printing ----------
// Front of the queue first, "[  ]" when empty, and the line always ends with a newline.

TEST(ArrayQueue, PrintQueueListsFrontFirst) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    EXPECT_EQ(printed(queue), "[ 1, 2, 3 ]\n");
}

TEST(ArrayQueue, PrintQueueOnEmptyQueue) {
    ds::ArrayQueue<int> queue;
    EXPECT_EQ(printed(queue), "[  ]\n");
}

TEST(ArrayQueue, PrintQueueOnSingleElement) {
    ds::ArrayQueue<int> queue;
    queue.enqueue(7);
    EXPECT_EQ(printed(queue), "[ 7 ]\n");
}

TEST(ArrayQueue, PrintQueueAfterDequeue) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    queue.dequeue();
    EXPECT_EQ(printed(queue), "[ 2, 3 ]\n");
}

TEST(ArrayQueue, PrintQueueAfterDequeuingEverything) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2});
    drain(queue);
    EXPECT_EQ(printed(queue), "[  ]\n");
}

TEST(ArrayQueue, PrintQueueDoesNotChangeQueue) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    printed(queue);
    EXPECT_EQ(queue.size(), 3u);
    EXPECT_EQ(drain(queue), std::vector<int>({1, 2, 3}));
}

// ---------- Other element types ----------

TEST(ArrayQueue, StoresStrings) {
    ds::ArrayQueue<std::string> queue;
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

TEST(ArrayQueue, WorksThroughQueueReference) {
    ds::ArrayQueue<int> concrete;
    ds::Queue<int> &queue = concrete;
    queue.enqueue(1);
    queue.enqueue(2);
    EXPECT_EQ(queue.front(), 1);
    EXPECT_EQ(queue.dequeue(), 1);
    EXPECT_EQ(queue.size(), 1u);
}

TEST(ArrayQueue, DeleteThroughQueuePointerFreesEverything) {
    // Needs a virtual destructor in Queue; ASan reports a leak otherwise.
    std::unique_ptr<ds::Queue<int>> queue(new ds::ArrayQueue<int>());
    queue->enqueue(1);
    queue->enqueue(2);
    EXPECT_EQ(queue->size(), 2u);
}

// ---------- Copy ----------
// A copy is independent: changing one queue never changes the other.

TEST(ArrayQueue, CopyConstructorCopiesValues) {
    ds::ArrayQueue<int> original;
    fill(original, {1, 2, 3});
    ds::ArrayQueue<int> copy(original);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(drain(copy), std::vector<int>({1, 2, 3}));
    EXPECT_EQ(drain(original), std::vector<int>({1, 2, 3}));
}

TEST(ArrayQueue, CopyOfEmptyQueue) {
    ds::ArrayQueue<int> original;
    ds::ArrayQueue<int> copy(original);
    EXPECT_TRUE(copy.isEmpty());
}

TEST(ArrayQueue, ChangingCopyLeavesOriginalUnchanged) {
    ds::ArrayQueue<int> original;
    fill(original, {1, 2, 3});
    ds::ArrayQueue<int> copy(original);
    copy.dequeue();
    copy.enqueue(9);
    EXPECT_EQ(drain(copy), std::vector<int>({2, 3, 9}));
    EXPECT_EQ(drain(original), std::vector<int>({1, 2, 3}));
}

TEST(ArrayQueue, CopyOutlivesOriginal) {
    auto *original = new ds::ArrayQueue<int>();
    fill(*original, {1, 2});
    ds::ArrayQueue<int> copy(*original);
    delete original;
    EXPECT_EQ(drain(copy), std::vector<int>({1, 2}));
}

TEST(ArrayQueue, CopyAssignmentReplacesContents) {
    ds::ArrayQueue<int> source;
    fill(source, {1, 2});
    ds::ArrayQueue<int> target;
    fill(target, {7, 8, 9});
    target = source;
    EXPECT_EQ(drain(target), std::vector<int>({1, 2}));
    EXPECT_EQ(drain(source), std::vector<int>({1, 2}));
}

TEST(ArrayQueue, SelfCopyAssignmentKeepsContents) {
    ds::ArrayQueue<int> queue;
    fill(queue, {1, 2, 3});
    ds::ArrayQueue<int> &alias = queue;
    queue = alias;
    EXPECT_EQ(drain(queue), std::vector<int>({1, 2, 3}));
}

TEST(ArrayQueue, MoveConstructorKeepsValues) {
    ds::ArrayQueue<int> source;
    fill(source, {1, 2, 3});
    ds::ArrayQueue<int> moved(std::move(source));
    EXPECT_EQ(drain(moved), std::vector<int>({1, 2, 3}));
}

TEST(ArrayQueue, MoveAssignmentKeepsValues) {
    ds::ArrayQueue<int> source;
    fill(source, {1, 2});
    ds::ArrayQueue<int> target;
    fill(target, {9});
    target = std::move(source);
    EXPECT_EQ(drain(target), std::vector<int>({1, 2}));
}

// ---------- Move (rule of zero) ----------
// The class declares none of the five special members, so the compiler
// generates moves that forward to the member's own. A user-declared destructor
// would suppress them and std::move would silently copy.

TEST(ArrayQueue, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::ArrayQueue<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::ArrayQueue<int>>::value);
}

TEST(ArrayQueue, MoveConstructorLeavesSourceEmpty) {
    ds::ArrayQueue<int> source;
    fill(source, {1, 2, 3});
    ds::ArrayQueue<int> moved(std::move(source));
    EXPECT_TRUE(source.isEmpty());
}

TEST(ArrayQueue, MoveAssignmentLeavesSourceEmpty) {
    ds::ArrayQueue<int> source;
    fill(source, {1, 2});
    ds::ArrayQueue<int> target;
    target = std::move(source);
    EXPECT_TRUE(source.isEmpty());
}
