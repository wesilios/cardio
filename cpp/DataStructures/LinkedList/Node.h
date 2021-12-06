#ifndef ALGORITHMS_NODE_H
#define ALGORITHMS_NODE_H

template<typename T>
class SinglyNode {
public:
    T data;
    SinglyNode *next;

    SinglyNode() : next(nullptr) {
    }

    SinglyNode(T data, SinglyNode *next) : data(data), next(next) {
    }
};

template<typename T>
class DoublyNode {
public:
    T data;
    DoublyNode *previous, *next;

    DoublyNode() : previous(nullptr), next(nullptr) {
    }

    DoublyNode(T data, DoublyNode *previous, DoublyNode *next)
            : data(data), previous(previous), next(next) {
    }
};

#endif //ALGORITHMS_NODE_H
