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

// ---------- Iterator ----------

TEST(DoublyLinkedList, RangeForVisitsEveryValueInOrder) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(DoublyLinkedList, RangeForOnEmptyListVisitsNothing) {
    dsa::DoublyLinkedList<int> list;
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_TRUE(visited.empty());
}

TEST(DoublyLinkedList, RangeForOnSingleNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {7});
    std::vector<int> visited;
    for (int value : list) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({7}));
}

TEST(DoublyLinkedList, RangeForSeesChangesToTheList) {
    dsa::DoublyLinkedList<int> list;
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
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    const dsa::DoublyLinkedList<int> &constList = list;
    std::vector<int> visited;
    for (int value : constList) {
        visited.push_back(value);
    }
    EXPECT_EQ(visited, std::vector<int>({1, 2, 3}));
}

TEST(DoublyLinkedList, BeginEqualsEndOnEmptyList) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_TRUE(list.begin() == list.end());
    EXPECT_FALSE(list.begin() != list.end());
}

TEST(DoublyLinkedList, BeginDiffersFromEndOnNonEmptyList) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1});
    EXPECT_TRUE(list.begin() != list.end());
    EXPECT_FALSE(list.begin() == list.end());
}

TEST(DoublyLinkedList, DereferenceReturnsCurrentValue) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
}

TEST(DoublyLinkedList, PrefixIncrementMovesToNextNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {10, 20, 30});
    auto it = list.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*++it, 30);
}

TEST(DoublyLinkedList, PostfixIncrementReturnsPreviousPosition) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);
    EXPECT_EQ(*it, 20);
}

TEST(DoublyLinkedList, IncrementPastLastNodeReachesEnd) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {10, 20});
    auto it = list.begin();
    ++it;
    ++it;
    EXPECT_TRUE(it == list.end());
}

TEST(DoublyLinkedList, IncrementAtEndStaysAtEnd) {
    dsa::DoublyLinkedList<int> list;
    auto it = list.end();
    ++it;
    EXPECT_TRUE(it == list.end());
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

TEST(DoublyLinkedList, ToStringOnEmptyList) {
    dsa::DoublyLinkedList<int> list;
    EXPECT_EQ(list.toString(), "[  ]");
}

TEST(DoublyLinkedList, ToStringOnSingleNode) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {7});
    EXPECT_EQ(list.toString(), "[ 7 ]");
}

TEST(DoublyLinkedList, ToStringAfterDeletes) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3, 4});
    list.deleteFirst();
    list.deleteLast();
    EXPECT_EQ(list.toString(), "[ 2, 3 ]");
}

// ---------- Copy (rule of three) ----------
// A copy owns its own nodes (deep copy): changing one list never changes the
// other. Double frees and leaks here only show up reliably in the sanitizer build.

namespace {

std::vector<const dsa::DoublyNode<int> *> nodesOf(const dsa::DoublyLinkedList<int> &list) {
    std::vector<const dsa::DoublyNode<int> *> nodes;
    auto *node = list.getHead();
    for (std::size_t i = 0; node != nullptr && i < list.size(); ++i) {
        nodes.push_back(node);
        node = node->getNext();
    }
    return nodes;
}

dsa::DoublyLinkedList<int> makeList(const std::vector<int> &values) {
    dsa::DoublyLinkedList<int> list;
    fill(list, values);
    return list;
}

} // namespace

TEST(DoublyLinkedList, IsCopyConstructibleAndAssignable) {
    EXPECT_TRUE(std::is_copy_constructible<dsa::DoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<dsa::DoublyLinkedList<int>>::value);
}

// Copy constructor

TEST(DoublyLinkedList, CopyConstructorCopiesValues) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyConstructorOfEmptyList) {
    dsa::DoublyLinkedList<int> original;
    dsa::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {}));
    EXPECT_TRUE(HasContents(original, {}));
}

TEST(DoublyLinkedList, CopyConstructorOfSingleNode) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {7});
    dsa::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {7}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

TEST(DoublyLinkedList, CopyConstructorKeepsDuplicates) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {2, 2, 1, 2});
    dsa::DoublyLinkedList<int> copy(original);
    EXPECT_TRUE(HasContents(copy, {2, 2, 1, 2}));
}

TEST(DoublyLinkedList, CopyConstructorAllocatesNewNodes) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::DoublyLinkedList<int> copy(original);
    const auto originalNodes = nodesOf(original);
    for (const auto *node : nodesOf(copy)) {
        for (const auto *shared : originalNodes) {
            EXPECT_NE(node, shared) << "the copy shares a node with the original";
        }
    }
}

TEST(DoublyLinkedList, ChangingCopyLeavesOriginalUnchanged) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::DoublyLinkedList<int> copy(original);
    copy.deleteFirst();
    copy.deleteLast();
    copy.addLast(9);
    copy.addFirst(0);
    EXPECT_TRUE(HasContents(copy, {0, 2, 9}));
    EXPECT_TRUE(HasContents(original, {1, 2, 3}));
}

TEST(DoublyLinkedList, ChangingOriginalLeavesCopyUnchanged) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {1, 2, 3});
    dsa::DoublyLinkedList<int> copy(original);
    original.clear();
    original.add(5);
    EXPECT_TRUE(HasContents(original, {5}));
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyOutlivesOriginal) {
    auto *original = new dsa::DoublyLinkedList<int>();
    fill(*original, {1, 2, 3});
    dsa::DoublyLinkedList<int> copy(*original);
    delete original;
    EXPECT_TRUE(HasContents(copy, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyInitializationWithEquals) {
    dsa::DoublyLinkedList<int> original;
    fill(original, {1, 2});
    dsa::DoublyLinkedList<int> copy = original;
    EXPECT_TRUE(HasContents(copy, {1, 2}));
    EXPECT_NE(copy.getHead(), original.getHead());
}

// Copy assignment

TEST(DoublyLinkedList, CopyAssignmentIntoEmptyList) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::DoublyLinkedList<int> target;
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyAssignmentReplacesLongerList) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9, 10});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(DoublyLinkedList, CopyAssignmentReplacesShorterList) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3, 4});
    dsa::DoublyLinkedList<int> target;
    fill(target, {9});
    target = source;
    EXPECT_TRUE(HasContents(target, {1, 2, 3, 4}));
}

TEST(DoublyLinkedList, CopyAssignmentFromEmptyListEmptiesTarget) {
    dsa::DoublyLinkedList<int> source;
    dsa::DoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = source;
    EXPECT_TRUE(HasContents(target, {}));
}

TEST(DoublyLinkedList, CopyAssignmentThenAddUsesNewTail) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = source;
    target.add(3);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2}));
}

TEST(DoublyLinkedList, CopyAssignmentAllocatesNewNodes) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::DoublyLinkedList<int> target;
    target = source;
    const auto sourceNodes = nodesOf(source);
    for (const auto *node : nodesOf(target)) {
        for (const auto *shared : sourceNodes) {
            EXPECT_NE(node, shared) << "the target shares a node with the source";
        }
    }
}

TEST(DoublyLinkedList, ChangingAssignedListLeavesSourceUnchanged) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::DoublyLinkedList<int> target;
    target = source;
    target.deleteNode(2);
    EXPECT_TRUE(HasContents(target, {1, 3}));
    EXPECT_TRUE(HasContents(source, {1, 2, 3}));
}

TEST(DoublyLinkedList, SelfCopyAssignmentKeepsContents) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {1, 2, 3});
    dsa::DoublyLinkedList<int> &alias = list;
    list = alias;
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, CopyAssignmentReturnsTarget) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1});
    dsa::DoublyLinkedList<int> target;
    EXPECT_EQ(&(target = source), &target);
}

TEST(DoublyLinkedList, ChainedCopyAssignment) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::DoublyLinkedList<int> first;
    dsa::DoublyLinkedList<int> second;
    fill(second, {9, 9, 9});
    second = first = source;
    EXPECT_TRUE(HasContents(first, {1, 2}));
    EXPECT_TRUE(HasContents(second, {1, 2}));
    EXPECT_NE(first.getHead(), second.getHead());
}

TEST(DoublyLinkedList, ReturnByValue) {
    dsa::DoublyLinkedList<int> list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, AssignFromTemporary) {
    dsa::DoublyLinkedList<int> list;
    fill(list, {9, 9});
    list = makeList({1, 2, 3});
    EXPECT_TRUE(HasContents(list, {1, 2, 3}));
}

TEST(DoublyLinkedList, CanBeStoredInStdVector) {
    std::vector<dsa::DoublyLinkedList<int>> lists;
    for (int i = 0; i < 10; ++i) {
        dsa::DoublyLinkedList<int> list;
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
    EXPECT_TRUE(std::is_nothrow_move_constructible<dsa::DoublyLinkedList<int>>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<dsa::DoublyLinkedList<int>>::value);
}

TEST(DoublyLinkedList, MoveConstructorTakesTheNodes) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    dsa::DoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
    EXPECT_EQ(nodesOf(moved), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(DoublyLinkedList, MoveConstructorLeavesSourceEmpty) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::DoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MoveConstructorOfEmptyList) {
    dsa::DoublyLinkedList<int> source;
    dsa::DoublyLinkedList<int> moved(std::move(source));
    EXPECT_TRUE(HasContents(moved, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MovedFromListIsReusable) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    dsa::DoublyLinkedList<int> moved(std::move(source));
    source.add(4);
    source.addFirst(3);
    EXPECT_TRUE(HasContents(source, {3, 4}));
    EXPECT_TRUE(HasContents(moved, {1, 2, 3}));
}

TEST(DoublyLinkedList, MoveAssignmentTakesTheNodes) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2, 3});
    const auto nodes = nodesOf(source);
    dsa::DoublyLinkedList<int> target;
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2, 3}));
    EXPECT_EQ(nodesOf(target), nodes) << "a move must reuse the nodes, not copy them";
}

TEST(DoublyLinkedList, MoveAssignmentReplacesExistingContents) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {1, 2}));
}

TEST(DoublyLinkedList, MoveAssignmentLeavesSourceEmpty) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1, 2});
    dsa::DoublyLinkedList<int> target;
    fill(target, {7, 8, 9});
    target = std::move(source);
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MoveAssignmentFromEmptyListEmptiesTarget) {
    dsa::DoublyLinkedList<int> source;
    dsa::DoublyLinkedList<int> target;
    fill(target, {1, 2, 3});
    target = std::move(source);
    EXPECT_TRUE(HasContents(target, {}));
    EXPECT_TRUE(HasContents(source, {}));
}

TEST(DoublyLinkedList, MoveAssignmentReturnsTarget) {
    dsa::DoublyLinkedList<int> source;
    fill(source, {1});
    dsa::DoublyLinkedList<int> target;
    EXPECT_EQ(&(target = std::move(source)), &target);
}
