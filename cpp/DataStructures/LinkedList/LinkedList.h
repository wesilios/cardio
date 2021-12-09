#ifndef ALGORITHMS_LINKEDLIST_H
#define ALGORITHMS_LINKEDLIST_H

namespace dsa {
    template<typename T, class U>
    class LinkedList {
    public:
        virtual int size() = 0;

        virtual bool isEmpty() = 0;

        virtual void add(const T &data) = 0;

        virtual void addLast(const T &data) = 0;

        virtual void addFirst(const T &data) = 0;

        virtual void addAtIndex(const int &index, const T &data) = 0;

        virtual void printList() = 0;

        virtual void deleteFirst() = 0;

        virtual void deleteLast() = 0;

        virtual void deleteNode(const T &data) = 0;

        virtual void deleteNodeAtIndex(const int &index) = 0;

        virtual void findByIndex(const int &index) = 0;

        virtual U *getHead() = 0;

        virtual U *getTail() = 0;
    };
}

#endif //ALGORITHMS_LINKEDLIST_H
