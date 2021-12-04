//
// Created by Admin on 12/4/2021.
//

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
};

#endif //ALGORITHMS_DOUBLYLINKEDLIST_H
