#ifndef ALGORITHMS_SINGLYLINKEDLIST_H
#define ALGORITHMS_SINGLYLINKEDLIST_H

#include "../Exception.h"
#include "LinkedList.h"
#include "Node.h"
#include <iostream>

namespace dsa {
void SinglyLinkedListTest();

template <typename T> class SinglyLinkedList : public LinkedList<T, SinglyNode<T>> {
  private:
    int size_ = 0;
    SinglyNode<T> *head_;
    SinglyNode<T> *tail_;

  public:
    class Iterator {
      private:
        const SinglyNode<T> *currNode_;

      public:
        explicit Iterator(const SinglyNode<T> *pNode) noexcept : currNode_(pNode) {}

        Iterator &operator++() {
            if (currNode_)
                currNode_ = currNode_->getNext();
            return *this;
        }

        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }

        bool operator!=(const Iterator &iterator) const { return currNode_ != iterator.currNode_; }
        bool operator==(const Iterator &iterator) const { return currNode_ == iterator.currNode_; }

        T operator*() { return currNode_->getData(); }
    };

  public:
    SinglyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {}

    virtual ~SinglyLinkedList() { clear(); }

    void clear() {
        SinglyNode<T> *node = head_;
        while (node != nullptr) {
            SinglyNode<T> *next = node->getNext();
            delete node;
            node = nullptr;
            node = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        node = nullptr;
        size_ = 0;
    }

    int size() override { return size_; }

    bool isEmpty() override { return size() == 0; }

    void add(const T &data) override { addLast(data); }

    // Add element to the end of the list, O(1)
    void addLast(const T &data) override {
        if (isEmpty()) {
            head_ = new SinglyNode<T>(data, nullptr);
            tail_ = head_;
        } else {
            tail_->setNext(new SinglyNode<T>(data, nullptr));
            tail_ = tail_->getNext();
        }
        size_++;
    }

    // Add element to the beginning of the list, O(1)
    void addFirst(const T &data) override {
        if (isEmpty()) {
            head_ = new SinglyNode<T>(data, nullptr);
            tail_ = head_;
        } else {
            auto *first = new SinglyNode<T>(data, head_);
            head_ = first;
        }
        size_++;
    }

    void addAtIndex(const int &index, const T &data) override {
        if (index < 0 || index > size()) {
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
        for (int i = 1; i < index; i++) {
            node = node->getNext();
        }
        auto *newNode = new SinglyNode<T>(data, node->getNext());
        node->setNext(newNode);
        size_++;
    }

    void printList() override { std::cout << toString() << "\n"; }

    void deleteFirst() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        SinglyNode<T> *node = head_;
        head_ = node->getNext();
        delete node;
        size_--;
    }

    void deleteLast() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        SinglyNode<T> *node = head_;
        SinglyNode<T> *previous = node;
        while (node != tail_) {
            previous = node;
            node = node->getNext();
        }

        previous->setNext(nullptr);
        tail_ = previous;
        delete node;
        size_--;
    }

    void deleteNode(const T &data) override {
        if (isEmpty()) {
            return;
        }

        while (head_ != nullptr && head_->getData() == data) {
            deleteFirst();
        }

        if (head_ == nullptr) {
            return;
        }

        while (head_ != tail_ && tail_->getData() == data) {
            deleteLast();
        }

        SinglyNode<T> *node = head_;
        while (node != tail_) {
            SinglyNode<T> *next = node->getNext();
            if (next->getData() == data) {
                node->setNext(next->getNext());
                delete next;
                --size_;
                continue;
            }

            node = next;
        }
    }

    void deleteNodeAtIndex(const int &index) override {
        if (index < 0 || index > size() - 1) {
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

        SinglyNode<T> *node = head_;
        SinglyNode<T> *previous = nullptr;
        for (int i = 0; node != nullptr && i < index; i++) {
            previous = node;
            node = node->getNext();
        }

        if (node == nullptr || node->getNext() == nullptr) {
            return;
        }

        SinglyNode<T> *next = node->getNext();
        previous->setNext(next);
        delete node;
        size_--;
    }

    void findByIndex(const int &index) override {
        if (index < 0 || index > size() - 1) {
            throw InvalidArgument("Illegal index");
        }

        SinglyNode<T> *node = head_;
        for (int i = 0; i < index; ++i) {
            node = node->getNext();
        }
        std::cout << node->getData() << std::endl;
    }

    SinglyNode<T> *getHead() override { return head_; }

    SinglyNode<T> *getTail() override { return tail_; }

    Iterator begin() { return Iterator(head_); }

    Iterator end() { return Iterator(nullptr); }

    std::string toString() const {
        std::stringstream os;
        os << "[ ";
        SinglyNode<T> *traversal = head_;
        while (traversal != nullptr) {
            os << traversal->getData();
            traversal = traversal->getNext();
            if (traversal)
                os << ", ";
        }
        os << " ]";
        return os.str();
    }

    friend std::ostream &operator<<(std::ostream &strm, const SinglyLinkedList<T> &a) { return strm << a.toString(); }
};

void SinglyLinkedListTest() {
    int n, data;
    std::cout << "Number of values: ";
    std::cin >> n;
    SinglyLinkedList<int> linkedList;
    std::cout << "Enter " << n << " values: ";
    for (int i = 0; i < n; i++) {
        std::cin >> data;
        linkedList.add(data);
    }
    linkedList.printList();
    int index;

    std::cout << "Add new value at specific index (index value): ";
    std::cin >> index >> data;
    linkedList.addAtIndex(index, data);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;

    std::cout << "Add new value to first node: ";
    std::cin >> data;
    linkedList.addFirst(data);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;

    std::cout << "Add new value to last node: ";
    std::cin >> data;
    linkedList.addLast(data);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;

    std::cout << "Find value at specific index: ";
    std::cin >> index;
    linkedList.findByIndex(index);

    std::cout << "Delete last node: ";
    linkedList.deleteLast();
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;

    std::cout << "Delete first node: ";
    linkedList.deleteFirst();
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;

    std::cout << "Delete node by specific value: ";
    std::cin >> data;
    linkedList.deleteNode(data);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;

    std::cout << "Delete node by specific index: ";
    std::cin >> index;
    linkedList.deleteNodeAtIndex(index);
    linkedList.printList();
    std::cout << "Size: " << linkedList.size() << std::endl;
}
} // namespace dsa

#endif // ALGORITHMS_SINGLYLINKEDLIST_H
