#include <iostream>

#ifndef ALGORITHMS_ARRAYSTACK_H
#define ALGORITHMS_ARRAYSTACK_H

template<typename T, const int U>
class ArrayStack {
private:
    T top_;
    T array_[U];

public:
    ArrayStack() {
        top_ = -1;
    }

    void push(const T &data) {
        array_[++top_] = data;
    }

    T pop() {
        return top_ == -1 ? top_ : array_[top_--];
    }

    T top() {
        return top_;
    }

    void printStack() {
        if (isEmpty()) {
            std::cout << top() << std::endl;
        }
        for (int i = 0; i <= top_; ++i) {
            std::cout << array_[i] << " ";
        }
        std::cout << std::endl;
    }

    bool isEmpty() {
        return top() == -1;
    }
};

void ArrayStackTest() {
    int t, data;
    std::cin >> t;
    ArrayStack<int, 1000> stack;
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

#endif //ALGORITHMS_ARRAYSTACK_H
