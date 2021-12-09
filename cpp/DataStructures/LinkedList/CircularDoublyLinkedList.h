#ifndef ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H
#define ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "LinkedList.h"
#include "../Exception.h"

namespace dsa {
    template<typename T>
    class CircularDoublyLinkedList : public LinkedList<T, DoublyNode<T>> {
    private:
        int size_ = 0;
        DoublyNode<T> *head_;
        DoublyNode<T> *tail_;

    public:

        CircularDoublyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {
        }

        virtual ~

        CircularDoublyLinkedList() {
            clear();
        }

        void clear() {
            DoublyNode<T> *node = head_;
            while (node->getNext() != head_) {
                DoublyNode<T> *next = node->getNext();
                delete node;
                node = next;
            }
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
        }

        int size()

        override {
            return
                    size_;
        }

        bool isEmpty()

        override {
            return

                    size()

                    == 0;
        }

        void printList()

        override {
            DoublyNode<T> *node = head_;
            for (
                    int i = 0;
                    node != nullptr &&
                    i < size();
                    ++i) {
                std::cout << node->

                        getData()

                          << " ";
                node = node->getNext();
            }
            std::cout <<
                      std::endl;
        }

        void add(const T &data)

        override {
            addLast(data);
        }

        void addLast(const T &data)

        override {
            if (

                    isEmpty()

                    ) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                head_->
                        setNext(head_);
                head_->
                        setPrevious(head_);
                tail_ = head_;
                size_++;
                return;
            }

            auto *node = new DoublyNode<T>(data, tail_, head_);
            tail_->
                    setNext(node);
            head_->
                    setPrevious(node);
            tail_ = node;
            size_++;
        }

        void addFirst(const T &data)

        override {
            if (

                    isEmpty()

                    ) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                head_->
                        setNext(head_);
                head_->
                        setPrevious(head_);
                tail_ = head_;
                size_++;
                return;
            }

            auto *node = new DoublyNode<T>(data, tail_, head_);
            tail_->
                    setNext(node);
            head_->
                    setPrevious(node);
            head_ = node;
            size_++;
        }

        void addAtIndex(const int &index, const T &data)

        override {
            if (
                    index < 0 || index > size_
                    ) {
                throw InvalidArgument("Illegal index");
            }

            if (index == 0) {
                addFirst(data);
                return;
            }

            if (index == size_ - 1) {
                addLast(data);
                return;
            }

            DoublyNode<T> *node = head_;
            for (
                    int i = 0;
                    i < index - 1; ++i) {
                node = node->getNext();
            }

            auto *newNode = new DoublyNode<T>(data, node, node->getNext());
            node->getNext()->
                    setPrevious(newNode);
            node->
                    setNext(newNode);
            size_++;
        }

        void findByIndex(const int &index)

        override {
            if (

                    index < 0 || index > size()

                    ) {
                throw InvalidArgument("Illegal index");
            }

            DoublyNode<T> *node = head_;
            for (
                    int i = 0;
                    i < index - 1; ++i) {
                node = node->getNext();
            }
            std::cout << node->

                    getData()

                      <<
                      std::endl;
        }

        void deleteFirst()

        override {
            DoublyNode<T> *head = head_;
            head_ = head->getNext();
            head_->
                    setPrevious(tail_);
            delete
                    head;
            size_--;
        }

        void deleteLast()

        override {
            DoublyNode<T> *tail = tail_;
            tail_ = tail->getPrevious();
            tail_->
                    setNext(head_);
            delete
                    tail;
            size_--;
        }

        void deleteNode(const T &data)

        override {
            if (head_->

                    getData()

                == data) {
                deleteFirst();

            }

            if (tail_->

                    getData()

                == data) {
                deleteLast();

            }

            DoublyNode<T> *node = head_->getNext();
            while (node != tail_) {
                if (node->

                        getData()

                    != data) {
                    node = node->getNext();
                    continue;
                }

                DoublyNode<T> *next = node->getNext();
                node->getPrevious()->
                        setNext(node
                                        ->

                                                getNext()

                );
                next->
                        setPrevious(node
                                            ->

                                                    getPrevious()

                );
                delete
                        node;
                node = next;
                size_--;
            }
        }

        void deleteNodeAtIndex(const int &index)

        override {
            if (

                    index < 0 || index > size()

                    ) {
                throw InvalidArgument("Illegal index");
            }

            if (index == 0) {
                deleteFirst();

                return;
            }

            if (index ==

                size()

                - 1) {
                deleteLast();

                return;
            }

            DoublyNode<T> *node = head_->getNext();
            for (
                    int i = 1;
                    i < index - 1; ++i) {
                node = node->getNext();
            }

            DoublyNode<T> *next = node->getNext();
            node->getPrevious()->
                    setNext(next);
            next->
                    setPrevious(node
                                        ->

                                                getPrevious()

            );
            delete
                    node;
            size_--;
        }

        DoublyNode<T> *getHead()

        override {
            return
                    head_;
        }

        DoublyNode<T> *getTail()

        override {
            return
                    tail_;
        }
    };

    void CircularDoublyLinkedListTest() {
        int n, data;
        std::cin >> n;
        CircularDoublyLinkedList<int> linkedList;
        for (int i = 0; i < n; i++) {
            std::cin >> data;
            linkedList.add(data);
        }
        linkedList.printList();

        int index;
        std::cin >> index >> data;
        linkedList.addAtIndex(index, data);
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;

        std::cin >> data;
        linkedList.addFirst(data);
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;

        std::cin >> data;
        linkedList.addLast(data);
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;

        std::cin >> index;
        linkedList.findByIndex(index);

        linkedList.deleteLast();
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;

        linkedList.deleteFirst();
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;

        std::cin >> data;
        linkedList.deleteNode(data);
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;

        std::cin >> index;
        linkedList.deleteNodeAtIndex(index);
        linkedList.printList();
        std::cout << "Size: " << linkedList.size() << std::endl;
    }

}

#endif //ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H
