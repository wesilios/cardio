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
    int _size = 0;
    Node *_head, *_tail;

public:
    DoublyLinkedList() : _size(0), _head(nullptr), _tail(nullptr) {
    }

    virtual ~DoublyLinkedList() {
        clear();
    }

    void clear() {
        Node *node = _head;
        while (node != nullptr) {
            Node *next = node->next;
            delete node;
            node = nullptr;
            node = next;
        }
        _head = nullptr;
        _tail = nullptr;
        node = nullptr;
        _size = 0;
    }

    int size() {
        return _size;
    }

    bool isEmpty() {
        return size() == 0;
    }

    void add(const T &data) {
        addLast(data);
    }

    void addFirst(const T &data) {
        if (isEmpty()) {
            _head = new Node(data, nullptr, nullptr);
            _tail = _head;
        } else {
            Node *first = new Node(data, nullptr, _head);
            _head->previous = first;
            _head = first;
        }
        _size++;
    }

    void addLast(const T &data) {
        if (isEmpty()) {
            _head = new Node(data, nullptr, nullptr);
            _tail = _head;
        } else {
            _tail->next = new Node(data, _tail, nullptr);
            _tail = _tail->next;
        }
        _size++;
    }

    void addAt(int index, const T &data) {
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
        Node *node = _head;
        for (int i = 1; i < index - 1; i++) {
            node = node->next;
        }
        Node *newNode = new Node(data, node, node->next);
        node->next = newNode;
        _size++;
    }

    void printList() {
        Node *node = _head;
        while (node != nullptr) {
            std::cout << node->data << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    void deleteFirst() {
        Node *head = _head;
        _head = head->next;
        _head->previous = nullptr;
        delete head;
        _size--;
    }

    void deleteLast() {
        Node *tail = _tail;
        _tail = tail->previous;
        _tail->next = nullptr;
        delete tail;
        _size--;
    }

    void deleteNode(const T &data) {
        if (_head->data == data) {
            deleteFirst();
        }

        if (_tail->data == data) {
            deleteLast();
        }

        Node *node = _head;
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
            _size--;
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
    }

    void findByIndex(const int &index) {
        if (index < 0 || index > size()) {
            throw InvalidArgument("Illegal index");
        }
        Node *node = _head;
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
    linkedList.addAt(index, data);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;
    std::cin >> data;
    linkedList.deleteNode(data);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;
    linkedList.deleteFirst();
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;
    linkedList.deleteLast();
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;
//    std::cin >> index;
//    linkedList.deleteNodeAtIndex(index);
//    linkedList.printList();
//    std::cout << "Size: " << linkedList.size() << std::endl;
    std::cin >> index;
    linkedList.findByIndex(index);
}

#endif //ALGORITHMS_DOUBLYLINKEDLIST_H
