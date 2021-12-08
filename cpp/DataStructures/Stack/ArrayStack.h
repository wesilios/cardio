#include <iostream>

#ifndef ALGORITHMS_ARRAYSTACK_H
#define ALGORITHMS_ARRAYSTACK_H

template<typename T, const int U>
class ArrayStack {
private:
    T top;
    T array[U];

public:
    ArrayStack() {
        top = -1;
    }

    ArrayStack(int length) {
        top = -1;
        array[length];
    }

    void push(const T &data) {
        array[++top] = data;
    }

    T pop() {
        return top == -1 ? top : array[top--];
    }

    void printStack() {
        for (int i = 0; i <= top; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
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
