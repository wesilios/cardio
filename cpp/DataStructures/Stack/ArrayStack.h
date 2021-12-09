#ifndef ALGORITHMS_ARRAYSTACK_H
#define ALGORITHMS_ARRAYSTACK_H

#include <iostream>
#include <vector>
#include <exception>

#include "Stack.h"

namespace dsa {
    void ArrayStackTest();

    template<typename T>
    class ArrayStack final : public Stack<T> {
    private:
        int capacity_;
        std::vector<T> data_;

    public:
        explicit ArrayStack(int id) : Stack<T>(id) {
            capacity_ = data_.capacity();
        }

        ~ArrayStack() override {
            data_.clear();
        }

        void clear() override {
            data_.clear();
            data_.shrink_to_fit();
            capacity_ = data_.capacity();
        }

        int size() override {
            return data_.size();
        }

        bool isEmpty() override {
            return data_.empty();
        }

        void push(const T &element) override {
            data_.push_back(element);
        }

        T pop() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }
            T element = data_.back();
            data_.erase(data_.end() - 1);
            return element;
        }

        T peek() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }
            return data_.back();
        }

        void printStack() override {
            if (isEmpty()) {
                throw std::runtime_error("Empty stack");
            }

            for (int i: data_) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }

        int capacity() override {
            return capacity_;
        }
    };

    void ArrayStackTest() {
        int t, data;
        std::cin >> t;
        ArrayStack<int> stack(0);
        for (int i = 0; i < t; i++) {
            std::cin >> data;
            stack.push(data);
        }
        stack.printStack();
        std::cin >> t;
        for (int i = 0; i < t; i++) {
            std::cout << stack.pop() << " ";
        }
        std::cout << std::endl;
        stack.printStack();
    }
}


#endif //ALGORITHMS_ARRAYSTACK_H
