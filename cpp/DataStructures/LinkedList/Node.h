#ifndef ALGORITHMS_NODE_H
#define ALGORITHMS_NODE_H

namespace dsa {
    template<typename T, class U>
    class Node {
    protected:
        T data_;
        U *next_;
    public:
        Node() : next_(nullptr) {}

        Node(T data, U *next) : data_(data), next_(next) {}

        void setData(const T &data) {
            Node<T, U>::data_ = data;
        }

        T getData() {
            return Node<T, U>::data_;
        }

        U *getNext() {
            return Node<T, U>::next_;
        }

        void setNext(U *next) {
            Node<T, U>::next_ = next;
        }
    };

    template<typename T>
    class SinglyNode final : public Node<T, SinglyNode<T>> {
    public:
        SinglyNode() : Node<T, SinglyNode<T>>(nullptr) {}

        SinglyNode(T data, SinglyNode *next) : Node<T, SinglyNode<T>>(data, next) {}
    };

    template<typename T>
    class DoublyNode final : public Node<T, DoublyNode<T>> {
    private:
        DoublyNode<T> *previous_;
    public:
        DoublyNode() : Node<T, DoublyNode<T>>(nullptr), previous_(nullptr) {}

        DoublyNode(T data, DoublyNode *previous, DoublyNode *next)
                : Node<T, DoublyNode<T>>(data, next), previous_(previous) {}

        void setPrevious(DoublyNode<T> *previous) {
            previous_ = previous;
        }

        DoublyNode<T> *getPrevious() {
            return previous_;
        }
    };
}
#endif //ALGORITHMS_NODE_H
