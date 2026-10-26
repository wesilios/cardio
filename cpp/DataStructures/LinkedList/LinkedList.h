#ifndef ALGORITHMS_LINKEDLIST_H
#define ALGORITHMS_LINKEDLIST_H

#include <cstddef>
#include <string>

namespace dsa {
template <typename T, class U> class LinkedList {
  public:
    virtual std::size_t size() const = 0;
    virtual bool isEmpty() const = 0;

    virtual void clear() = 0;

    virtual void add(const T &data) = 0;
    virtual void addFirst(const T &data) = 0;
    virtual void addLast(const T &data) = 0;
    virtual void addAtIndex(const int &index, const T &data) = 0;

    virtual void deleteFirst() = 0;
    virtual void deleteLast() = 0;
    virtual void deleteNode(const T &data) = 0;
    virtual void deleteNodeAtIndex(const int &index) = 0;

    virtual void printList() const = 0;
    virtual void findByIndex(const int &index) const = 0;

    virtual U *getHead() const = 0;
    virtual U *getTail() const = 0;

    virtual ~LinkedList() = default;

    virtual std::string toString() const = 0;
};
} // namespace dsa

#endif // ALGORITHMS_LINKEDLIST_H
