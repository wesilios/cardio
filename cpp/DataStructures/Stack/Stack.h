#ifndef ALGORITHMS_STACK_H
#define ALGORITHMS_STACK_H

#include <iostream>

namespace dsa {
    template<typename T>
    class Stack {
    private:
        int id_;
    public:
        explicit Stack(int id) : id_(id) {}

        virtual ~Stack() = default;

        virtual int id() {
            return id_;
        }

        virtual int size() = 0;

        virtual void clear() = 0;

        virtual bool isEmpty() = 0;

        virtual void push(const T &element) = 0;

        virtual T pop() = 0;

        virtual T peek() = 0;

        virtual int capacity() = 0;

        virtual void printStack() = 0;
    };
}

#endif //ALGORITHMS_STACK_H
