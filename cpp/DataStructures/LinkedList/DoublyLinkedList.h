#ifndef ALGORITHMS_DOUBLYLINKEDLIST_H
#define ALGORITHMS_DOUBLYLINKEDLIST_H

#include <iostream>
#include "Node.h"
#include "LinkedList.h"
#include "../Exception.h"

namespace dsa {
    void DoublyLinkedListTest();

    template<typename T>
    class DoublyLinkedList : public LinkedList<T>{
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
                DoublyNode<T> *next = node->getNext();
                delete node;
                node = next;
            }
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
        }

        int size() override{
            return size_;
        }

        bool isEmpty() override{
            return size() == 0;
        }

        void add(const T &data)override {
            addLast(data);
        }

        void addFirst(const T &data)override {
            if (isEmpty()) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                tail_ = head_;
            } else {
                auto *first = new DoublyNode<T>(data, nullptr, head_);
                head_->setPrevious(first);
                head_ = first;
            }
            size_++;
        }

        void addLast(const T &data) override{
            if (isEmpty()) {
                head_ = new DoublyNode<T>(data, nullptr, nullptr);
                tail_ = head_;
            } else {
                tail_->setNext(new DoublyNode<T>(data, tail_, nullptr));
                tail_ = tail_->getNext();
            }
            size_++;
        }

        void addAtIndex(const int &index, const T &data)override {
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
                node = node->getNext();
            }
            DoublyNode<T> *testNode = node->getNext();
            auto *newNode = new DoublyNode<T>(data, node, node->getNext());
            node->getNext()->setPrevious(newNode);
            node->setNext(newNode);
            size_++;
        }

        void printList() override{
            DoublyNode<T> *node = head_;
            while (node != nullptr) {
                std::cout << node->getData() << " ";
                node = node->getNext();
            }
            std::cout << std::endl;
        }

        void deleteFirst()override {
            DoublyNode<T> *head = head_;
            head_ = head->getNext();
            head_->setPrevious(nullptr);
            delete head;
            size_--;
        }

        void deleteLast()override {
            DoublyNode<T> *tail = tail_;
            tail_ = tail->getPrevious();
            tail_->setNext(nullptr);
            delete tail;
            size_--;
        }

        void deleteNode(const T &data) override{
            if (head_->getData() == data) {
                deleteFirst();
            }

            if (tail_->getData() == data) {
                deleteLast();
            }

            DoublyNode<T> *node = head_;
            while (node != nullptr) {
                if (node->getData() != data) {
                    node = node->getNext();
                    continue;
                }

                DoublyNode<T> *next = node->getNext();
                node->getPrevious()->setNext(next);
                if (next != nullptr) {
                    next->setPrevious(node->getPrevious());
                }

                delete node;
                node = next;
                size_--;
            }
        }

        void deleteNodeAtIndex(const int &index) override{
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

            DoublyNode<T> *node = head_->getNext();
            for (int i = 1; node != nullptr && i < index - 2; i++) {
                node = node->getNext();
            }
            if (node == nullptr || node->getNext() == nullptr) {
                return;
            }

            DoublyNode<T> *next = node->getNext()->getNext();
            next->setPrevious(node);
            delete node->getNext();
            node->setNext(next);
            size_--;
        }

        void findByIndex(const int &index)override {
            if (index < 0 || index > size()) {
                throw InvalidArgument("Illegal index");
            }

            DoublyNode<T> *node = head_;
            for (int i = 0; i < index - 1; ++i) {
                node = node->getNext();
            }
            std::cout << node->getData() << std::endl;
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
