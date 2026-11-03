#ifndef DATASTRUCTURE_QUEUE_H
#define DATASTRUCTURE_QUEUE_H

#include <cstddef>

namespace ds {
template <typename T> class Queue {
  public:
    explicit Queue() {}

    virtual ~Queue() = default;

    virtual std::size_t size() const = 0;
    virtual bool isEmpty() const = 0;

    virtual void clear() = 0;

    virtual void enqueue(const T &element) = 0;
    virtual T dequeue() = 0;
    virtual T front() const = 0;

    virtual void printQueue() const = 0;
};
} // namespace ds

#endif // DATASTRUCTURE_QUEUE_H
