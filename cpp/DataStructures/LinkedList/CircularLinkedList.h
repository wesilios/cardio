#include <iostream>
#include "../../Exception.h"

#ifndef ALGORITHMS_CIRCULARLINKEDLIST_H
#define ALGORITHMS_CIRCULARLINKEDLIST_H

template<typename T>
class CircularLinkedList;

template<typename T>
class CircularLinkedList {
public:
    class Node {
    private:
        T data;
        Node *next;

        friend class CircularLinkedList<T>;

    public:
        Node() : next(nullptr) {
        }

        Node(const T &data, Node *next) : data(data), next(next) {
        }
    };

private:
    int size_ = 0;
    Node *head_;
    Node *tail_;

public:
    CircularLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {
    }

    virtual ~CircularLinkedList() {
        clear();
    }

    void clear() {
        Node *node = head_;
        while (node->next != head_) {
            Node *next = node->next;
            delete node;
            node = nullptr;
            node = next;
        }
        delete head_;
        head_ = nullptr;
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

    void addLast(const T &data) {
        if (isEmpty()) {
            head_ = new Node(data, nullptr);
            head_->next = head_;
            tail_ = head_;
        } else {
            tail_->next = new Node(data, head_);
            tail_ = tail_->next;
        }
        size_++;
    }

    void addFirst(const T &data) {
        if (isEmpty()) {
            head_ = new Node(data, nullptr);
            head_->next = head_;
            tail_ = head_;
        } else {
            Node *head = new Node(data, head_);
            head_ = head;
            tail_->next = head;
        }
        size_++;
    }

    void addAtIndex(int &index, const T &data) {
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
        Node *newNode = new Node(data, node->next);
        node->next = newNode;
        size_++;
    }

    void printList() {
        Node *node = head_;
        for (int i = 0; node != nullptr && i < size(); i++) {
            std::cout << node->data << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    void findByIndex(const int &index) {
        if (index < 0 || index > size()) {
            throw InvalidArgument("Illegal index");
        }

        Node *node = head_;
        for (int i = 0; i < index - 1; i++) {
            node = node->next;
        }
        std::cout << node->data << std::endl;
    }

    void deleteNode(const T &data) {
        if (head_->data == data) {
            deleteFirst();
        }

        if (tail_->data == data) {
            deleteLast();
        }

        Node *node = head_;
        while (node != tail_) {
            if (node->next->data != data) {
                node = node->next;
                continue;
            }

            Node *next = node->next;
            node->next = next->next;
            node = node->next;
            delete next;
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

        Node *previousNode = head_;
        for (int i = 0; i < index - 2; ++i) {
            previousNode = previousNode->next;
        }

        Node *node = previousNode->next;
        previousNode->next = previousNode->next->next;
        delete node;
        size_--;
    }

    void deleteFirst() {
        Node *head = head_;
        tail_->next = head->next;
        head_ = head->next;
        delete head;
        size_--;
    }

    void deleteLast() {
        Node *node = head_;
        while (node->next != tail_) {
            node = node->next;
        }
        node->next = tail_->next;
        delete tail_;
        tail_ = node;
        size_--;
    }
};

void CircularLinkedListTests() {
    int n, data;
    std::cin >> n;
    CircularLinkedList<int> linkedList;
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

#endif //ALGORITHMS_CIRCULARLINKEDLIST_H
