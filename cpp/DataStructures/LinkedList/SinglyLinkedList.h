#ifndef ALGORITHMS_SINGLYLINKEDLIST_H
#define ALGORITHMS_SINGLYLINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "LinkedList.h"
#include "../Exception.h"

namespace dsa {
    void SinglyLinkedListTest();

    template<typename T>
    class SinglyLinkedList : public LinkedList<T, SinglyNode<T>> {
    private:
        int size_ = 0;
        SinglyNode<T> *head_;
        SinglyNode<T> *tail_;

    public:
        SinglyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {}

        virtual ~SinglyLinkedList() {
            clear();
        }

        void clear() {
            SinglyNode<T> *node = head_;
            while (node != nullptr) {
                SinglyNode<T> *next = node->getNext();
                delete node;
                node = nullptr;
                node = next;
            }
            head_ = nullptr;
            tail_ = nullptr;
            node = nullptr;
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

        // Add element to the end of the list, O(1)
        void addLast(const T &data) override {
            if (isEmpty()) {
                head_ = new SinglyNode<T>(data, nullptr);
                tail_ = head_;
            } else {
                tail_->setNext(new SinglyNode<T>(data, nullptr));
                tail_ = tail_->getNext();
            }
            size_++;
        }

        // Add element to the beginning of the list, O(1)
        void addFirst(const T &data) override {
            if (isEmpty()) {
                head_ = new SinglyNode<T>(data, nullptr);
                tail_ = head_;
            } else {
                auto *first = new SinglyNode<T>(data, head_);
                head_ = first;
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
            while (node != nullptr) {
                std::cout << node->getData() << " ";
                node = node->getNext();
            }
            std::cout << std::endl;
        }

        void deleteFirst() override {
            SinglyNode<T> *node = head_;
            head_ = node->getNext();
            delete node;
            size_--;
        }

        void deleteLast() override {
            SinglyNode<T> *node = head_;
            SinglyNode<T> *previous;
            for (int i = 0; i < size() - 1; i++) {
                previous = node;
                node = node->getNext();
            }
            previous->setNext(nullptr);
            tail_ = previous;
            delete node;
            size_--;
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
            if (index < 0) {
                throw InvalidArgument("Illegal index");
            }

            if (index == 0) {
                deleteFirst();
                return;
            }

            if (index == size()) {
                deleteLast();
                return;
            }

            SinglyNode<T> *node = head_;
            for (int i = 0; node != nullptr && i < index - 2; i++) {
                node = node->getNext();
            }
            if (node == nullptr || node->getNext() == nullptr) {
                return;
            }

            SinglyNode<T> *next = node->getNext()->getNext();
            delete node->getNext();
            node->setNext(next);
            size_--;
        }

        void findByIndex(const int &index) override {
            if (index < 0 || index > size()) {
                throw InvalidArgument("Illegal index");
            }

            SinglyNode<T> *node = head_;
            for (int i = 0; i < index - 1; ++i) {
                node = node->getNext();
            }
            std::cout << node->getData() << std::endl;
        }

        SinglyNode<T> *getHead() override {
            return head_;
        }

        SinglyNode<T> *getTail() override {
            return tail_;
        }
    };

    void SinglyLinkedListTest() {
        int n, data;
        std::cin >> n;
        SinglyLinkedList<int> linkedList;
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

#endif //ALGORITHMS_SINGLYLINKEDLIST_H
