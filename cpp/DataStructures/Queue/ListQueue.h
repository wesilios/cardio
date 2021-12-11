#ifndef ALGORITHMS_LISTQUEUE_H
#define ALGORITHMS_LISTQUEUE_H

#include <iostream>
#include <vector>

#include "Queue.h"
#include "../LinkedList/SinglyLinkedList.h"

namespace dsa {
    void ListQueueTest();

    template<typename T>
    class ListQueue final : public Queue<T> {
    private:
        SinglyLinkedList<T> linkedList_;

    public:
        explicit ListQueue(int id) : Queue<T>(id) {}

        ~ListQueue() override {
            clear();
        }

        int id() override {
            return Queue<T>::id();
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

            T element = linkedList_.getHead()->getData();
            linkedList_.deleteFirst();
            return element;
        }

        T peek() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }

            T element = linkedList_.getHead()->getData();
            return element;
        }

        void printStack() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }
            linkedList_.printList();
        }
    };

    void ListQueueTest() {
        int t, data;
        std::cin >> t;
        ListQueue<int> queue(0);
        for (int i = 0; i < t; i++) {
            std::cin >> data;
            queue.push(data);
        }
        queue.printStack();
        std::cin >> t;
        for (int i = 0; i < t; i++) {
            std::cout << queue.peek() << " ";
        }
        std::cout << std::endl;
        queue.printStack();
        for (int i = 0; i < t; i++) {
            std::cout << "Pop: " << queue.pop() << std::endl;
            queue.printStack();
        }
        std::cout << std::endl;
    }
}

#endif //ALGORITHMS_LISTQUEUE_H
