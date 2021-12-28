cmake_minimum_required(VERSION 3.20)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(Algorithms main.cpp
        DataStructures/Array/ArrayRotationSolution.h
        DataStructures/Exception.h
        DataStructures/LinkedList/Node.h
        DataStructures/LinkedList/LinkedList.h
        DataStructures/LinkedList/SinglyLinkedList.h
        DataStructures/LinkedList/DoublyLinkedList.h
        DataStructures/Stack/Stack.h
        DataStructures/Stack/ArrayStack.h
        DataStructures/Stack/ListStack.h
        DataStructures/Queue/Queue.h
        DataStructures/Queue/ArrayQueue.h
        DataStructures/Queue/ListQueue.h
        DataStructures/LinkedList/CircularDoublyLinkedList.h
        DataStructures/LinkedList/CircularSinglyLinkedList.h)
