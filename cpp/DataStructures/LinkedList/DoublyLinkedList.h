#include <iostream>
#include "../../Exception.h"

#ifndef ALGORITHMS_DOUBLYLINKEDLIST_H
#define ALGORITHMS_DOUBLYLINKEDLIST_H

template<typename T>
class DoublyLinkedList;

template<typename T>
class DoublyLinkedList {
public:
    class Node {
    private:
        T data;
        Node *previous, *next;

        friend class DoublyLinkedList<T>;

    public:
        Node() : previous(nullptr), next(nullptr) {
        }

        Node(T data, Node *previous, Node *next) : data(data), previous(previous), next(next) {
        }
    };

private:
    int size_ = 0;
    Node *head_, *tail_;

public:
    DoublyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {
    }

    virtual ~DoublyLinkedList() {
        clear();
    }

    void clear() {
        Node *node = head_;
        while (node != nullptr) {
            Node *next = node->next;
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

    void addFirst(const T &data) {
        if (isEmpty()) {
            head_ = new Node(data, nullptr, nullptr);
            tail_ = head_;
        } else {
            Node *first = new Node(data, nullptr, head_);
            head_->previous = first;
            head_ = first;
        }
        size_++;
    }

    void addLast(const T &data) {
        if (isEmpty()) {
            head_ = new Node(data, nullptr, nullptr);
            tail_ = head_;
        } else {
            tail_->next = new Node(data, tail_, nullptr);
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

        Node *node = head_;
        for (int i = 1; i < index - 1; i++) {
            node = node->next;
        }
        Node *newNode = new Node(data, node, node->next);
        node->next = newNode;
        size_++;
    }

    void printList() {
        Node *node = head_;
        while (node != nullptr) {
            std::cout << node->data << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    void deleteFirst() {
        Node *head = head_;
        head_ = head->next;
        head_->previous = nullptr;
        delete head;
        size_--;
    }

    void deleteLast() {
        Node *tail = tail_;
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

        Node *node = head_;
        while (node != nullptr) {
            if (node->data != data) {
                node = node->next;
                continue;
            }

            Node *next = node->next;
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

        Node *node = head_->next;
        for (int i = 1; node != nullptr && i < index - 2; i++) {
            node = node->next;
        }
        if (node == nullptr || node->next == nullptr) {
            return;
        }

        Node *next = node->next->next;
        next->previous = node;
        delete node->next;
        node->next = next;
        size_--;
    }

    void findByIndex(const int &index) {
        if (index < 0 || index > size()) {
            throw InvalidArgument("Illegal index");
        }

        Node *node = head_;
        for (int i = 0; i < index - 1; ++i) {
            node = node->next;
        }
        std::cout << node->data << std::endl;
    }
};

void DoublyLinkedListTests() {
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

#endif //ALGORITHMS_DOUBLYLINKEDLIST_H
