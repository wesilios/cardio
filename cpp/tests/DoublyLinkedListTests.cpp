#include "DataStructures/LinkedList/DoublyLinkedList.h"
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

void fill(dsa::DoublyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, that tail->next and head->previous are nullptr,
// and that the previous links give the same values walking back from the tail.
::testing::AssertionResult HasContents(dsa::DoublyLinkedList<int> &list, const std::vector<int> &expected) {
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

    // Walk size() nodes from the head.
    std::vector<int> forward;
    auto *node = list.getHead();
    auto *last = node;
    for (int i = 0; node != nullptr && i < list.size(); ++i) {
        forward.push_back(node->getData());
        last = node;
        node = node->getNext();
    }
    if (forward != expected) {
        return ::testing::AssertionFailure() << "contents are " << describe(forward) << ", expected "
                                             << describe(expected);
    }
    if (last != list.getTail()) {
        return ::testing::AssertionFailure() << "the last node is not tail";
    }
    if (list.getTail()->getNext() != nullptr) {
        return ::testing::AssertionFailure() << "tail->next must be nullptr";
    }
    if (list.getHead()->getPrevious() != nullptr) {
        return ::testing::AssertionFailure() << "head->previous must be nullptr";
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

TEST(DoublyLinkedList, NewListIsEmpty) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, AddAppendsInOrder) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddLastAppends) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    dsa::DoublyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, AddFirstPrepends) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    dsa::DoublyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, AddFirstAndAddLastMixed) {
    dsa::DoublyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(DoublyLinkedList, DeleteFirstRemovesHead) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(DoublyLinkedList, DeleteLastRemovesTail) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    dsa::DoublyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteLastOnEmptyListIsSafe) {
    dsa::DoublyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.deleteFirst();
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(DoublyLinkedList, DeleteLastUntilEmptyThenReuse) {
    // This is how ListStack::pop() drains a stack.
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(DoublyLinkedList, DeleteFirstThenAddFirst) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(DoublyLinkedList, DeleteLastThenAddLast) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(DoublyLinkedList, DeleteNodeRemovesMiddleValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesHeadValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesTailValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeIgnoresMissingValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    dsa::DoublyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesEveryMatch) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(DoublyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(DoublyLinkedList, AddAtIndexZeroOnEmptyList) {
    dsa::DoublyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, AddAtIndexZeroPrepends) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddAtIndexOneInsertsAfterHead) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddAtIndexInsertsInMiddle) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(DoublyLinkedList, AddAtIndexBeforeTail) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(DoublyLinkedList, AddAtIndexSizeAppends) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsNegativeIndex) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsIndexPastSize) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(DoublyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    dsa::DoublyLinkedList<int> list;
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

TEST(DoublyLinkedList, FindByIndexRejectsNegativeIndex) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), dsa::InvalidArgument);
}

TEST(DoublyLinkedList, FindByIndexRejectsIndexPastEnd) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), dsa::InvalidArgument);
}

TEST(DoublyLinkedList, FindByIndexOnEmptyListThrows) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), dsa::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(DoublyLinkedList, ClearEmptiesListAndAllowsReuse) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(DoublyLinkedList, ClearOnEmptyListIsSafe) {
    dsa::DoublyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, ClearOnSingleNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, ClearTwiceIsSafe) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(DoublyLinkedList, DestructorOnEmptyList) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(DoublyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

// ---------- Printing ----------

TEST(DoublyLinkedList, ToStringListsValues) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(DoublyLinkedList, StreamOperatorPrintsToString) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}
