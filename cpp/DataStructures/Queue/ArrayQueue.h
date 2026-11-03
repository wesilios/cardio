#ifndef DATASTRUCTURE_ARRAYQUEUE_H
#define DATASTRUCTURE_ARRAYQUEUE_H

#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

#include "../Exception.h"
#include "Queue.h"

namespace ds {
void ArrayQueueTest();

template <typename T> class ArrayQueue final : public Queue<T> {
  private:
    std::vector<T> data_;

  public:
    explicit ArrayQueue() : Queue<T>() {}

    std::size_t size() const override { return data_.size(); };

    void clear() override {
        data_.clear();
        data_.shrink_to_fit();
    };

    bool isEmpty() const override { return data_.empty(); };

    void enqueue(const T &element) override { data_.push_back(element); };

    T dequeue() override {
        if (isEmpty()) {
            throw EmptyContainer("Empty queue");
        }
        T element = data_.front();
        data_.erase(data_.begin());
        return element;
    };

    T front() const override {
        if (isEmpty()) {
            throw EmptyContainer("Empty queue");
        }
        return data_.front();
    };

    __attribute__((unused)) int capacity() { return data_.capacity(); };

    void printQueue() const override {
        if (isEmpty()) {
            std::cout << "[  ]" << std::endl;
            return;
        }

        std::cout << "[ ";
        for (std::size_t index = 0; index < size(); index++) {
            std::cout << data_.at(index);
            if (index < size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << " ]" << std::endl;
    };
};

inline void ArrayQueueTest() {
    int t, data;
    std::cout << "Number of values to enqueue: ";
    std::cin >> t;
    ArrayQueue<int> queue;
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

#endif // DATASTRUCTURE_ARRAYQUEUE_H
