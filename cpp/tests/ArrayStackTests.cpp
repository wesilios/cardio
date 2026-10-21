#include "../DataStructures/Stack/ArrayStack.h"
#include <gtest/gtest.h>

TEST(ArrayStack, PushThenPopReturnsLastPushed) {
    dsa::ArrayStack<int> stack(0);
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.size(), 1);
}

TEST(ArrayStack, PopOnEmptyThrows) {
    dsa::ArrayStack<int> stack(0);
    EXPECT_THROW(stack.pop(), std::runtime_error);
}
