#ifndef ALGORITHMS_ARRAYQUEUE_H
#define ALGORITHMS_ARRAYQUEUE_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "Queue.h"

namespace dsa {
    void ArrayQueueTest();

    template<typename T>
    class ArrayQueue final : public Queue<T> {
    private:
        int capacity_;
        std::vector<T> data_;

    public:
        explicit ArrayQueue(int id) : Queue<T>(id) {
            capacity_ = data_.capacity();
        }

        ~ArrayQueue() override {
            clear();
        }

        int id() override {
            return Queue<T>::id();
        }

        int size() override {
            return data_.size();
        };

        void clear() override {
            data_.clear();
            data_.shrink_to_fit();
            capacity_ = data_.capacity();
        };

        bool isEmpty() override {
            return data_.empty();
        };

        void push(const T &element) override {
            data_.push_back(element);
        };

        T pop() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }
            T element = data_.front();
            data_.erase(data_.begin());
            return element;
        };

        T peek() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }
            return data_.front();
        };

        __attribute__((unused)) int capacity() {
            return capacity_;
        };

        void printStack() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }

            for (int i: data_) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        };
    };

    void ArrayQueueTest() {
        int t, data;
        std::cin >> t;
        ArrayQueue<int> queue(0);
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

#endif //ALGORITHMS_ARRAYQUEUE_H
