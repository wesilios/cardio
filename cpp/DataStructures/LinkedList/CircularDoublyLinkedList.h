#ifndef ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H
#define ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H

#include "../Exception.h"
#include "LinkedList.h"
#include "Node.h"
#include <cstddef>
#include <iostream>

namespace dsa {
template <typename T> class CircularDoublyLinkedList : public LinkedList<T, DoublyNode<T>> {
  private:
    std::size_t size_ = 0;
    DoublyNode<T> *head_;
    DoublyNode<T> *tail_;

    void swap(CircularDoublyLinkedList<T> &other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

  public:
    class Iterator {
      private:
        const DoublyNode<T> *currNode_;
        int remaining_;

      public:
        explicit Iterator(const DoublyNode<T> *pNode, const int remaining) noexcept
            : currNode_(pNode), remaining_(remaining) {}

        Iterator &operator++() {
            if (remaining_ > 0) {
                currNode_ = currNode_->getNext();
                --remaining_;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }

        bool operator!=(const Iterator &iterator) const { return remaining_ != iterator.remaining_; }
        bool operator==(const Iterator &iterator) const { return remaining_ == iterator.remaining_; }

        const T &operator*() { return currNode_->getData(); }
    };

  public:
    CircularDoublyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {}

    // Rule of Three
    CircularDoublyLinkedList(const CircularDoublyLinkedList<T> &other) : CircularDoublyLinkedList() {
        for (const auto &value : other) {
            addLast(value);
        }
    }

    CircularDoublyLinkedList &operator=(const CircularDoublyLinkedList<T> &other) {
        if (this != &other) {
            CircularDoublyLinkedList<T> temp(other);
            swap(temp);
        }

        return *this;
    }

    ~CircularDoublyLinkedList() { clear(); }

    void clear() override {
        DoublyNode<T> *node = head_;

        for (int i = 0; i < size_; ++i) {
            DoublyNode<T> *next = node->getNext();
            delete node;
            node = next;
        }

        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const override { return size_; }

    bool isEmpty() const override { return size() == 0; }

    void add(const T &data) override { addLast(data); }

    void addLast(const T &data) override {
        if (isEmpty()) {
            head_ = new DoublyNode<T>(data, nullptr, nullptr);
            head_->setNext(head_);
            head_->setPrevious(head_);
            tail_ = head_;
            size_++;
            return;
        }

        auto *node = new DoublyNode<T>(data, tail_, head_);
        tail_->setNext(node);
        head_->setPrevious(node);
        tail_ = node;
        size_++;
    }

    void addFirst(const T &data) override {
        if (isEmpty()) {
            head_ = new DoublyNode<T>(data, nullptr, nullptr);
            head_->setNext(head_);
            head_->setPrevious(head_);
            tail_ = head_;
            size_++;
            return;
        }

        auto *node = new DoublyNode<T>(data, tail_, head_);
        tail_->setNext(node);
        head_->setPrevious(node);
        head_ = node;
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

        DoublyNode<T> *node = head_;
        for (int i = 0; i < index; ++i) {
            node = node->getNext();
        }

        auto *previous = node->getPrevious();
        auto *newNode = new DoublyNode<T>(data, previous, node);
        previous->setNext(newNode);
        node->setPrevious(newNode);
        size_++;
    }

    void deleteFirst() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        DoublyNode<T> *currentHead = head_;
        DoublyNode<T> *next = currentHead->getNext();
        next->setPrevious(tail_);
        tail_->setNext(next);
        head_ = next;
        delete currentHead;
        size_--;
    }

    void deleteLast() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        DoublyNode<T> *currentTail = tail_;
        DoublyNode<T> *previous = currentTail->getPrevious();
        previous->setNext(head_);
        head_->setPrevious(previous);
        tail_ = previous;
        delete currentTail;
        size_--;
    }

    void deleteNode(const T &data) override {
        if (isEmpty()) {
            return;
        }

        DoublyNode<T> *node = head_;
        int count = size();
        while (count > 0) {
            DoublyNode<T> *next = node->getNext();
            if (node->getData() == data) {
                if (node == head_) {
                    deleteFirst();
                } else if (node == tail_) {
                    deleteLast();
                } else {
                    DoublyNode<T> *previous = node->getPrevious();

                    previous->setNext(next);
                    next->setPrevious(previous);

                    delete node;
                    --size_;
                }
            }

            node = next;
            count--;
        }
    }

    void deleteNodeAtIndex(const int &index) override {
        if (index < 0 || static_cast<std::size_t>(index) >= size()) {
            throw InvalidArgument("Illegal index");
        }

        if (index == 0) {
            deleteFirst();
            return;
        }

        if (static_cast<std::size_t>(index) == size() - 1) {
            deleteLast();
            return;
        }

        DoublyNode<T> *node = head_;
        for (int i = 0; i < index; i++) {
            node = node->getNext();
        }

        DoublyNode<T> *next = node->getNext();
        DoublyNode<T> *previous = node->getPrevious();
        next->setPrevious(previous);
        previous->setNext(next);
        delete node;
        size_--;
    }

    void printList() const override {
        DoublyNode<T> *node = head_;
        for (int i = 0; node != nullptr && i < size(); ++i) {
            std::cout << node->getData() << " ";
            node = node->getNext();
        }
        std::cout << std::endl;
    }

    void findByIndex(const int &index) const override {
        if (index < 0 || static_cast<std::size_t>(index) >= size()) {
            throw InvalidArgument("Illegal index");
        }

        DoublyNode<T> *node = head_;
        for (int i = 0; i < index; ++i) {
            node = node->getNext();
        }
        std::cout << node->getData() << std::endl;
    }

    DoublyNode<T> *getHead() const override { return head_; }

    DoublyNode<T> *getTail() const override { return tail_; }

    Iterator begin() const { return Iterator(head_, size_); }

    Iterator end() const { return Iterator(head_, 0); }

    std::string toString() const override {
        std::stringstream os;
        os << "[ ";
        DoublyNode<T> *traversal = head_;
        std::size_t size = size_;
        while (size > 0) {
            os << traversal->getData();
            traversal = traversal->getNext();
            --size;
            if (size > 0)
                os << ", ";
        }
        os << " ]";
        return os.str();
    }

    friend std::ostream &operator<<(std::ostream &strm, const CircularDoublyLinkedList<T> &a) {
        return strm << a.toString();
    }
};

void CircularDoublyLinkedListTest() {
    int n, data;
    std::cout << "Number of values: ";
    std::cin >> n;
    CircularDoublyLinkedList<int> linkedList;
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

#endif // ALGORITHMS_CIRCULARDOUBLYLINKEDLIST_H
