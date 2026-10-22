#include "DataStructures/LinkedList/CircularDoublyLinkedList.h"
#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

namespace {

std::string describe(const std::vector<int> &values) {
    std::stringstream os;
    os << "[";
    for (size_t i = 0; i < values.size(); ++i) {
        os << (i ? ", " : "") << values[i];
    }
    os << "]";
    return os.str();
}

void fill(dsa::CircularDoublyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, that tail->next is head and head->previous is tail,
// and that the previous links give the same values walking back from the tail.
::testing::AssertionResult HasContents(dsa::CircularDoublyLinkedList<int> &list, const std::vector<int> &expected) {
    const int expectedSize = static_cast<int>(expected.size());
    if (list.size() != expectedSize) {
        return ::testing::AssertionFailure() << "size() is " << list.size() << ", expected " << expectedSize;
    }
    if (list.isEmpty() != expected.empty()) {
        return ::testing::AssertionFailure() << "isEmpty() is " << std::boolalpha << list.isEmpty();
    }
    if (expected.empty()) {
        if (list.getHead() != nullptr || list.getTail() != nullptr) {
            return ::testing::AssertionFailure() << "empty list must have null head and tail";
        }
        return ::testing::AssertionSuccess();
    }
    if (list.getHead() == nullptr || list.getTail() == nullptr) {
        return ::testing::AssertionFailure() << "non-empty list has a null head or tail";
    }

    // Walk size() nodes from the head; a broken circular link can't loop forever.
    std::vector<int> forward;
    auto *node = list.getHead();
    auto *last = node;
    for (int i = 0; node != nullptr && i < list.size(); ++i) {
        forward.push_back(node->getData());
        last = node;
        node = node->getNext();
    }
    if (forward != expected) {
        return ::testing::AssertionFailure()
               << "contents are " << describe(forward) << ", expected " << describe(expected);
    }
    if (last != list.getTail()) {
        return ::testing::AssertionFailure() << "the last node is not tail";
    }
    if (list.getTail()->getNext() != list.getHead()) {
        return ::testing::AssertionFailure() << "tail->next must be head";
    }
    if (list.getHead()->getPrevious() != list.getTail()) {
        return ::testing::AssertionFailure() << "head->previous must be tail";
    }

    // Walk size() nodes back from the tail through the previous links.
    std::vector<int> backward;
    node = list.getTail();
    for (int i = 0; node != nullptr && i < list.size(); ++i) {
        backward.insert(backward.begin(), node->getData());
        node = node->getPrevious();
    }
    if (backward != expected) {
        return ::testing::AssertionFailure() << "walking previous links from tail gives " << describe(backward);
    }
    return ::testing::AssertionSuccess();
}

} // namespace

// ---------- Adding ----------

TEST(CircularDoublyLinkedList, NewListIsEmpty) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, AddAppendsInOrder) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddLastAppends) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    dsa::CircularDoublyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, AddFirstPrepends) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    dsa::CircularDoublyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, AddFirstAndAddLastMixed) {
    dsa::CircularDoublyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(CircularDoublyLinkedList, DeleteFirstRemovesHead) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteLastRemovesTail) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    dsa::CircularDoublyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteLastOnEmptyListIsSafe) {
    dsa::CircularDoublyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.deleteFirst();
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(CircularDoublyLinkedList, DeleteLastUntilEmptyThenReuse) {
    // This is how ListStack::pop() drains a stack.
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(CircularDoublyLinkedList, DeleteFirstThenAddFirst) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteLastThenAddLast) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(CircularDoublyLinkedList, DeleteNodeRemovesMiddleValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesHeadValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesTailValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    list.printList();
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeIgnoresMissingValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    dsa::CircularDoublyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesEveryMatch) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(CircularDoublyLinkedList, AddAtIndexZeroOnEmptyList) {
    dsa::CircularDoublyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, AddAtIndexZeroPrepends) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddAtIndexOneInsertsAfterHead) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddAtIndexInsertsInMiddle) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularDoublyLinkedList, AddAtIndexBeforeTail) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularDoublyLinkedList, AddAtIndexSizeAppends) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsNegativeIndex) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsIndexPastSize) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularDoublyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20, 30});

    testing::internal::CaptureStdout();
    list.findByIndex(0);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "10\n");

    testing::internal::CaptureStdout();
    list.findByIndex(1);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "20\n");

    testing::internal::CaptureStdout();
    list.findByIndex(2);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "30\n");
}

TEST(CircularDoublyLinkedList, FindByIndexRejectsNegativeIndex) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), dsa::InvalidArgument);
}

TEST(CircularDoublyLinkedList, FindByIndexRejectsIndexPastEnd) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), dsa::InvalidArgument);
}

TEST(CircularDoublyLinkedList, FindByIndexOnEmptyListThrows) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), dsa::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(CircularDoublyLinkedList, ClearEmptiesListAndAllowsReuse) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(CircularDoublyLinkedList, ClearOnEmptyListIsSafe) {
    dsa::CircularDoublyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, ClearOnSingleNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, ClearTwiceIsSafe) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(CircularDoublyLinkedList, DestructorOnEmptyList) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularDoublyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}
