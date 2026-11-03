#ifndef DATASTRUCTURE_STACK_H
#define DATASTRUCTURE_STACK_H

#include <cstddef>

namespace ds {
template <typename T> class Stack {
  public:
    explicit Stack() {}

    virtual ~Stack() = default;

    virtual std::size_t size() const = 0;
    virtual bool isEmpty() const = 0;

    virtual void clear() = 0;

    virtual void push(const T &element) = 0;
    virtual T pop() = 0;
    virtual T peek() const = 0;

    virtual void printStack() const = 0;
};
} // namespace ds

#endif // DATASTRUCTURE_STACK_H
