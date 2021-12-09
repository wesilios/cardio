#ifndef ALGORITHMS_CIRCULARSINGLYLINKEDLIST_H
#define ALGORITHMS_CIRCULARSINGLYLINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "LinkedList.h"
#include "../Exception.h"

namespace dsa {
    void CircularSinglyLinkedListTest();

    template<typename T>
    class CircularSinglyLinkedList : public LinkedList<T, SinglyNode<T>> {
    private:
        int size_ = 0;
        SinglyNode<T> *head_;
        SinglyNode<T> *tail_;

    public:
        CircularSinglyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {
        }

        virtual ~CircularSinglyLinkedList() {
            clear();
        }

        void clear() {
            SinglyNode<T> *node = head_;
            while (node->getNext() != head_) {
                SinglyNode<T> *next = node->getNext();
                delete node;
                node = next;
            }
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
        }

        int size() override {
            return size_;
        }

        bool isEmpty() override {
            return size() == 0;
        }

        void add(const T &data) override {
            addLast(data);
        }

        void addLast(const T &data) override {
            if (isEmpty()) {
                head_ = new SinglyNode<T>(data, nullptr);
                head_->setNext(head_);
                tail_ = head_;
            } else {
                tail_->setNext(new SinglyNode<T>(data, head_));
                tail_ = tail_->getNext();
            }
            size_++;
        }

        void addFirst(const T &data) override {
            if (isEmpty()) {
                head_ = new SinglyNode<T>(data, nullptr);
                head_->setNext(head_);
                tail_ = head_;
            } else {
                auto *head = new SinglyNode<T>(data, head_);
                head_ = head;
                tail_->setNext(head);
            }
            size_++;
        }

        void addAtIndex(const int &index, const T &data) override {
            if (index < 0) {
                throw InvalidArgument("Illegal index");
            }

            if (index == 0) {
                addFirst(data);
                return;
            }

            if (index == size()) {
                addLast(data);
                return;
            }

            SinglyNode<T> *node = head_;
            for (int i = 1; i < index - 1; i++) {
                node = node->getNext();
            }
            auto *newNode = new SinglyNode<T>(data, node->getNext());
            node->setNext(newNode);
            size_++;
        }

        void printList() override {
            SinglyNode<T> *node = head_;
            for (int i = 0; node != nullptr && i < size(); i++) {
                std::cout << node->getData() << " ";
                node = node->getNext();
            }
            std::cout << std::endl;
        }

        void findByIndex(const int &index) override {
            if (index < 0 || index > size()) {
                throw InvalidArgument("Illegal index");
            }

            SinglyNode<T> *node = head_;
            for (int i = 0; i < index - 1; i++) {
                node = node->getNext();
            }
            std::cout << node->getData() << std::endl;
        }

        void deleteNode(const T &data) override {
            if (head_->getData() == data) {
                deleteFirst();
            }

            if (tail_->getData() == data) {
                deleteLast();
            }

            SinglyNode<T> *node = head_;
            while (node != tail_) {
                if (node->getNext()->getData() != data) {
                    node = node->getNext();
                    continue;
                }

                SinglyNode<T> *next = node->getNext();
                node->setNext(next->getNext());
                node = node->getNext();
                delete next;
                size_--;
            }
        }

        void deleteNodeAtIndex(const int &index) override {
            if (index < 0 || index > size()) {
                throw InvalidArgument("Illegal index");
            }

            if (index == 0) {
                deleteFirst();
                return;
            }

            if (index == size() - 1) {
                deleteLast();
                return;
            }

            SinglyNode<T> *previousNode = head_;
            for (int i = 0; i < index - 2; ++i) {
                previousNode = previousNode->getNext();
            }

            SinglyNode<T> *node = previousNode->getNext();
            previousNode->setNext(previousNode->getNext()->getNext());
            delete node;
            size_--;
        }

        void deleteFirst() override {
            SinglyNode<T> *head = head_;
            tail_->setNext(head->getNext());
            head_ = head->getNext();
            delete head;
            size_--;
        }

        void deleteLast() override {
            SinglyNode<T> *node = head_;
            while (node->getNext() != tail_) {
                node = node->getNext();
            }
            node->setNext(tail_->getNext());
            delete tail_;
            tail_ = node;
            size_--;
        }

        SinglyNode<T> *getHead() override {
            return head_;
        }

        SinglyNode<T> *getTail() override {
            return tail_;
        }
    };

    void CircularSinglyLinkedListTest() {
        int n, data;
        std::cin >> n;
        CircularSinglyLinkedList<int> linkedList;
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

#endif //ALGORITHMS_CIRCULARSINGLYLINKEDLIST_H
