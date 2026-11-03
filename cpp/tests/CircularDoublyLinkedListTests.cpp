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

void fill(ds::CircularDoublyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, that tail->next is head and head->previous is tail,
// and that the previous links give the same values walking back from the tail.
::testing::AssertionResult HasContents(ds::CircularDoublyLinkedList<int> &list, const std::vector<int> &expected) {
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
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, AddAppendsInOrder) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddLastAppends) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    ds::CircularDoublyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, AddFirstPrepends) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    ds::CircularDoublyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, AddFirstAndAddLastMixed) {
    ds::CircularDoublyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(CircularDoublyLinkedList, DeleteFirstRemovesHead) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteLastRemovesTail) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    ds::CircularDoublyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteLastOnEmptyListIsSafe) {
    ds::CircularDoublyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(CircularDoublyLinkedList, DeleteFirstThenAddFirst) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteLastThenAddLast) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(CircularDoublyLinkedList, DeleteNodeRemovesMiddleValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesHeadValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesTailValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    list.printList();
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeIgnoresMissingValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    ds::CircularDoublyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesEveryMatch) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularDoublyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularDoublyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(CircularDoublyLinkedList, AddAtIndexZeroOnEmptyList) {
    ds::CircularDoublyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, AddAtIndexZeroPrepends) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddAtIndexOneInsertsAfterHead) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddAtIndexInsertsInMiddle) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularDoublyLinkedList, AddAtIndexBeforeTail) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(CircularDoublyLinkedList, AddAtIndexSizeAppends) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsNegativeIndex) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsIndexPastSize) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(CircularDoublyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(CircularDoublyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), ds::InvalidArgument);
}

TEST(CircularDoublyLinkedList, FindByIndexRejectsIndexPastEnd) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), ds::InvalidArgument);
}

TEST(CircularDoublyLinkedList, FindByIndexOnEmptyListThrows) {
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), ds::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(CircularDoublyLinkedList, ClearEmptiesListAndAllowsReuse) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(CircularDoublyLinkedList, ClearOnEmptyListIsSafe) {
    ds::CircularDoublyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, ClearOnSingleNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, ClearTwiceIsSafe) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(CircularDoublyLinkedList, DestructorOnEmptyList) {
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularDoublyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

// ---------- Iterator ----------
// The tail links back to the head, so end() must not rely on reaching nullptr.
// Each range-for stops after 10 values so a broken end() fails the test instead of looping forever.

TEST(CircularDoublyLinkedList, RangeForVisitsEveryValueOnce) {
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
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
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const ds::CircularDoublyLinkedList<int> &constList = list;
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
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(CircularDoublyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(CircularDoublyLinkedList, DereferenceReturnsCurrentValue) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(CircularDoublyLinkedList, DereferenceOnConstIterator) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    const auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(CircularDoublyLinkedList, PrefixIncrementMovesToNextNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(CircularDoublyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(CircularDoublyLinkedList, IncrementPastTailReachesEnd) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(CircularDoublyLinkedList, IncrementPastSingleNodeReachesEnd) {
    // The node's next is itself, so the iterator must still count it as one lap.
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    auto it = list.begin();
    ++it;
    EXPECT_TRUE(it == list.end());
}

// ---------- Printing ----------

TEST(CircularDoublyLinkedList, ToStringListsValues) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(CircularDoublyLinkedList, StreamOperatorPrintsToString) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}

TEST(CircularDoublyLinkedList, ToStringOnEmptyList) {
    ds::CircularDoublyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(CircularDoublyLinkedList, ToStringOnSingleNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(CircularDoublyLinkedList, ToStringAfterDeletes) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}

// ---------- Copy (rule of three) ----------
// A copy owns its own nodes (deep copy): changing one list never changes the
// other. Double frees and leaks here only show up reliably in the sanitizer build.

namespace {

std::vector<const ds::DoublyNode<int> *> nodesOf(const ds::CircularDoublyLinkedList<int> &list) {
    std::vector<const ds::DoublyNode<int> *> nodes;
    auto *node = list.getHead();
    for (std::size_t i = 0; node != nullptr && i < list.size(); ++i) {
        nodes.push_back(node);
        node = node->getNext();
    }
    return nodes;
}

ds::CircularDoublyLinkedList<int> makeList(const std::vector<int> &values) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, values);
    return list;
}

} // namespace

TEST(CircularDoublyLinkedList, IsCopyConstructibleAndAssignable) {
    EXPECT_TRUE(std::is_copy_constructible<ds::CircularDoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<ds::CircularDoublyLinkedList<int>>::value);
}

// Copy constructor

TEST(CircularDoublyLinkedList, CopyConstructorCopiesValues) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyConstructorOfEmptyList) {
    ds::CircularDoublyLinkedList<int> original;
    ds::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {}));
    EXPECT_TRUE(HasContents(original, {}));
}

TEST(CircularDoublyLinkedList, CopyConstructorOfSingleNode) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {7});
    ds::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {7}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

TEST(CircularDoublyLinkedList, CopyConstructorKeepsDuplicates) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {2, 2, 1, 2});
    ds::CircularDoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {2, 2, 1, 2}));
}

TEST(CircularDoublyLinkedList, CopyConstructorAllocatesNewNodes) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> copy(original);
    const auto originalNodes = nodesOf(original);
    for (const auto *node : nodesOf(copy)) {
        for (const auto *shared : originalNodes) {
            EXPECT_NE(node, shared) << "the copy shares a node with the original";
        }
    }
}

TEST(CircularDoublyLinkedList, ChangingCopyLeavesOriginalUnchanged) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> copy(original);
    copy.deleteFirst();
    copy.deleteLast();
    copy.addLast(9);
    copy.addFirst(0);
    EXPECT_TRUE(HasContents(copy, {0, 2, 9}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, ChangingOriginalLeavesCopyUnchanged) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> copy(original);
    original.clear();
    original.add(5);
    EXPECT_TRUE(HasContents(original, {5}));
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyOutlivesOriginal) {
    auto *original = new ds::CircularDoublyLinkedList<int>();
    fill(*original, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> copy(*original);
    delete original;
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyInitializationWithEquals) {
    ds::CircularDoublyLinkedList<int> original;
    fill(original, {1, 2});
    ds::CircularDoublyLinkedList<int> copy = original;
    EXPECT_TRUE(HasContents(copy, {1, 2}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

// Copy assignment

TEST(CircularDoublyLinkedList, CopyAssignmentIntoEmptyList) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentReplacesLongerList) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9, 10});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentReplacesShorterList) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3, 4});
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {9});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3, 4}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentFromEmptyListEmptiesTarget) {
    ds::CircularDoublyLinkedList<int> source;
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentThenAddUsesNewTail) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = source;
    target.add(3);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentAllocatesNewNodes) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> target;
    target = source;
    const auto sourceNodes = nodesOf(source);
    for (const auto *node : nodesOf(target)) {
        for (const auto *shared : sourceNodes) {
            EXPECT_NE(node, shared) << "the target shares a node with the source";
        }
    }
}

TEST(CircularDoublyLinkedList, ChangingAssignedListLeavesSourceUnchanged) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> target;
    target = source;
    target.deleteNode(2);
    EXPECT_TRUE(HasContents(target, {1, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, SelfCopyAssignmentKeepsContents) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> &alias = list;
    list = alias;
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CopyAssignmentReturnsTarget) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1});
    ds::CircularDoublyLinkedList<int> target;
    EXPECT_EQ(&(target = source), &target);
}

TEST(CircularDoublyLinkedList, ChainedCopyAssignment) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularDoublyLinkedList<int> first;
    ds::CircularDoublyLinkedList<int> second;
    fill(second, {9, 9, 9});
    second = first = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
    EXPECT_NE(first.getHead(), second.getHead());
}

TEST(CircularDoublyLinkedList, ReturnByValue) {
    ds::CircularDoublyLinkedList<int> list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, AssignFromTemporary) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {9, 9});
    list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, CanBeStoredInStdVector) {
    std::vector<ds::CircularDoublyLinkedList<int>> lists;
    for (int i = 0; i < 10; ++i) {
        ds::CircularDoublyLinkedList<int> list;
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

TEST(CircularDoublyLinkedList, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::CircularDoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::CircularDoublyLinkedList<int>>::value);
}

TEST(CircularDoublyLinkedList, MoveConstructorTakesTheNodes) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    ds::CircularDoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
    EXPECT_EQ(nodesOf(moved), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularDoublyLinkedList, MoveConstructorLeavesSourceEmpty) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, MoveConstructorOfEmptyList) {
    ds::CircularDoublyLinkedList<int> source;
    ds::CircularDoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, MovedFromListIsReusable) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> moved(std::move(source));
    source.add(4);
    source.addFirst(3);
    EXPECT_TRUE(HasContents(source, {3, 4}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, MoveAssignmentTakesTheNodes) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    ds::CircularDoublyLinkedList<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_EQ(nodesOf(target), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(CircularDoublyLinkedList, MoveAssignmentReplacesExistingContents) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(CircularDoublyLinkedList, MoveAssignmentLeavesSourceEmpty) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, MoveAssignmentFromEmptyListEmptiesTarget) {
    ds::CircularDoublyLinkedList<int> source;
    ds::CircularDoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(CircularDoublyLinkedList, MoveAssignmentReturnsTarget) {
    ds::CircularDoublyLinkedList<int> source;
    fill(source, {1});
    ds::CircularDoublyLinkedList<int> target;
    EXPECT_EQ(&(target = std::move(source)), &target);
}

// Self-move goes through a reference so the compiler doesn't warn about
// `list = std::move(list)`. A leak or double free is reported by ASan when
// the list is destroyed at the end of the test.

TEST(CircularDoublyLinkedList, SelfMoveAssignmentKeepsContents) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(CircularDoublyLinkedList, SelfMoveAssignmentKeepsTheNodes) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const auto nodes = nodesOf(list);
    ds::CircularDoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_EQ(nodesOf(list), nodes);
}

TEST(CircularDoublyLinkedList, SelfMoveAssignmentOnEmptyList) {
    ds::CircularDoublyLinkedList<int> list;
    ds::CircularDoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(CircularDoublyLinkedList, SelfMoveAssignmentOnSingleNode) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {7});
    ds::CircularDoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(CircularDoublyLinkedList, SelfMoveAssignmentThenModify) {
    ds::CircularDoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::CircularDoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    list.addFirst(0);
    list.addLast(4);
    list.deleteNode(2);
    EXPECT_TRUE(HasContents(list, {0, 1, 3, 4}));
}
