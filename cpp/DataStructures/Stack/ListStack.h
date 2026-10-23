#ifndef ALGORITHMS_LISTSTACK_H
#define ALGORITHMS_LISTSTACK_H

#include "../Exception.h"
#include "../LinkedList/SinglyLinkedList.h"
#include "Stack.h"
#include <iostream>

namespace dsa {
void ListStackTest();

template <typename T> class ListStack final : public Stack<T> {
  private:
    SinglyLinkedList<T> linkedList_;

  public:
    explicit ListStack(int id) : Stack<T>(id) {}

    virtual ~ListStack() override { clear(); }

    int id() override { return Stack<T>::id(); }

    int size() override { return linkedList_.size(); }

    void clear() override { linkedList_.clear(); }

    bool isEmpty() override { return linkedList_.isEmpty(); }

    void push(const T &element) override { linkedList_.add(element); }

    T pop() override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        T data = linkedList_.getTail()->getData();
        linkedList_.deleteLast();
        return data;
    }

    T peek() override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        return linkedList_.getTail()->getData();
    }

    void printStack() override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        linkedList_.printList();
    }
};

void ListStackTest() {
    int t, data;
    std::cout << "Number of values to push: ";
    std::cin >> t;
    ListStack<int> stack(0);
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
} // namespace dsa

#endif // ALGORITHMS_LISTSTACK_H
