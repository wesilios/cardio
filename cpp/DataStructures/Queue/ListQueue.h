#ifndef DATASTRUCTURE_LISTQUEUE_H
#define DATASTRUCTURE_LISTQUEUE_H

#include <cstddef>
#include <iostream>

#include "../Exception.h"
#include "../LinkedList/SinglyLinkedList.h"
#include "Queue.h"

namespace ds {
void ListQueueTest();

template <typename T> class ListQueue final : public Queue<T> {
  private:
    SinglyLinkedList<T> linkedList_;

  public:
    explicit ListQueue() : Queue<T>() {}

    std::size_t size() const override { return linkedList_.size(); }

    void clear() override { linkedList_.clear(); }

    bool isEmpty() const override { return linkedList_.isEmpty(); }

    void enqueue(const T &element) override { linkedList_.add(element); }

    T dequeue() override {
        if (isEmpty()) {
            throw ds::EmptyContainer("Empty queue");
        }

        T element = linkedList_.getHead()->getData();
        linkedList_.deleteFirst();
        return element;
    }

    T front() const override {
        if (isEmpty()) {
            throw ds::EmptyContainer("Empty queue");
        }

        T element = linkedList_.getHead()->getData();
        return element;
    }

    void printQueue() const override {
        if (isEmpty()) {
            std::cout << "[  ]" << std::endl;
            return;
        }
        linkedList_.printList();
    }
};

inline void ListQueueTest() {
    int t, data;
    std::cout << "Number of values to enqueue: ";
    std::cin >> t;
    ListQueue<int> queue;
    std::cout << "Enter " << t << " values: ";
    for (int i = 0; i < t; i++) {
        std::cin >> data;
        queue.enqueue(data);
    }
    std::cout << "Queue: ";
    queue.printQueue();

    std::cout << "Number of times to front and dequeue: ";
    std::cin >> t;
    std::cout << "Front: ";
    for (int i = 0; i < t; i++) {
        std::cout << queue.front() << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < t; i++) {
        std::cout << "Dequeue: " << queue.dequeue() << std::endl;
        if (queue.isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            break;
        }
        std::cout << "Queue: ";
        queue.printQueue();
    }
}
} // namespace ds

#endif // DATASTRUCTURE_LISTQUEUE_H
