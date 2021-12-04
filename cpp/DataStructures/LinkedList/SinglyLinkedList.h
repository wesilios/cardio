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

    void deleteNode(Node *&node, const T &value) {
        if (node->data == value) {
            Node *temp = node;
            node = node->next;
            delete temp;
            _size--;
            return;
        }
        deleteNode(node->next, value);
    }

public:
    SinglyLinkedList() : _size(0), _head(nullptr), _tail(nullptr) {}

    virtual ~SinglyLinkedList() {
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
            Node *first = new Node(data, _head);
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

        Node *node = _head;
        for (int i = 1; i < index - 1; i++) {
            node = node->next;
        }
        Node *newNode = new Node(data, node->next);
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
        Node *node = _head;
        _head = node->next;
        delete node;
        _size--;
    }

    void deleteLast() {
        Node *node = _head;
        Node *previous;
        for (int i = 0; i < size(); i++) {
            previous = node;
            node = node->next;
        }
        _tail = previous;
        delete node;
        _size--;
    }

    void deleteNode(const int &value) {
        deleteNode(_head, value);
    }

    void deleteNodeAtPosition(const int &index) {
        if (index < 0) {
            throw ("Illegal index");
        }

        if (index == 0) {
            deleteFirst();
        }

        if (index == size()) {
            deleteLast();
        }

        Node *node = _head;
        for (int i = 1; i < index; i++) {
            node = node->next;
        }
        deleteNode(_head, node->data);
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
    linkedList.addAt(index, data);
    linkedList.printList();
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
    std::cin >> index;
    linkedList.deleteNodeAtPosition(index);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;
}

#endif //ALGORITHMS_SINGLYLINKEDLIST_H
