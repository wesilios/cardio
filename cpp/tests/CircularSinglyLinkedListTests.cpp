#include "DataStructures/LinkedList/CircularSinglyLinkedList.h"
#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
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

void fill(dsa::CircularSinglyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, and that tail->next points back to head.
::testing::AssertionResult HasContents(dsa::CircularSinglyLinkedList<int> &list, const std::vector<int> &expected) {
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
    return ::testing::AssertionSuccess();
}

} // namespace

// ---------- Adding ----------

TEST(CircularSinglyLinkedList, NewListIsEmpty) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, AddAppendsInOrder) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddLastAppends) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    dsa::CircularSinglyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, AddFirstPrepends) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    dsa::CircularSinglyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, AddFirstAndAddLastMixed) {
    dsa::CircularSinglyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(CircularSinglyLinkedList, DeleteFirstRemovesHead) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteLastRemovesTail) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    dsa::CircularSinglyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteLastOnEmptyListIsSafe) {
    dsa::CircularSinglyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.deleteFirst();
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(CircularSinglyLinkedList, DeleteLastUntilEmptyThenReuse) {
    // This is how ListStack::pop() drains a stack.
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(CircularSinglyLinkedList, DeleteFirstThenAddFirst) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteLastThenAddLast) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(CircularSinglyLinkedList, DeleteNodeRemovesMiddleValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesHeadValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesTailValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeIgnoresMissingValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    dsa::CircularSinglyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesEveryMatch) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(CircularSinglyLinkedList, AddAtIndexZeroOnEmptyList) {
    dsa::CircularSinglyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, AddAtIndexZeroPrepends) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddAtIndexOneInsertsAfterHead) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddAtIndexInsertsInMiddle) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularSinglyLinkedList, AddAtIndexBeforeTail) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularSinglyLinkedList, AddAtIndexSizeAppends) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsNegativeIndex) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsIndexPastSize) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), dsa::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    dsa::CircularSinglyLinkedList<int> list;
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

TEST(CircularSinglyLinkedList, FindByIndexRejectsNegativeIndex) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), dsa::InvalidArgument);
}

TEST(CircularSinglyLinkedList, FindByIndexRejectsIndexPastEnd) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), dsa::InvalidArgument);
}

TEST(CircularSinglyLinkedList, FindByIndexOnEmptyListThrows) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), dsa::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(CircularSinglyLinkedList, ClearEmptiesListAndAllowsReuse) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(CircularSinglyLinkedList, ClearOnEmptyListIsSafe) {
    dsa::CircularSinglyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, ClearOnSingleNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, ClearTwiceIsSafe) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(CircularSinglyLinkedList, DestructorOnEmptyList) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularSinglyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

// ---------- Iterator ----------
// The tail links back to the head, so end() must not rely on reaching nullptr.
// Each range-for stops after 10 values so a broken end() fails the test instead of looping forever.

TEST(CircularSinglyLinkedList, RangeForVisitsEveryValueOnce) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(CircularSinglyLinkedList, RangeForOnEmptyListVisitsNothing) {
    dsa::CircularSinglyLinkedList<int> list;
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_TRUE(visited.empty());
}

TEST(CircularSinglyLinkedList, RangeForOnSingleNodeVisitsItOnce) {
    // The single node links to itself.
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_EQ(visited, std::vector<int>({7}));
}

TEST(CircularSinglyLinkedList, RangeForSeesChangesToTheList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(0);
    list.addLast(4);
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_EQ(visited, std::vector<int>({0, 2, 3, 4}));
}

TEST(CircularSinglyLinkedList, RangeForTwiceGivesSameValues) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::vector<int> first;
    for (int value : list) {
        first.push_back(value);
        if (first.size() > 10) {
            break;
        }
    }
    std::vector<int> second;
    for (int value : list) {
        second.push_back(value);
        if (second.size() > 10) {
            break;
        }
    }
    EXPECT_EQ(first, std::vector<int>({1, 2, 3}));
    EXPECT_EQ(second, std::vector<int>({1, 2, 3}));
}

TEST(CircularSinglyLinkedList, RangeForOverConstList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const dsa::CircularSinglyLinkedList<int> &constList = list;
    std::vector<int> visited;
    for (int value : constList) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(CircularSinglyLinkedList, BeginEqualsEndOnEmptyList) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(CircularSinglyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(CircularSinglyLinkedList, DereferenceReturnsCurrentValue) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(CircularSinglyLinkedList, PrefixIncrementMovesToNextNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(CircularSinglyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(CircularSinglyLinkedList, IncrementPastTailReachesEnd) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(CircularSinglyLinkedList, IncrementPastSingleNodeReachesEnd) {
    // The node's next is itself, so the iterator must still count it as one lap.
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    auto it = list.begin();
    ++it;
    EXPECT_TRUE(it == list.end());
}

// ---------- Printing ----------

TEST(CircularSinglyLinkedList, ToStringListsValues) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(CircularSinglyLinkedList, StreamOperatorPrintsToString) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}

TEST(CircularSinglyLinkedList, ToStringOnEmptyList) {
    dsa::CircularSinglyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(CircularSinglyLinkedList, ToStringOnSingleNode) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(CircularSinglyLinkedList, ToStringAfterDeletes) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}

// ---------- Copy (rule of three) ----------
// A copy owns its own nodes (deep copy): changing one list never changes the
// other. Double frees and leaks here only show up reliably in the sanitizer build.

namespace {

std::vector<const dsa::SinglyNode<int> *> nodesOf(const dsa::CircularSinglyLinkedList<int> &list) {
    std::vector<const dsa::SinglyNode<int> *> nodes;
    auto *node = list.getHead();
    for (std::size_t i = 0; node != nullptr && i < list.size(); ++i) {
        nodes.push_back(node);
        node = node->getNext();
    }
    return nodes;
}

dsa::CircularSinglyLinkedList<int> makeList(const std::vector<int> &values) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, values);
    return list;
}

} // namespace

TEST(CircularSinglyLinkedList, IsCopyConstructibleAndAssignable) {
    EXPECT_TRUE(std::is_copy_constructible<dsa::CircularSinglyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<dsa::CircularSinglyLinkedList<int>>::value);
}

// Copy constructor

TEST(CircularSinglyLinkedList, CopyConstructorCopiesValues) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyConstructorOfEmptyList) {
    dsa::CircularSinglyLinkedList<int> original;
    dsa::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {}));
    EXPECT_TRUE(HasContents(original, {}));
}

TEST(CircularSinglyLinkedList, CopyConstructorOfSingleNode) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {7});
    dsa::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {7}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

TEST(CircularSinglyLinkedList, CopyConstructorKeepsDuplicates) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {2, 2, 1, 2});
    dsa::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {2, 2, 1, 2}));
}

TEST(CircularSinglyLinkedList, CopyConstructorAllocatesNewNodes) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> copy(original);
    const auto originalNodes = nodesOf(original);
    for (const auto *node : nodesOf(copy)) {
        for (const auto *shared : originalNodes) {
            EXPECT_NE(node, shared) << "the copy shares a node with the original";
        }
    }
}

TEST(CircularSinglyLinkedList, ChangingCopyLeavesOriginalUnchanged) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> copy(original);
    copy.deleteFirst();
    copy.deleteLast();
    copy.addLast(9);
    copy.addFirst(0);
    EXPECT_TRUE(HasContents(copy, {0, 2, 9}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, ChangingOriginalLeavesCopyUnchanged) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> copy(original);
    original.clear();
    original.add(5);
    EXPECT_TRUE(HasContents(original, {5}));
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyOutlivesOriginal) {
    auto *original = new dsa::CircularSinglyLinkedList<int>();
    fill(*original, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> copy(*original);
    delete original;
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyInitializationWithEquals) {
    dsa::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2});
    dsa::CircularSinglyLinkedList<int> copy = original;
    EXPECT_TRUE(HasContents(copy, {1, 2}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

// Copy assignment

TEST(CircularSinglyLinkedList, CopyAssignmentIntoEmptyList) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentReplacesLongerList) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9, 10});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentReplacesShorterList) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3, 4});
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {9});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3, 4}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentFromEmptyListEmptiesTarget) {
    dsa::CircularSinglyLinkedList<int> source;
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentThenAddUsesNewTail) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = source;
    target.add(3);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentAllocatesNewNodes) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> target;
    target = source;
    const auto sourceNodes = nodesOf(source);
    for (const auto *node : nodesOf(target)) {
        for (const auto *shared : sourceNodes) {
            EXPECT_NE(node, shared) << "the target shares a node with the source";
        }
    }
}

TEST(CircularSinglyLinkedList, ChangingAssignedListLeavesSourceUnchanged) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> target;
    target = source;
    target.deleteNode(2);
    EXPECT_TRUE(HasContents(target, {1, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, SelfCopyAssignmentKeepsContents) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> &alias = list;
    list = alias;
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentReturnsTarget) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1});
    dsa::CircularSinglyLinkedList<int> target;
    EXPECT_EQ(&(target = source), &target);
}

TEST(CircularSinglyLinkedList, ChainedCopyAssignment) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularSinglyLinkedList<int> first;
    dsa::CircularSinglyLinkedList<int> second;
    fill(second, {9, 9, 9});
    second = first = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
    EXPECT_NE(first.getHead(), second.getHead());
}

TEST(CircularSinglyLinkedList, ReturnByValue) {
    dsa::CircularSinglyLinkedList<int> list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AssignFromTemporary) {
    dsa::CircularSinglyLinkedList<int> list;
    fill(list, {9, 9});
    list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CanBeStoredInStdVector) {
    std::vector<dsa::CircularSinglyLinkedList<int>> lists;
    for (int i = 0; i < 10; ++i) {
        dsa::CircularSinglyLinkedList<int> list;
        fill(list, {i, i + 1});
        lists.push_back(list);
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(HasContents(lists[i], {i, i + 1})) << "lists[" << i << "]";
    }
}

// ---------- Move (rule of five) ----------
// DISABLED_ until the move constructor and move assignment exist: without them
// std::move silently falls back to copying. Remove the prefix to enable them.
// A move takes over the source's nodes without allocating, and leaves the
// source empty and reusable.

TEST(CircularSinglyLinkedList, DISABLED_MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<dsa::CircularSinglyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<dsa::CircularSinglyLinkedList<int>>::value);
}

TEST(CircularSinglyLinkedList, DISABLED_MoveConstructorTakesTheNodes) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    dsa::CircularSinglyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
    EXPECT_EQ(nodesOf(moved), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularSinglyLinkedList, DISABLED_MoveConstructorLeavesSourceEmpty) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, DISABLED_MoveConstructorOfEmptyList) {
    dsa::CircularSinglyLinkedList<int> source;
    dsa::CircularSinglyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, DISABLED_MovedFromListIsReusable) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularSinglyLinkedList<int> moved(std::move(source));
    source.add(4);
    source.addFirst(3);
    EXPECT_TRUE(HasContents(source, {3, 4}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DISABLED_MoveAssignmentTakesTheNodes) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    dsa::CircularSinglyLinkedList<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_EQ(nodesOf(target), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularSinglyLinkedList, DISABLED_MoveAssignmentReplacesExistingContents) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularSinglyLinkedList, DISABLED_MoveAssignmentLeavesSourceEmpty) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, DISABLED_MoveAssignmentFromEmptyListEmptiesTarget) {
    dsa::CircularSinglyLinkedList<int> source;
    dsa::CircularSinglyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, DISABLED_MoveAssignmentReturnsTarget) {
    dsa::CircularSinglyLinkedList<int> source;
    fill(source, {1});
    dsa::CircularSinglyLinkedList<int> target;
    EXPECT_EQ(&(target = std::move(source)), &target);
}
