#include "DataStructures/LinkedList/SinglyLinkedList.h"
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

void fill(dsa::SinglyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, and that tail->next is nullptr.
::testing::AssertionResult HasContents(dsa::SinglyLinkedList<int> &list, const std::vector<int> &expected) {
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
    return ::testing::AssertionSuccess();
}

} // namespace

// ---------- Adding ----------

TEST(SinglyLinkedList, NewListIsEmpty) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, AddAppendsInOrder) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, AddLastAppends) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    dsa::SinglyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(SinglyLinkedList, AddFirstPrepends) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    dsa::SinglyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(SinglyLinkedList, AddFirstAndAddLastMixed) {
    dsa::SinglyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(SinglyLinkedList, DeleteFirstRemovesHead) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(SinglyLinkedList, DeleteLastRemovesTail) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    dsa::SinglyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteLastOnEmptyListIsSafe) {
    dsa::SinglyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.deleteFirst();
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(SinglyLinkedList, DeleteLastUntilEmptyThenReuse) {
    // This is how ListStack::pop() drains a stack.
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(SinglyLinkedList, DeleteFirstThenAddFirst) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(SinglyLinkedList, DeleteLastThenAddLast) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(SinglyLinkedList, DeleteNodeRemovesMiddleValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(SinglyLinkedList, DeleteNodeRemovesHeadValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(SinglyLinkedList, DeleteNodeRemovesTailValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, DeleteNodeIgnoresMissingValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    dsa::SinglyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(SinglyLinkedList, DeleteNodeRemovesEveryMatch) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(SinglyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(SinglyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(SinglyLinkedList, AddAtIndexZeroOnEmptyList) {
    dsa::SinglyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(SinglyLinkedList, AddAtIndexZeroPrepends) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, AddAtIndexOneInsertsAfterHead) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, AddAtIndexInsertsInMiddle) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(SinglyLinkedList, AddAtIndexBeforeTail) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(SinglyLinkedList, AddAtIndexSizeAppends) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(SinglyLinkedList, AddAtIndexRejectsNegativeIndex) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, AddAtIndexRejectsIndexPastSize) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(SinglyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(SinglyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    dsa::SinglyLinkedList<int> list;
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

TEST(SinglyLinkedList, FindByIndexRejectsNegativeIndex) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), dsa::InvalidArgument);
}

TEST(SinglyLinkedList, FindByIndexRejectsIndexPastEnd) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), dsa::InvalidArgument);
}

TEST(SinglyLinkedList, FindByIndexOnEmptyListThrows) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), dsa::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(SinglyLinkedList, ClearEmptiesListAndAllowsReuse) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(SinglyLinkedList, ClearOnEmptyListIsSafe) {
    dsa::SinglyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, ClearOnSingleNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, ClearTwiceIsSafe) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(SinglyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(SinglyLinkedList, DestructorOnEmptyList) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(SinglyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

// ---------- Iterator ----------

TEST(SinglyLinkedList, RangeForVisitsEveryValueInOrder) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(SinglyLinkedList, RangeForOnEmptyListVisitsNothing) {
    dsa::SinglyLinkedList<int> list;
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_TRUE(visited.empty());
}

TEST(SinglyLinkedList, RangeForOnSingleNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {7});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({7}));
}

TEST(SinglyLinkedList, RangeForSeesChangesToTheList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(0);
    list.addLast(4);
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({0, 2, 3, 4}));
}

TEST(SinglyLinkedList, BeginEqualsEndOnEmptyList) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(SinglyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(SinglyLinkedList, DereferenceReturnsCurrentValue) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(SinglyLinkedList, PrefixIncrementMovesToNextNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(SinglyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(SinglyLinkedList, IncrementPastLastNodeReachesEnd) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(SinglyLinkedList, IncrementAtEndStaysAtEnd) {
    dsa::SinglyLinkedList<int> list;
    auto it = list.end();
    ++it;
    EXPECT_TRUE(it == list.end());
}

// ---------- Printing ----------

TEST(SinglyLinkedList, ToStringListsValues) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(SinglyLinkedList, StreamOperatorPrintsToString) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}

TEST(SinglyLinkedList, ToStringOnEmptyList) {
    dsa::SinglyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(SinglyLinkedList, ToStringOnSingleNode) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(SinglyLinkedList, ToStringAfterDeletes) {
    dsa::SinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}
