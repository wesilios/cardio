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

void fill(ds::CircularSinglyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, and that tail->next points back to head.
::testing::AssertionResult HasContents(ds::CircularSinglyLinkedList<int> &list, const std::vector<int> &expected) {
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
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, AddAppendsInOrder) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddLastAppends) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    ds::CircularSinglyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, AddFirstPrepends) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    ds::CircularSinglyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, AddFirstAndAddLastMixed) {
    ds::CircularSinglyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(CircularSinglyLinkedList, DeleteFirstRemovesHead) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteLastRemovesTail) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteLastOnTwoNodes) {
    // The walk to the node before tail takes zero steps here.
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, DeleteLastRepeatedlyKeepsRing) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
    list.addLast(6);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 6}));
}

TEST(CircularSinglyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    ds::CircularSinglyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteLastOnEmptyListIsSafe) {
    ds::CircularSinglyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(CircularSinglyLinkedList, DeleteFirstThenAddFirst) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteLastThenAddLast) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(CircularSinglyLinkedList, DeleteNodeRemovesMiddleValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesHeadValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesTailValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeIgnoresMissingValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    ds::CircularSinglyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesEveryMatch) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesValueJustBeforeTail) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesSecondNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeRemovesMatchesAtBothEndsAndMiddle) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {5, 5, 1, 5, 5, 2, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeLeavesOnlyTwoNodes) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 5, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(CircularSinglyLinkedList, AddAtIndexZeroOnEmptyList) {
    ds::CircularSinglyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, AddAtIndexZeroPrepends) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddAtIndexOneInsertsAfterHead) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddAtIndexInsertsInMiddle) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularSinglyLinkedList, AddAtIndexBeforeTail) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularSinglyLinkedList, AddAtIndexSizeAppends) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsNegativeIndex) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsIndexPastSize) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularSinglyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularSinglyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), ds::InvalidArgument);
}

TEST(CircularSinglyLinkedList, FindByIndexRejectsIndexPastEnd) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), ds::InvalidArgument);
}

TEST(CircularSinglyLinkedList, FindByIndexOnEmptyListThrows) {
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), ds::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(CircularSinglyLinkedList, ClearEmptiesListAndAllowsReuse) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(CircularSinglyLinkedList, ClearOnEmptyListIsSafe) {
    ds::CircularSinglyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, ClearOnSingleNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, ClearTwiceIsSafe) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(CircularSinglyLinkedList, DestructorOnEmptyList) {
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularSinglyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

// ---------- Iterator ----------
// The tail links back to the head, so end() must not rely on reaching nullptr.
// Each range-for stops after 10 values so a broken end() fails the test instead of looping forever.

TEST(CircularSinglyLinkedList, RangeForVisitsEveryValueOnce) {
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
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
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const ds::CircularSinglyLinkedList<int> &constList = list;
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
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(CircularSinglyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(CircularSinglyLinkedList, DereferenceReturnsCurrentValue) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(CircularSinglyLinkedList, DereferenceOnConstIterator) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    const auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(CircularSinglyLinkedList, PrefixIncrementMovesToNextNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(CircularSinglyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(CircularSinglyLinkedList, IncrementPastTailReachesEnd) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(CircularSinglyLinkedList, IncrementPastSingleNodeReachesEnd) {
    // The node's next is itself, so the iterator must still count it as one lap.
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    auto it = list.begin();
    ++it;
    EXPECT_TRUE(it == list.end());
}

// ---------- Printing ----------

TEST(CircularSinglyLinkedList, ToStringListsValues) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(CircularSinglyLinkedList, StreamOperatorPrintsToString) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}

TEST(CircularSinglyLinkedList, ToStringOnEmptyList) {
    ds::CircularSinglyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(CircularSinglyLinkedList, ToStringOnSingleNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(CircularSinglyLinkedList, ToStringAfterDeletes) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}

// ---------- Copy (rule of three) ----------
// A copy owns its own nodes (deep copy): changing one list never changes the
// other. Double frees and leaks here only show up reliably in the sanitizer build.

namespace {

std::vector<const ds::SinglyNode<int> *> nodesOf(const ds::CircularSinglyLinkedList<int> &list) {
    std::vector<const ds::SinglyNode<int> *> nodes;
    auto *node = list.getHead();
    for (std::size_t i = 0; node != nullptr && i < list.size(); ++i) {
        nodes.push_back(node);
        node = node->getNext();
    }
    return nodes;
}

ds::CircularSinglyLinkedList<int> makeList(const std::vector<int> &values) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, values);
    return list;
}

} // namespace

TEST(CircularSinglyLinkedList, IsCopyConstructibleAndAssignable) {
    EXPECT_TRUE(std::is_copy_constructible<ds::CircularSinglyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<ds::CircularSinglyLinkedList<int>>::value);
}

// Copy constructor

TEST(CircularSinglyLinkedList, CopyConstructorCopiesValues) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyConstructorOfEmptyList) {
    ds::CircularSinglyLinkedList<int> original;
    ds::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {}));
    EXPECT_TRUE(HasContents(original, {}));
}

TEST(CircularSinglyLinkedList, CopyConstructorOfSingleNode) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {7});
    ds::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {7}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

TEST(CircularSinglyLinkedList, CopyConstructorKeepsDuplicates) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {2, 2, 1, 2});
    ds::CircularSinglyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {2, 2, 1, 2}));
}

TEST(CircularSinglyLinkedList, CopyConstructorAllocatesNewNodes) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> copy(original);
    const auto originalNodes = nodesOf(original);
    for (const auto *node : nodesOf(copy)) {
        for (const auto *shared : originalNodes) {
            EXPECT_NE(node, shared) << "the copy shares a node with the original";
        }
    }
}

TEST(CircularSinglyLinkedList, ChangingCopyLeavesOriginalUnchanged) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> copy(original);
    copy.deleteFirst();
    copy.deleteLast();
    copy.addLast(9);
    copy.addFirst(0);
    EXPECT_TRUE(HasContents(copy, {0, 2, 9}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, ChangingOriginalLeavesCopyUnchanged) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> copy(original);
    original.clear();
    original.add(5);
    EXPECT_TRUE(HasContents(original, {5}));
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyOutlivesOriginal) {
    auto *original = new ds::CircularSinglyLinkedList<int>();
    fill(*original, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> copy(*original);
    delete original;
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyInitializationWithEquals) {
    ds::CircularSinglyLinkedList<int> original;
    fill(original, {1, 2});
    ds::CircularSinglyLinkedList<int> copy = original;
    EXPECT_TRUE(HasContents(copy, {1, 2}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

// Copy assignment

TEST(CircularSinglyLinkedList, CopyAssignmentIntoEmptyList) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentReplacesLongerList) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9, 10});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentReplacesShorterList) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3, 4});
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {9});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3, 4}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentFromEmptyListEmptiesTarget) {
    ds::CircularSinglyLinkedList<int> source;
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentThenAddUsesNewTail) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = source;
    target.add(3);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentAllocatesNewNodes) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> target;
    target = source;
    const auto sourceNodes = nodesOf(source);
    for (const auto *node : nodesOf(target)) {
        for (const auto *shared : sourceNodes) {
            EXPECT_NE(node, shared) << "the target shares a node with the source";
        }
    }
}

TEST(CircularSinglyLinkedList, ChangingAssignedListLeavesSourceUnchanged) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> target;
    target = source;
    target.deleteNode(2);
    EXPECT_TRUE(HasContents(target, {1, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, SelfCopyAssignmentKeepsContents) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> &alias = list;
    list = alias;
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CopyAssignmentReturnsTarget) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1});
    ds::CircularSinglyLinkedList<int> target;
    EXPECT_EQ(&(target = source), &target);
}

TEST(CircularSinglyLinkedList, ChainedCopyAssignment) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularSinglyLinkedList<int> first;
    ds::CircularSinglyLinkedList<int> second;
    fill(second, {9, 9, 9});
    second = first = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
    EXPECT_NE(first.getHead(), second.getHead());
}

TEST(CircularSinglyLinkedList, ReturnByValue) {
    ds::CircularSinglyLinkedList<int> list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, AssignFromTemporary) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {9, 9});
    list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, CanBeStoredInStdVector) {
    std::vector<ds::CircularSinglyLinkedList<int>> lists;
    for (int i = 0; i < 10; ++i) {
        ds::CircularSinglyLinkedList<int> list;
        fill(list, {i, i + 1});
        lists.push_back(list);
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(HasContents(lists[i], {i, i + 1})) << "lists[" << i << "]";
    }
}

// ---------- Move (rule of five) ----------
// A move takes over the source's nodes without allocating, and leaves the
// source empty and reusable.

TEST(CircularSinglyLinkedList, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::CircularSinglyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::CircularSinglyLinkedList<int>>::value);
}

TEST(CircularSinglyLinkedList, MoveConstructorTakesTheNodes) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    ds::CircularSinglyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
    EXPECT_EQ(nodesOf(moved), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularSinglyLinkedList, MoveConstructorLeavesSourceEmpty) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, MoveConstructorOfEmptyList) {
    ds::CircularSinglyLinkedList<int> source;
    ds::CircularSinglyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, MovedFromListIsReusable) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> moved(std::move(source));
    source.add(4);
    source.addFirst(3);
    EXPECT_TRUE(HasContents(source, {3, 4}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, MoveAssignmentTakesTheNodes) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    ds::CircularSinglyLinkedList<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_EQ(nodesOf(target), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularSinglyLinkedList, MoveAssignmentReplacesExistingContents) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularSinglyLinkedList, MoveAssignmentLeavesSourceEmpty) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, MoveAssignmentFromEmptyListEmptiesTarget) {
    ds::CircularSinglyLinkedList<int> source;
    ds::CircularSinglyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularSinglyLinkedList, MoveAssignmentReturnsTarget) {
    ds::CircularSinglyLinkedList<int> source;
    fill(source, {1});
    ds::CircularSinglyLinkedList<int> target;
    EXPECT_EQ(&(target = std::move(source)), &target);
}

// Self-move goes through a reference so the compiler doesn't warn about
// `list = std::move(list)`. A leak or double free is reported by ASan when
// the list is destroyed at the end of the test.

TEST(CircularSinglyLinkedList, SelfMoveAssignmentKeepsContents) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularSinglyLinkedList, SelfMoveAssignmentKeepsTheNodes) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const auto nodes = nodesOf(list);
    ds::CircularSinglyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_EQ(nodesOf(list), nodes);
}

TEST(CircularSinglyLinkedList, SelfMoveAssignmentOnEmptyList) {
    ds::CircularSinglyLinkedList<int> list;
    ds::CircularSinglyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularSinglyLinkedList, SelfMoveAssignmentOnSingleNode) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {7});
    ds::CircularSinglyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularSinglyLinkedList, SelfMoveAssignmentThenModify) {
    ds::CircularSinglyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::CircularSinglyLinkedList<int> &alias = list;
    list = std::move(alias);
    list.addFirst(0);
    list.addLast(4);
    list.deleteNode(2);
    EXPECT_TRUE(HasContents(list, {0, 1, 3, 4}));
}
