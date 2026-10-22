cmake_minimum_required(VERSION 3.20)
project(Algorithms CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd

add_library(dsa INTERFACE)
target_include_directories(dsa INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Algorithms main.cpp)
target_link_libraries(Algorithms PRIVATE dsa)

option(CARDIO_BUILD_TESTS "Build unit tests" ON)
if(CARDIO_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz)
    fetchcontent_makeavailable(googletest)

    enable_testing()
    add_executable(
        runTests
        tests/ArrayStackTests.cpp
        tests/ArrayRotationSolutionTests.cpp
        tests/SinglyLinkedListTests.cpp
        tests/DoublyLinkedListTests.cpp
        tests/CircularSinglyLinkedListTests.cpp
        tests/CircularDoublyLinkedListTests.cpp)
    target_link_libraries(runTests PRIVATE dsa GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(runTests)
endif()

# add_executable( Algorithms main.cpp DataStructures/Array/ArrayRotationSolution.h DataStructures/Exception.h
# DataStructures/LinkedList/Node.h DataStructures/LinkedList/LinkedList.h DataStructures/LinkedList/SinglyLinkedList.h
# DataStructures/LinkedList/DoublyLinkedList.h DataStructures/Stack/Stack.h DataStructures/Stack/ArrayStack.h
# DataStructures/Stack/ListStack.h DataStructures/Queue/Queue.h DataStructures/Queue/ArrayQueue.h
# DataStructures/Queue/ListQueue.h DataStructures/LinkedList/CircularDoublyLinkedList.h
# DataStructures/LinkedList/CircularSinglyLinkedList.h)
