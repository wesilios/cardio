#ifndef ALGORITHMS_LISTSTACK_H
#define ALGORITHMS_LISTSTACK_H

#include <iostream>
#include <exception>
#include "Stack.h"
#include "../LinkedList/SinglyLinkedList.h"

namespace dsa {
    void ListStackTest();

    template<typename T>
    class ListStack final : public Stack<T> {
    private:
        SinglyLinkedList <T> linkedList_;

    public:
        explicit ListStack(int id) : Stack<T>(id) {
        }

        ~ListStack() override {
            clear();
        }

        int id() override {
            return Stack<T>::id();
        }

        int size() override {
            return linkedList_.size();
        }

        void clear() override {
            linkedList_.clear();
        }

        bool isEmpty() override {
            return linkedList_.isEmpty();
        }

        void push(const T &element) override {
            linkedList_.add(element);
        }

        T pop() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }

            T data = linkedList_.getTail()->getData();
            linkedList_.deleteLast();
            return data;
        }

        T peek() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }

            return linkedList_.getTail()->getData();
        }

        void printStack() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }

            linkedList_.printList();
        }
    };

    void ListStackTest() {
        int t, data;
        std::cin >> t;
        ListStack<int> stack(0);
        for (int i = 0; i < t; ++i) {
            std::cin >> data;
            stack.push(data);
        }
        stack.printStack();
        std::cin >> t;
        for (int i = 0; i < t; i++) {
            std::cout << stack.peek() << " ";
        }
        std::cout << std::endl;
        stack.printStack();
        for (int i = 0; i < t; i++) {
            std::cout << "Pop: " << stack.pop() << std::endl;
            stack.printStack();
        }
        std::cout << std::endl;
    }
}

#endif //ALGORITHMS_LISTSTACK_H
