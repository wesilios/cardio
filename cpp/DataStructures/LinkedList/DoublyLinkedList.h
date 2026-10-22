#ifndef ALGORITHMS_DOUBLYLINKEDLIST_H
#define ALGORITHMS_DOUBLYLINKEDLIST_H

#include "../Exception.h"
#include "LinkedList.h"
#include "Node.h"
#include <iostream>

namespace dsa {
void DoublyLinkedListTest();

template <typename T> class DoublyLinkedList : public LinkedList<T, DoublyNode<T>> {
  private:
    int size_ = 0;
    DoublyNode<T> *head_, *tail_;

  public:
    class Iterator {
      private:
        const DoublyNode<T> *currNode_;

      public:
        explicit Iterator(const DoublyNode<T> *pNode) noexcept : currNode_(pNode) {}

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
    DoublyLinkedList() : size_(0), head_(nullptr), tail_(nullptr) {}

    virtual ~DoublyLinkedList() { clear(); }

    void clear() {
        DoublyNode<T> *node = head_;
        while (node != nullptr) {
            DoublyNode<T> *next = node->getNext();
            delete node;
            node = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    int size() override { return size_; }

    bool isEmpty() override { return size() == 0; }

    void add(const T &data) override { addLast(data); }

    void addFirst(const T &data) override {
        if (isEmpty()) {
            head_ = new DoublyNode<T>(data, nullptr, nullptr);
            tail_ = head_;
        } else {
            auto *first = new DoublyNode<T>(data, nullptr, head_);
            head_->setPrevious(first);
            head_ = first;
        }
        size_++;
    }

    void addLast(const T &data) override {
        if (isEmpty()) {
            head_ = new DoublyNode<T>(data, nullptr, nullptr);
            tail_ = head_;
        } else {
            tail_->setNext(new DoublyNode<T>(data, tail_, nullptr));
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

        DoublyNode<T> *node = head_;
        for (int i = 0; i < index; i++) {
            node = node->getNext();
        }

        auto *previous = node->getPrevious();
        auto *newNode = new DoublyNode<T>(data, previous, node);
        previous->setNext(newNode);
        node->setPrevious(newNode);
        size_++;
    }

    void printList() override {
        DoublyNode<T> *node = head_;
        while (node != nullptr) {
            std::cout << node->getData() << " ";
            node = node->getNext();
        }
        std::cout << std::endl;
    }

    void deleteFirst() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        DoublyNode<T> *head = head_;
        head_ = head->getNext();
        head_->setPrevious(nullptr);
        delete head;
        size_--;
    }

    void deleteLast() override {
        if (head_ == tail_) {
            clear();
            return;
        }

        DoublyNode<T> *tail = tail_;
        tail_ = tail->getPrevious();
        tail_->setNext(nullptr);
        delete tail;
        size_--;
    }

    void deleteNode(const T &data) override {
        DoublyNode<T> *node = head_;

        while (node != nullptr) {
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

        DoublyNode<T> *node = head_;
        for (int i = 0; node != nullptr && i < index; i++) {
            node = node->getNext();
        }
        if (node == nullptr || node->getNext() == nullptr) {
            return;
        }

        DoublyNode<T> *next = node->getNext();
        DoublyNode<T> *previous = node->getPrevious();
        next->setPrevious(previous);
        previous->setNext(next);
        delete node;
        size_--;
    }

    void findByIndex(const int &index) override {
        if (index < 0 || index > size() - 1) {
            throw InvalidArgument("Illegal index");
        }

        DoublyNode<T> *node = head_;
        for (int i = 0; i < index; ++i) {
            node = node->getNext();
        }
        std::cout << node->getData() << std::endl;
    }

    DoublyNode<T> *getHead() override { return head_; }

    DoublyNode<T> *getTail() override { return tail_; }

    Iterator begin() { return Iterator(head_); }

    Iterator end() { return Iterator(nullptr); }

    std::string toString() const {
        std::stringstream os;
        os << "[ ";
        DoublyNode<T> *traversal = head_;
        while (traversal != nullptr) {
            os << traversal->getData();
            traversal = traversal->getNext();
            if (traversal)
                os << ", ";
        }
        os << " ]";
        return os.str();
    }

    friend std::ostream &operator<<(std::ostream &strm, const DoublyLinkedList<T> &a) { return strm << a.toString(); }
};

void DoublyLinkedListTest() {
    int n, data;
    std::cout << "Number of values: ";
    std::cin >> n;
    DoublyLinkedList<int> linkedList;
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

#endif // ALGORITHMS_DOUBLYLINKEDLIST_H
