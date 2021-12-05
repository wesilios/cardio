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
};

#endif //ALGORITHMS_CIRCULARLINKEDLIST_H
