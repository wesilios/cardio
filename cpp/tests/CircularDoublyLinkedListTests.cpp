#include "DataStructures/LinkedList/CircularDoublyLinkedList.h"
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

// ---------- Iterator ----------
// The tail links back to the head, so end() must not rely on reaching nullptr.
// Each range-for stops after 10 values so a broken end() fails the test instead of looping forever.

TEST(CircularDoublyLinkedList, RangeForVisitsEveryValueOnce) {
    dsa::CircularDoublyLinkedList<int> list;
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

TEST(CircularDoublyLinkedList, RangeForOnEmptyListVisitsNothing) {
    dsa::CircularDoublyLinkedList<int> list;
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_TRUE(visited.empty());
}

TEST(CircularDoublyLinkedList, RangeForOnSingleNodeVisitsItOnce) {
    // The single node links to itself.
    dsa::CircularDoublyLinkedList<int> list;
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

TEST(CircularDoublyLinkedList, RangeForSeesChangesToTheList) {
    dsa::CircularDoublyLinkedList<int> list;
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

TEST(CircularDoublyLinkedList, RangeForTwiceGivesSameValues) {
    dsa::CircularDoublyLinkedList<int> list;
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

TEST(CircularDoublyLinkedList, RangeForOverConstList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const dsa::CircularDoublyLinkedList<int> &constList = list;
    std::vector<int> visited;
    for (int value : constList) {
        visited.push_back(value);
        if (visited.size() > 10) {
            break;
        }
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(CircularDoublyLinkedList, BeginEqualsEndOnEmptyList) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(CircularDoublyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(CircularDoublyLinkedList, DereferenceReturnsCurrentValue) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(CircularDoublyLinkedList, PrefixIncrementMovesToNextNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(CircularDoublyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(CircularDoublyLinkedList, IncrementPastTailReachesEnd) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(CircularDoublyLinkedList, IncrementPastSingleNodeReachesEnd) {
    // The node's next is itself, so the iterator must still count it as one lap.
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    auto it = list.begin();
    ++it;
    EXPECT_TRUE(it == list.end());
}

// ---------- Printing ----------

TEST(CircularDoublyLinkedList, ToStringListsValues) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(CircularDoublyLinkedList, StreamOperatorPrintsToString) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}

TEST(CircularDoublyLinkedList, ToStringOnEmptyList) {
    dsa::CircularDoublyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(CircularDoublyLinkedList, ToStringOnSingleNode) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(CircularDoublyLinkedList, ToStringAfterDeletes) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}

// ---------- Copy (rule of three) ----------
// A copy owns its own nodes (deep copy): changing one list never changes the
// other. Double frees and leaks here only show up reliably in the sanitizer build.

namespace {

std::vector<const dsa::DoublyNode<int> *> nodesOf(const dsa::CircularDoublyLinkedList<int> &list) {
    std::vector<const dsa::DoublyNode<int> *> nodes;
    auto *node = list.getHead();
    for (std::size_t i = 0; node != nullptr && i < list.size(); ++i) {
        nodes.push_back(node);
        node = node->getNext();
    }
    return nodes;
}

dsa::CircularDoublyLinkedList<int> makeList(const std::vector<int> &values) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, values);
    return list;
}

} // namespace

TEST(CircularDoublyLinkedList, IsCopyConstructibleAndAssignable) {
    EXPECT_TRUE(std::is_copy_constructible<dsa::CircularDoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<dsa::CircularDoublyLinkedList<int>>::value);
}

// Copy constructor

TEST(CircularDoublyLinkedList, CopyConstructorCopiesValues) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyConstructorOfEmptyList) {
    dsa::CircularDoublyLinkedList<int> original;
    dsa::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {}));
    EXPECT_TRUE(HasContents(original, {}));
}

TEST(CircularDoublyLinkedList, CopyConstructorOfSingleNode) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {7});
    dsa::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {7}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

TEST(CircularDoublyLinkedList, CopyConstructorKeepsDuplicates) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {2, 2, 1, 2});
    dsa::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {2, 2, 1, 2}));
}

TEST(CircularDoublyLinkedList, CopyConstructorAllocatesNewNodes) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> copy(original);
    const auto originalNodes = nodesOf(original);
    for (const auto *node : nodesOf(copy)) {
        for (const auto *shared : originalNodes) {
            EXPECT_NE(node, shared) << "the copy shares a node with the original";
        }
    }
}

TEST(CircularDoublyLinkedList, ChangingCopyLeavesOriginalUnchanged) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> copy(original);
    copy.deleteFirst();
    copy.deleteLast();
    copy.addLast(9);
    copy.addFirst(0);
    EXPECT_TRUE(HasContents(copy, {0, 2, 9}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, ChangingOriginalLeavesCopyUnchanged) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> copy(original);
    original.clear();
    original.add(5);
    EXPECT_TRUE(HasContents(original, {5}));
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyOutlivesOriginal) {
    auto *original = new dsa::CircularDoublyLinkedList<int>();
    fill(*original, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> copy(*original);
    delete original;
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyInitializationWithEquals) {
    dsa::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2});
    dsa::CircularDoublyLinkedList<int> copy = original;
    EXPECT_TRUE(HasContents(copy, {1, 2}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

// Copy assignment

TEST(CircularDoublyLinkedList, CopyAssignmentIntoEmptyList) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentReplacesLongerList) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9, 10});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentReplacesShorterList) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3, 4});
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {9});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3, 4}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentFromEmptyListEmptiesTarget) {
    dsa::CircularDoublyLinkedList<int> source;
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentThenAddUsesNewTail) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = source;
    target.add(3);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentAllocatesNewNodes) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> target;
    target = source;
    const auto sourceNodes = nodesOf(source);
    for (const auto *node : nodesOf(target)) {
        for (const auto *shared : sourceNodes) {
            EXPECT_NE(node, shared) << "the target shares a node with the source";
        }
    }
}

TEST(CircularDoublyLinkedList, ChangingAssignedListLeavesSourceUnchanged) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> target;
    target = source;
    target.deleteNode(2);
    EXPECT_TRUE(HasContents(target, {1, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, SelfCopyAssignmentKeepsContents) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> &alias = list;
    list = alias;
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentReturnsTarget) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1});
    dsa::CircularDoublyLinkedList<int> target;
    EXPECT_EQ(&(target = source), &target);
}

TEST(CircularDoublyLinkedList, ChainedCopyAssignment) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularDoublyLinkedList<int> first;
    dsa::CircularDoublyLinkedList<int> second;
    fill(second, {9, 9, 9});
    second = first = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
    EXPECT_NE(first.getHead(), second.getHead());
}

TEST(CircularDoublyLinkedList, ReturnByValue) {
    dsa::CircularDoublyLinkedList<int> list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AssignFromTemporary) {
    dsa::CircularDoublyLinkedList<int> list;
    fill(list, {9, 9});
    list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CanBeStoredInStdVector) {
    std::vector<dsa::CircularDoublyLinkedList<int>> lists;
    for (int i = 0; i < 10; ++i) {
        dsa::CircularDoublyLinkedList<int> list;
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

TEST(CircularDoublyLinkedList, DISABLED_MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<dsa::CircularDoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<dsa::CircularDoublyLinkedList<int>>::value);
}

TEST(CircularDoublyLinkedList, DISABLED_MoveConstructorTakesTheNodes) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    dsa::CircularDoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
    EXPECT_EQ(nodesOf(moved), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularDoublyLinkedList, DISABLED_MoveConstructorLeavesSourceEmpty) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, DISABLED_MoveConstructorOfEmptyList) {
    dsa::CircularDoublyLinkedList<int> source;
    dsa::CircularDoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, DISABLED_MovedFromListIsReusable) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::CircularDoublyLinkedList<int> moved(std::move(source));
    source.add(4);
    source.addFirst(3);
    EXPECT_TRUE(HasContents(source, {3, 4}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DISABLED_MoveAssignmentTakesTheNodes) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    dsa::CircularDoublyLinkedList<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_EQ(nodesOf(target), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularDoublyLinkedList, DISABLED_MoveAssignmentReplacesExistingContents) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularDoublyLinkedList, DISABLED_MoveAssignmentLeavesSourceEmpty) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, DISABLED_MoveAssignmentFromEmptyListEmptiesTarget) {
    dsa::CircularDoublyLinkedList<int> source;
    dsa::CircularDoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, DISABLED_MoveAssignmentReturnsTarget) {
    dsa::CircularDoublyLinkedList<int> source;
    fill(source, {1});
    dsa::CircularDoublyLinkedList<int> target;
    EXPECT_EQ(&(target = std::move(source)), &target);
}
