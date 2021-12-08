#ifndef ALGORITHMS_DOUBLYLINKEDLIST_H
#define ALGORITHMS_DOUBLYLINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "../Exception.h"

namespace dsa {
    void DoublyLinkedListTest();

    template<typename T>
    class DoublyLinkedList {
    private:
        int size_ = 0;
        DoublyNode <T> *head_, *tail_;

    public:
        DoublyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {
        }

        virtual ~DoublyLinkedList() {
            clear();
        }

        void clear() {
            DoublyNode<T> *node = head_;
            while (node != nullptr) {
                DoublyNode<T> *next = node->next;
                delete node;
                node = next;
            }
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

        void add(const T &data) {
            addLast(data);
        }

        void addFirst(const T &data) {
            if (isEmpty()) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                tail_ = head_;
            } else {
                auto *first = new DoublyNode<T>(data, nullptr, head_);
                head_->previous = first;
                head_ = first;
            }
            size_++;
        }

        void addLast(const T &data) {
            if (isEmpty()) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                tail_ = head_;
            } else {
                tail_->next = new DoublyNode<T>(data, tail_, nullptr);
                tail_ = tail_->next;
            }
            size_++;
        }

        void addAtIndex(int index, const T &data) {
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

            DoublyNode<T> *node = head_;
            for (int i = 1; i < index - 1; i++) {
                node = node->next;
            }
            auto *newNode = new DoublyNode<T>(data, node, node->next);
            node->next->previous = newNode;
            node->next = newNode;
            size_++;
        }

        void printList() {
            DoublyNode<T> *node = head_;
            while (node != nullptr) {
                std::cout << node->data << " ";
                node = node->next;
            }
            std::cout << std::endl;
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

            DoublyNode<T> *node = head_;
            while (node != nullptr) {
                if (node->data != data) {
                    node = node->next;
                    continue;
                }

                DoublyNode<T> *next = node->next;
                node->previous->next = next;
                if (next != nullptr) {
                    next->previous = node->previous;
                }

                delete node;
                node = next;
                size_--;
            }
        }

        void deleteNodeAtIndex(const int &index) {
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

            DoublyNode<T> *node = head_->next;
            for (int i = 1; node != nullptr && i < index - 2; i++) {
                node = node->next;
            }
            if (node == nullptr || node->next == nullptr) {
                return;
            }

            DoublyNode<T> *next = node->next->next;
            next->previous = node;
            delete node->next;
            node->next = next;
            size_--;
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
    };

    void DoublyLinkedListTest() {
        int n, data;
        std::cin >> n;
        DoublyLinkedList<int> linkedList;
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

#endif //ALGORITHMS_DOUBLYLINKEDLIST_H
