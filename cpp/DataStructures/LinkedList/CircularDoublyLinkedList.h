#ifndef ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H
#define ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "../Exception.h"

namespace dsa {
    template<typename T>
    class CircularDoublyLinkedList {
    private:
        int size_ = 0;
        DoublyNode <T> *head_;
        DoublyNode <T> *tail_;

    public:
        CircularDoublyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {
        }

        virtual ~CircularDoublyLinkedList() {
            clear();
        }

        void clear() {
            DoublyNode<T> *node = head_;
            while (node->next != head_) {
                DoublyNode<T> *next = node->next;
                delete node;
                node = next;
            }
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
        }

        int size() {
            return size_;
        }

        bool isEmpty() {
            return size() == 0;
        }

        void printList() {
            DoublyNode<T> *node = head_;
            for (int i = 0; node != nullptr && i < size(); ++i) {
                std::cout << node->data << " ";
                node = node->next;
            }
            std::cout << std::endl;
        }

        void add(const T &data) {
            addLast(data);
        }

        void addLast(const T &data) {
            if (isEmpty()) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                head_->next = head_;
                head_->previous = head_;
                tail_ = head_;
                size_++;
                return;
            }

            auto *node = new DoublyNode<T>(data, tail_, head_);
            tail_->next = node;
            head_->previous = node;
            tail_ = node;
            size_++;
        }

        void addFirst(const T &data) {
            if (isEmpty()) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                head_->next = head_;
                head_->previous = head_;
                tail_ = head_;
                size_++;
                return;
            }

            auto *node = new DoublyNode<T>(data, tail_, head_);
            tail_->next = node;
            head_->previous = node;
            head_ = node;
            size_++;
        }

        void addAtIndex(const int &index, const T &data) {
            if (index < 0 || index > size_) {
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
            for (int i = 0; i < index - 1; ++i) {
                node = node->next;
            }

            auto *newNode = new DoublyNode<T>(data, node, node->next);
            node->next->previous = newNode;
            node->next = newNode;
            size_++;
        }

        void findByIndex(const int &index) {
            if (index < 0 || index > size()) {
                throw InvalidArgument("Illegal index");
            }

            DoublyNode<T> *node = head_;
            for (int i = 0; i < index - 1; ++i) {
                node = node->next;
            }
            std::cout << node->data << std::endl;
        }

        void deleteFirst() {
            DoublyNode<T> *head = head_;
            head_ = head->next;
            head_->previous = nullptr;
            delete head;
            size_--;
        }

        void deleteLast() {
            DoublyNode<T> *tail = tail_;
            tail_ = tail->previous;
            tail_->next = nullptr;
            delete tail;
            size_--;
        }

        void deleteNode(const T &data) {
            if (head_->data == data) {
                deleteFirst();
            }

            if (tail_->data == data) {
                deleteLast();
            }

            DoublyNode<T> *node = head_->next;
            while (node != tail_) {
                if (node->data != data) {
                    node = node->next;
                    continue;
                }

                DoublyNode<T> *next = node->next;
                node->previous->next = node->next;
                next->previous = node->previous;
                delete node;
                node = next;
                size_--;
            }
        }

        void deleteNodeAtIndex(const int &index) {
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

            DoublyNode<T> *node = head_->next;
            for (int i = 1; i < index - 1; ++i) {
                node = node->next;
            }

            DoublyNode<T> *next = node->next;
            node->previous->next = next;
            next->next = node->previous;
            delete node;
            size_--;
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
