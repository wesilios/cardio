#include "DataStructures/LinkedList/DoublyLinkedList.h"
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

void fill(ds::DoublyLinkedList<int> &list, const std::vector<int> &values) {
    for (int value : values) {
        list.add(value);
    }
}

// Checks size, contents, head/tail, that tail->next and head->previous are nullptr,
// and that the previous links give the same values walking back from the tail.
::testing::AssertionResult HasContents(ds::DoublyLinkedList<int> &list, const std::vector<int> &expected) {
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
        return ::testing::AssertionFailure()
               << "contents are " << describe(forward) << ", expected " << describe(expected);
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
    ds::DoublyLinkedList<int> list;
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, AddAppendsInOrder) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddLastAppends) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addLast(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddLastOnEmptyListSetsHeadAndTail) {
    ds::DoublyLinkedList<int> list;
    list.addLast(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, AddFirstPrepends) {
    ds::DoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddFirstOnEmptyListSetsHeadAndTail) {
    ds::DoublyLinkedList<int> list;
    list.addFirst(7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, AddFirstAndAddLastMixed) {
    ds::DoublyLinkedList<int> list;
    list.addFirst(2);
    list.addLast(3);
    list.addFirst(1);
    list.addLast(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4}));
}

// ---------- Deleting first / last ----------

TEST(DoublyLinkedList, DeleteFirstRemovesHead) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(DoublyLinkedList, DeleteLastRemovesTail) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteFirstOnSingleNodeEmptiesList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteLastOnSingleNodeEmptiesList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1});
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteFirstOnEmptyListIsSafe) {
    ds::DoublyLinkedList<int> list;
    list.deleteFirst();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteLastOnEmptyListIsSafe) {
    ds::DoublyLinkedList<int> list;
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteFirstUntilEmptyThenReuse) {
    ds::DoublyLinkedList<int> list;
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
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.deleteLast();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4, 5});
    EXPECT_TRUE(HasContents(list, {4, 5}));
}

TEST(DoublyLinkedList, DeleteFirstThenAddFirst) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteFirst();
    list.addFirst(9);
    EXPECT_TRUE(HasContents(list, {9, 2, 3}));
}

TEST(DoublyLinkedList, DeleteLastThenAddLast) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteLast();
    list.addLast(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 9}));
}

// ---------- Deleting by value ----------
// deleteNode removes every node holding the value.

TEST(DoublyLinkedList, DeleteNodeRemovesMiddleValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(3);
    EXPECT_TRUE(HasContents(list, {1, 2, 4}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesHeadValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(1);
    EXPECT_TRUE(HasContents(list, {2, 3, 4}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesTailValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNode(4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeIgnoresMissingValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNode(9);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeOnEmptyListIsSafe) {
    ds::DoublyLinkedList<int> list;
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeOnSingleNodeEmptiesList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeOnSingleNodeKeepsOtherValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNode(99);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesEveryMatch) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 5, 2, 5, 3});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesAdjacentMatches) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 5, 5, 2});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesRepeatedHeadValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {5, 5, 1});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(DoublyLinkedList, DeleteNodeRemovesRepeatedTailValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(DoublyLinkedList, DeleteNodeOnTwoMatchingNodesEmptiesList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeWhenEveryValueMatchesEmptiesList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {5, 5, 5, 5});
    list.deleteNode(5);
    EXPECT_TRUE(HasContents(list, {}));
}

// ---------- Index functions ----------
// Indexes start at 0. addAtIndex accepts 0..size() (size() appends);
// deleteNodeAtIndex and findByIndex accept 0..size()-1.

TEST(DoublyLinkedList, AddAtIndexZeroOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    list.addAtIndex(0, 7);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, AddAtIndexZeroPrepends) {
    ds::DoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addAtIndex(0, 1);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddAtIndexOneInsertsAfterHead) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddAtIndexInsertsInMiddle) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 4, 5});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(DoublyLinkedList, AddAtIndexBeforeTail) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 5});
    list.addAtIndex(3, 4);
    EXPECT_TRUE(HasContents(list, {1, 2, 3, 4, 5}));
}

TEST(DoublyLinkedList, AddAtIndexSizeAppends) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.addAtIndex(2, 3);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsNegativeIndex) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(-1, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsIndexPastSize) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.addAtIndex(3, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    EXPECT_THROW(list.addAtIndex(1, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, AddAtIndexRejectsIndexPastSizeOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_THROW(list.addAtIndex(2, 9), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1}));
}

TEST(DoublyLinkedList, AddAtIndexSizeAppendsOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1});
    list.addAtIndex(1, 2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexZeroRemovesHead) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexOneRemovesSecondNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_TRUE(HasContents(list, {1, 3, 4}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexRemovesMiddleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2, 4, 5}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexLastRemovesTail) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.deleteNodeAtIndex(2);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexOnSingleNodeEmptiesList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {7});
    list.deleteNodeAtIndex(0);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexRejectsNegativeIndex) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(-1), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexRejectsIndexPastEnd) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.deleteNodeAtIndex(2), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {1, 2}));
}

TEST(DoublyLinkedList, DeleteNodeAtIndexOnEmptyListThrows) {
    ds::DoublyLinkedList<int> list;
    EXPECT_THROW(list.deleteNodeAtIndex(0), ds::InvalidArgument);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, FindByIndexPrintsValueAtEveryIndex) {
    ds::DoublyLinkedList<int> list;
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
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(-1), ds::InvalidArgument);
}

TEST(DoublyLinkedList, FindByIndexRejectsIndexPastEnd) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    EXPECT_THROW(list.findByIndex(2), ds::InvalidArgument);
}

TEST(DoublyLinkedList, FindByIndexOnEmptyListThrows) {
    ds::DoublyLinkedList<int> list;
    EXPECT_THROW(list.findByIndex(0), ds::InvalidArgument);
}

// ---------- Clear / destruction ----------
// Memory errors here (double free, use after free, leaks) are only reported
// reliably in the sanitizer build, which is what CI runs.

TEST(DoublyLinkedList, ClearEmptiesListAndAllowsReuse) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));

    fill(list, {4});
    EXPECT_TRUE(HasContents(list, {4}));
}

TEST(DoublyLinkedList, ClearOnEmptyListIsSafe) {
    ds::DoublyLinkedList<int> list;
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, ClearOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1});
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, ClearTwiceIsSafe) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.clear();
    list.clear();
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, DestructorFreesEveryNode) {
    // The list is destroyed at the end of this scope.
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4, 5});
    EXPECT_EQ(list.size(), 5);
}

TEST(DoublyLinkedList, DestructorOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
}

TEST(DoublyLinkedList, DestructorAfterDeletesFreesRemainingNodes) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_TRUE(HasContents(list, {2, 3}));
}

// ---------- Iterator ----------

TEST(DoublyLinkedList, RangeForVisitsEveryValueInOrder) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(DoublyLinkedList, RangeForOnEmptyListVisitsNothing) {
    ds::DoublyLinkedList<int> list;
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_TRUE(visited.empty());
}

TEST(DoublyLinkedList, RangeForOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {7});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({7}));
}

TEST(DoublyLinkedList, RangeForSeesChangesToTheList) {
    ds::DoublyLinkedList<int> list;
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

TEST(DoublyLinkedList, RangeForOverConstList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const ds::DoublyLinkedList<int> &constList = list;
    std::vector<int> visited;
    for (int value : constList) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(DoublyLinkedList, BeginEqualsEndOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(DoublyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(DoublyLinkedList, DereferenceReturnsCurrentValue) {
    ds::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(DoublyLinkedList, DereferenceOnConstIterator) {
    ds::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    const auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(DoublyLinkedList, PrefixIncrementMovesToNextNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(DoublyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    ds::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(DoublyLinkedList, IncrementPastLastNodeReachesEnd) {
    ds::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(DoublyLinkedList, IncrementAtEndStaysAtEnd) {
    ds::DoublyLinkedList<int> list;
    auto it = list.end();
    ++it;
    EXPECT_TRUE(it == list.end());
}

// ---------- Printing ----------

TEST(DoublyLinkedList, ToStringListsValues) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(DoublyLinkedList, StreamOperatorPrintsToString) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::stringstream os;
    os << list;
    EXPECT_EQ(os.str(), "[ 1, 2, 3 ]");
}

TEST(DoublyLinkedList, ToStringOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(DoublyLinkedList, ToStringOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(DoublyLinkedList, ToStringAfterDeletes) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}

// toStringInReverse walks from tail through the previous links, so these also
// catch an operation that leaves a previous link stale.

TEST(DoublyLinkedList, ToStringInReverseListsValuesTailFirst) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    EXPECT_EQ(list.toStringInReverse(), "[ 3, 2, 1 ]");
}

TEST(DoublyLinkedList, ToStringInReverseOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    EXPECT_EQ(list.toStringInReverse(), "[  ]");
}

TEST(DoublyLinkedList, ToStringInReverseOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toStringInReverse(), "[ 7 ]");
}

TEST(DoublyLinkedList, ToStringInReverseKeepsDuplicates) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 2, 3});
    EXPECT_EQ(list.toStringInReverse(), "[ 3, 2, 2, 1 ]");
}

TEST(DoublyLinkedList, ToStringInReverseDoesNotChangeList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.toStringInReverse();
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
    EXPECT_EQ(list.toString(), "[ 1, 2, 3 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterAddFirst) {
    ds::DoublyLinkedList<int> list;
    fill(list, {2, 3});
    list.addFirst(1);
    EXPECT_EQ(list.toStringInReverse(), "[ 3, 2, 1 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterAddAtIndex) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 3});
    list.addAtIndex(1, 2);
    list.addAtIndex(0, 0);
    list.addAtIndex(4, 4);
    EXPECT_EQ(list.toStringInReverse(), "[ 4, 3, 2, 1, 0 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterDeletes) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toStringInReverse(), "[ 3, 2 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterDeleteNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {5, 1, 5, 2, 5});
    list.deleteNode(5);
    EXPECT_EQ(list.toStringInReverse(), "[ 2, 1 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterDeleteNodeAtIndex) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteNodeAtIndex(1);
    EXPECT_EQ(list.toStringInReverse(), "[ 4, 3, 1 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterDeletingDownToOneNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    list.deleteFirst();
    EXPECT_EQ(list.toStringInReverse(), "[ 2 ]");
}

TEST(DoublyLinkedList, ToStringInReverseAfterClear) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    list.clear();
    EXPECT_EQ(list.toStringInReverse(), "[  ]");
    list.add(9);
    EXPECT_EQ(list.toStringInReverse(), "[ 9 ]");
}

TEST(DoublyLinkedList, ToStringInReverseOnConstList) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2});
    const ds::DoublyLinkedList<int> &constList = list;
    EXPECT_EQ(constList.toStringInReverse(), "[ 2, 1 ]");
}

TEST(DoublyLinkedList, ToStringInReverseOfStrings) {
    ds::DoublyLinkedList<std::string> list;
    list.add("a");
    list.add("b");
    list.add("c");
    EXPECT_EQ(list.toStringInReverse(), "[ c, b, a ]");
}

// ---------- Copy (rule of three) ----------
// A copy owns its own nodes (deep copy): changing one list never changes the
// other. Double frees and leaks here only show up reliably in the sanitizer build.

namespace {

std::vector<const ds::DoublyNode<int> *> nodesOf(const ds::DoublyLinkedList<int> &list) {
    std::vector<const ds::DoublyNode<int> *> nodes;
    auto *node = list.getHead();
    for (std::size_t i = 0; node != nullptr && i < list.size(); ++i) {
        nodes.push_back(node);
        node = node->getNext();
    }
    return nodes;
}

ds::DoublyLinkedList<int> makeList(const std::vector<int> &values) {
    ds::DoublyLinkedList<int> list;
    fill(list, values);
    return list;
}

} // namespace

TEST(DoublyLinkedList, IsCopyConstructibleAndAssignable) {
    EXPECT_TRUE(std::is_copy_constructible<ds::DoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<ds::DoublyLinkedList<int>>::value);
}

// Copy constructor

TEST(DoublyLinkedList, CopyConstructorCopiesValues) {
    ds::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyConstructorOfEmptyList) {
    ds::DoublyLinkedList<int> original;
    ds::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {}));
    EXPECT_TRUE(HasContents(original, {}));
}

TEST(DoublyLinkedList, CopyConstructorOfSingleNode) {
    ds::DoublyLinkedList<int> original;
    fill(original, {7});
    ds::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {7}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

TEST(DoublyLinkedList, CopyConstructorKeepsDuplicates) {
    ds::DoublyLinkedList<int> original;
    fill(original, {2, 2, 1, 2});
    ds::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {2, 2, 1, 2}));
}

TEST(DoublyLinkedList, CopyConstructorAllocatesNewNodes) {
    ds::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::DoublyLinkedList<int> copy(original);
    const auto originalNodes = nodesOf(original);
    for (const auto *node : nodesOf(copy)) {
        for (const auto *shared : originalNodes) {
            EXPECT_NE(node, shared) << "the copy shares a node with the original";
        }
    }
}

TEST(DoublyLinkedList, ChangingCopyLeavesOriginalUnchanged) {
    ds::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::DoublyLinkedList<int> copy(original);
    copy.deleteFirst();
    copy.deleteLast();
    copy.addLast(9);
    copy.addFirst(0);
    EXPECT_TRUE(HasContents(copy, {0, 2, 9}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(DoublyLinkedList, ChangingOriginalLeavesCopyUnchanged) {
    ds::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    ds::DoublyLinkedList<int> copy(original);
    original.clear();
    original.add(5);
    EXPECT_TRUE(HasContents(original, {5}));
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyOutlivesOriginal) {
    auto *original = new ds::DoublyLinkedList<int>();
    fill(*original, {1, 2, 3});
    ds::DoublyLinkedList<int> copy(*original);
    delete original;
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyInitializationWithEquals) {
    ds::DoublyLinkedList<int> original;
    fill(original, {1, 2});
    ds::DoublyLinkedList<int> copy = original;
    EXPECT_TRUE(HasContents(copy, {1, 2}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

// Copy assignment

TEST(DoublyLinkedList, CopyAssignmentIntoEmptyList) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::DoublyLinkedList<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyAssignmentReplacesLongerList) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9, 10});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(DoublyLinkedList, CopyAssignmentReplacesShorterList) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3, 4});
    ds::DoublyLinkedList<int> target;
    fill(target, {9});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3, 4}));
}

TEST(DoublyLinkedList, CopyAssignmentFromEmptyListEmptiesTarget) {
    ds::DoublyLinkedList<int> source;
    ds::DoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(DoublyLinkedList, CopyAssignmentThenAddUsesNewTail) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = source;
    target.add(3);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2}));
}

TEST(DoublyLinkedList, CopyAssignmentAllocatesNewNodes) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::DoublyLinkedList<int> target;
    target = source;
    const auto sourceNodes = nodesOf(source);
    for (const auto *node : nodesOf(target)) {
        for (const auto *shared : sourceNodes) {
            EXPECT_NE(node, shared) << "the target shares a node with the source";
        }
    }
}

TEST(DoublyLinkedList, ChangingAssignedListLeavesSourceUnchanged) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::DoublyLinkedList<int> target;
    target = source;
    target.deleteNode(2);
    EXPECT_TRUE(HasContents(target, {1, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(DoublyLinkedList, SelfCopyAssignmentKeepsContents) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::DoublyLinkedList<int> &alias = list;
    list = alias;
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyAssignmentReturnsTarget) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1});
    ds::DoublyLinkedList<int> target;
    EXPECT_EQ(&(target = source), &target);
}

TEST(DoublyLinkedList, ChainedCopyAssignment) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::DoublyLinkedList<int> first;
    ds::DoublyLinkedList<int> second;
    fill(second, {9, 9, 9});
    second = first = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
    EXPECT_NE(first.getHead(), second.getHead());
}

TEST(DoublyLinkedList, ReturnByValue) {
    ds::DoublyLinkedList<int> list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AssignFromTemporary) {
    ds::DoublyLinkedList<int> list;
    fill(list, {9, 9});
    list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, CanBeStoredInStdVector) {
    std::vector<ds::DoublyLinkedList<int>> lists;
    for (int i = 0; i < 10; ++i) {
        ds::DoublyLinkedList<int> list;
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

TEST(DoublyLinkedList, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<ds::DoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ds::DoublyLinkedList<int>>::value);
}

TEST(DoublyLinkedList, MoveConstructorTakesTheNodes) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    ds::DoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
    EXPECT_EQ(nodesOf(moved), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(DoublyLinkedList, MoveConstructorLeavesSourceEmpty) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::DoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MoveConstructorOfEmptyList) {
    ds::DoublyLinkedList<int> source;
    ds::DoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MovedFromListIsReusable) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    ds::DoublyLinkedList<int> moved(std::move(source));
    source.add(4);
    source.addFirst(3);
    EXPECT_TRUE(HasContents(source, {3, 4}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(DoublyLinkedList, MoveAssignmentTakesTheNodes) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    ds::DoublyLinkedList<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_EQ(nodesOf(target), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(DoublyLinkedList, MoveAssignmentReplacesExistingContents) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(DoublyLinkedList, MoveAssignmentLeavesSourceEmpty) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    ds::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MoveAssignmentFromEmptyListEmptiesTarget) {
    ds::DoublyLinkedList<int> source;
    ds::DoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MoveAssignmentReturnsTarget) {
    ds::DoublyLinkedList<int> source;
    fill(source, {1});
    ds::DoublyLinkedList<int> target;
    EXPECT_EQ(&(target = std::move(source)), &target);
}

// Self-move goes through a reference so the compiler doesn't warn about
// `list = std::move(list)`. A leak or double free is reported by ASan when
// the list is destroyed at the end of the test.

TEST(DoublyLinkedList, SelfMoveAssignmentKeepsContents) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::DoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, SelfMoveAssignmentKeepsTheNodes) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const auto nodes = nodesOf(list);
    ds::DoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_EQ(nodesOf(list), nodes);
}

TEST(DoublyLinkedList, SelfMoveAssignmentOnEmptyList) {
    ds::DoublyLinkedList<int> list;
    ds::DoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {}));
}

TEST(DoublyLinkedList, SelfMoveAssignmentOnSingleNode) {
    ds::DoublyLinkedList<int> list;
    fill(list, {7});
    ds::DoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    EXPECT_TRUE(HasContents(list, {7}));
}

TEST(DoublyLinkedList, SelfMoveAssignmentThenModify) {
    ds::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    ds::DoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    list.addFirst(0);
    list.addLast(4);
    list.deleteNode(2);
    EXPECT_TRUE(HasContents(list, {0, 1, 3, 4}));
}
