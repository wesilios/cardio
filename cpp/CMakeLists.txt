cmake_minimum_required(VERSION 3.20)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(Algorithms main.cpp
        DataStructures/Array/ArrayRotationSolution.h
        DataStructures/Exception.h
        DataStructures/LinkedList/Node.h
        DataStructures/LinkedList/SinglyLinkedList.h
        DataStructures/LinkedList/DoublyLinkedList.h
        DataStructures/LinkedList/CircularSinglyLinkedList.h
        DataStructures/LinkedList/CircularDoublyLinkedList.h
        DataStructures/Stack/Stack.h
        DataStructures/Stack/ArrayStack.h)
