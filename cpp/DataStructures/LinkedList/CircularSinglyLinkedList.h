#ifndef ALGORITHMS_CIRCULARSINGLYLINKEDLIST_H
#define ALGORITHMS_CIRCULARSINGLYLINKEDLIST_H

#include "../Exception.h"
#include "LinkedList.h"
#include "Node.h"
#include <cstddef>
#include <iostream>

namespace dsa {
void CircularSinglyLinkedListTest();

template <typename T> class CircularSinglyLinkedList : public LinkedList<T, SinglyNode<T>> {
  private:
    std::size_t size_ = 0;
    SinglyNode<T> *head_;
    SinglyNode<T> *tail_;

    void swap(CircularSinglyLinkedList<T> &other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

  public:
    class Iterator {
      private:
        const SinglyNode<T> *currNode_;
        int remaining_;

      public:
        explicit Iterator(const SinglyNode<T> *pNode, int remaining) noexcept
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

        const T &operator*() const { return currNode_->getData(); }
    };

  public:
    CircularSinglyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {}

    // Rule of Three
    CircularSinglyLinkedList(const CircularSinglyLinkedList<T> &other) : CircularSinglyLinkedList() {
        for (const auto &value : other) {
            addLast(value);
        }
    }

    CircularSinglyLinkedList &operator=(const CircularSinglyLinkedList<T> &other) {
        if (this != &other) {
            CircularSinglyLinkedList<T> temp(other);
            swap(temp);
        }

        return *this;
    }

    // Rule of Five
    CircularSinglyLinkedList(CircularSinglyLinkedList<T> &&other) noexcept
        : size_(other.size()), head_(other.getHead()), tail_(other.getTail()) {
        other.size_ = 0;
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }

    CircularSinglyLinkedList &operator=(CircularSinglyLinkedList<T> &&other) noexcept {
        CircularSinglyLinkedList<T> temp(std::move(other));
        swap(temp);
        return *this;
    }

    ~CircularSinglyLinkedList() { clear(); }

    void clear() override {
        SinglyNode<T> *node = head_;

        for (int i = 0; i < size_; ++i) {
            SinglyNode<T> *next = node->getNext();
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

    void addFirst(const T &data) override {
        if (isEmpty()) {
            head_ = new SinglyNode<T>(data, nullptr);
            head_->setNext(head_);
            tail_ = head_;
        } else {
            auto *head = new SinglyNode<T>(data, head_);
            head_ = head;
            tail_->setNext(head);
        }
        size_++;
    }

    void addLast(const T &data) override {
        if (isEmpty()) {
            head_ = new SinglyNode<T>(data, nullptr);
            head_->setNext(head_);
            tail_ = head_;
        } else {
            tail_->setNext(new SinglyNode<T>(data, head_));
            tail_ = tail_->getNext();
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
        auto count = size() - 1;
        while (count > 0) {
            SinglyNode<T> *next = node->getNext();
            --count;
            if (next->getData() == data) {
                node->setNext(next->getNext());
                delete next;
                --size_;
                continue;
            }

            node = node->getNext();
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

        SinglyNode<T> *node = head_;
        SinglyNode<T> *previous = nullptr;
        for (int i = 0; i < index; ++i) {
            previous = node;
            node = node->getNext();
        }

        SinglyNode<T> *next = node->getNext();
        previous->setNext(next);
        delete node;
        size_--;
    }

    void deleteFirst() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        SinglyNode<T> *head = head_;
        tail_->setNext(head->getNext());
        head_ = head->getNext();
        delete head;
        size_--;
    }

    void deleteLast() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        SinglyNode<T> *node = head_;
        auto count = size() - 1;
        while (count > 1) {
            node = node->getNext();
            --count;
        }
        node->setNext(tail_->getNext());
        delete tail_;
        tail_ = node;
        size_--;
    }

    void printList() const override {
        SinglyNode<T> *node = head_;
        for (std::size_t i = 0; node != nullptr && i < size(); i++) {
            std::cout << node->getData() << " ";
            node = node->getNext();
        }
        std::cout << std::endl;
    }

    void findByIndex(const int &index) const override {
        if (index < 0 || static_cast<std::size_t>(index) >= size()) {
            throw InvalidArgument("Illegal index");
        }

        SinglyNode<T> *node = head_;
        for (int i = 0; i < index; i++) {
            node = node->getNext();
        }
        std::cout << node->getData() << std::endl;
    }

    SinglyNode<T> *getHead() const override { return head_; }

    SinglyNode<T> *getTail() const override { return tail_; }

    Iterator begin() const { return Iterator(head_, size_); }

    Iterator end() const { return Iterator(head_, 0); }

    std::string toString() const override {
        std::stringstream os;
        os << "[ ";
        SinglyNode<T> *traversal = head_;
        auto count = size();
        while (count > 0) {
            os << traversal->getData();
            traversal = traversal->getNext();
            count--;
            if (count > 0)
                os << ", ";
        }
        os << " ]";
        return os.str();
    }

    friend std::ostream &operator<<(std::ostream &strm, const CircularSinglyLinkedList<T> &a) {
        return strm << a.toString();
    }
};

void CircularSinglyLinkedListTest() {
    int n, data;
    std::cout << "Number of values: ";
    std::cin >> n;
    CircularSinglyLinkedList<int> linkedList;
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

#endif // ALGORITHMS_CIRCULARSINGLYLINKEDLIST_H
