#ifndef DATASTRUCTURE_ARRAYSTACK_H
#define DATASTRUCTURE_ARRAYSTACK_H

#include "../Exception.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

#include "Stack.h"

namespace ds {
void ArrayStackTest();

template <typename T> class ArrayStack final : public Stack<T> {
  private:
    std::vector<T> data_;

  public:
    explicit ArrayStack() : Stack<T>() {}

    void clear() override {
        data_.clear();
        data_.shrink_to_fit();
    }

    std::size_t size() const override { return data_.size(); }

    bool isEmpty() const override { return data_.empty(); }

    void push(const T &element) override { data_.push_back(element); }

    T pop() override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        T element = data_.back();
        data_.erase(data_.end() - 1);
        return element;
    }

    T peek() const override {
        if (isEmpty()) {
            throw EmptyContainer("Empty stack");
        }

        return data_.back();
    }

    void printStack() const override {
        std::cout << "[ ";
        for (std::size_t index = size(); index-- > 0;) {
            std::cout << data_.at(index);
            if (index > 0) {
                std::cout << ", ";
            }
        }

        std::cout << " ]" << std::endl;
    }

    __attribute__((unused)) int capacity() const { return data_.capacity(); }
};

inline void ArrayStackTest() {
    int t, data;
    std::cout << "Number of values to push: ";
    std::cin >> t;
    ArrayStack<int> stack;
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

#endif // DATASTRUCTURE_ARRAYSTACK_H
