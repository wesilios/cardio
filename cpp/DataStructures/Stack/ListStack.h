#ifndef DATASTRUCTURE_LISTSTACK_H
#define DATASTRUCTURE_LISTSTACK_H

#include "../Exception.h"
#include "../LinkedList/DoublyLinkedList.h"
#include "Stack.h"
#include <cstddef>
#include <iostream>

namespace ds {
void ListStackTest();

template <typename T> class ListStack final : public Stack<T> {
  private:
    DoublyLinkedList<T> linkedList_;

  public:
    explicit ListStack() : Stack<T>() {}

    std::size_t size() const override { return linkedList_.size(); }

    void clear() override { linkedList_.clear(); }

    bool isEmpty() const override { return linkedList_.isEmpty(); }

    void push(const T &element) override { linkedList_.add(element); }

    T pop() override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        T data = linkedList_.getTail()->getData();
        linkedList_.deleteLast();
        return data;
    }

    T peek() const override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        return linkedList_.getTail()->getData();
    }

    void printStack() const override {
        if (isEmpty()) {
            std::cout << "[  ]" << std::endl;
            return;
        }

        std::cout << linkedList_.toStringInReverse() << std::endl;
    }
};

inline void ListStackTest() {
    int t, data;
    std::cout << "Number of values to push: ";
    std::cin >> t;
    ListStack<int> stack;
    std::cout << "Enter " << t << " values: ";
    for (int i = 0; i < t; i++) {
        std::cin >> data;
        stack.push(data);
    }
    std::cout << "Stack: ";
    stack.printStack();

    std::cout << "Number of times to peek and pop: ";
    std::cin >> t;
    std::cout << "Peek: ";
    for (int i = 0; i < t; i++) {
        std::cout << stack.peek() << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < t; i++) {
        std::cout << "Pop: " << stack.pop() << std::endl;
        if (stack.isEmpty()) {
            std::cout << "Stack is empty" << std::endl;
            break;
        }
        std::cout << "Stack: ";
        stack.printStack();
    }
}
} // namespace ds

#endif // DATASTRUCTURE_LISTSTACK_H
