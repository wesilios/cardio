#include <iostream>
#include <sstream>

#ifndef ALGORITHMS_SINGLYLINKEDLIST_H
#define ALGORITHMS_SINGLYLINKEDLIST_H

template<class T>
class SinglyLinkedList;


template<typename T>
class SinglyLinkedList {
public:
    class Node {
    private:
        T data;
        Node *next;

        friend class SinglyLinkedList<T>;

    public:
        Node() : next(nullptr) {
        }

        Node(const T &data, Node *next) : data(data), next(next) {
        }

        std::string toString() const {
            std::stringstream ss;
            ss << "Node(" << data << ")";
            return ss.str();
        }
    };

private:
    int _size = 0;
    Node *_head;
    Node *_tail;

public:
    SinglyLinkedList() : _size(0), _head(nullptr), _tail(nullptr) {}

    virtual ~SinglyLinkedList() {
        clear();
    }

    void clear() {
        Node *temp = _head;
        while (temp != nullptr) {
            Node *next = temp->next;
            delete temp;
            temp = nullptr;
            temp = next;
        }
        _head = nullptr;
        _tail = nullptr;
        temp = nullptr;
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

    // Add element to the end of the list, O(1)
    void addLast(const T &data) {
        if (isEmpty()) {
            _head = new Node(data, nullptr);
            _tail = _head;
        } else {
            _tail->next = new Node(data, nullptr);
            _tail = _tail->next;
        }
        _size++;
    }

    // Add element to the beginning of the list, O(1)
    void addFirst(const T &data) {
        if (isEmpty()) {
            _head = new Node(data, nullptr);
            _tail = _head;
        } else {
            Node first = new Node(data, _head);
            _head = first;
        }
        _size++;
    }

    void addAt(int index, const T &data) {
        if (index < 0) {
            throw ("Illegal index");
        }
        if (index == 0) {
            addFirst(data);
            return;
        }

        if (index == size()) {
            addLast(data);
            return;
        }

        Node *temp = _head;
        for (int i = 1; i < index - 1; i++) {
            temp = temp->next;
        }
        Node *newNode = new Node(data, temp->next);
        temp->next = newNode;
        _size++;
    }

    void printList() {
        Node *temp = _head;
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
    }
};

void linkedListTest() {
    int n, data;
    std::cin >> n;
    SinglyLinkedList<int> linkedList;
    for (int i = 0; i < n; i++) {
        std::cin >> data;
        linkedList.add(data);
    }
    linkedList.printList();
}

#endif //ALGORITHMS_SINGLYLINKEDLIST_H
