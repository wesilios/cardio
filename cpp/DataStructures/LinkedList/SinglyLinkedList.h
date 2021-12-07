#include <iostream>
#include <sstream>
#include "Node.h"
#include "../../Exception.h"

#ifndef ALGORITHMS_SINGLYLINKEDLIST_H
#define ALGORITHMS_SINGLYLINKEDLIST_H

void SinglyLinkedListTest();

template<typename T>
class SinglyLinkedList {
private:
    int size_ = 0;
    SinglyNode<T> *head_;
    SinglyNode<T> *tail_;

    void deleteNode(SinglyNode<T> *&node, const T &data) {
        if (node->data == data) {
            SinglyNode<T> *temp = node;
            node = node->next;
            delete temp;
            size_--;
            return;
        }
        deleteNode(node->next, data);
    }

public:
    SinglyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {}

    virtual ~SinglyLinkedList() {
        clear();
    }

    void clear() {
        SinglyNode<T> *node = head_;
        while (node != nullptr) {
            SinglyNode<T> *next = node->next;
            delete node;
            node = nullptr;
            node = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        node = nullptr;
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

    // Add element to the end of the list, O(1)
    void addLast(const T &data) {
        if (isEmpty()) {
            head_ = new SinglyNode<T>(data, nullptr);
            tail_ = head_;
        } else {
            tail_->next = new SinglyNode<T>(data, nullptr);
            tail_ = tail_->next;
        }
        size_++;
    }

    // Add element to the beginning of the list, O(1)
    void addFirst(const T &data) {
        if (isEmpty()) {
            head_ = new SinglyNode<T>(data, nullptr);
            tail_ = head_;
        } else {
            auto *first = new SinglyNode<T>(data, head_);
            head_ = first;
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

        SinglyNode<T> *node = head_;
        for (int i = 1; i < index - 1; i++) {
            node = node->next;
        }
        auto *newNode = new SinglyNode<T>(data, node->next);
        node->next = newNode;
        size_++;
    }

    void printList() {
        SinglyNode<T> *node = head_;
        while (node != nullptr) {
            std::cout << node->data << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    void deleteFirst() {
        SinglyNode<T> *node = head_;
        head_ = node->next;
        delete node;
        size_--;
    }

    void deleteLast() {
        SinglyNode<T> *node = head_;
        SinglyNode<T> *previous;
        for (int i = 0; i < size() - 1; i++) {
            previous = node;
            node = node->next;
        }
        previous->next = nullptr;
        tail_ = previous;
        delete node;
        size_--;
    }

    void deleteNode(const T &data) {
        deleteNode(head_, data);
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

        SinglyNode<T> *node = head_;
        for (int i = 0; node != nullptr && i < index - 2; i++) {
            node = node->next;
        }
        if (node == nullptr || node->next == nullptr) {
            return;
        }

        SinglyNode<T> *next = node->next->next;
        delete node->next;
        node->next = next;
        size_--;
    }

    void findByIndex(const int &index) {
        if (index < 0 || index > size()) {
            throw InvalidArgument("Illegal index");
        }

        SinglyNode<T> *node = head_;
        for (int i = 0; i < index - 1; ++i) {
            node = node->next;
        }
        std::cout << node->data << std::endl;
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

#endif //ALGORITHMS_SINGLYLINKEDLIST_H
