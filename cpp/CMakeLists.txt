cmake_minimum_required(VERSION 3.20)
project (Algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(Algorithms main.cpp DataStructures/Array/ArrayRotationSolution.h DataStructures/LinkedList/SinglyLinkedList.h Exception.h)
