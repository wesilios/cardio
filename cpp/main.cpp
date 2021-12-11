#include "DataStructures/Array/ArrayRotationSolution.h"
#include "DataStructures/LinkedList/SinglyLinkedList.h"
#include "DataStructures/LinkedList/DoublyLinkedList.h"
#include "DataStructures/LinkedList/CircularSinglyLinkedList.h"
#include "DataStructures/LinkedList/CircularDoublyLinkedList.h"
#include "DataStructures/Stack/ArrayStack.h"
#include "DataStructures/Stack/ListStack.h"
#include "DataStructures/Queue/ArrayQueue.h"
#include "DataStructures/Queue/ListQueue.h"

int main() {
    int choice;
    std::cout << "1. Array rotation." << std::endl;
    std::cout << "2. Singly Linked list." << std::endl;
    std::cout << "3. Doubly Linked list." << std::endl;
    std::cout << "4. Circular Singly Linked list." << std::endl;
    std::cout << "5. Circular Doubly Linked list." << std::endl;
    std::cout << "6. Array Stack." << std::endl;
    std::cout << "7. List Stack." << std::endl;
    std::cout << "8. Array Queue." << std::endl;
    std::cout << "9. List Queue." << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1:
            arrayRotationSolutionTests();
            break;
        case 2:
            dsa::SinglyLinkedListTest();
            break;
        case 3:
            dsa::DoublyLinkedListTest();
            break;
        case 4:
            dsa::CircularSinglyLinkedListTest();
            break;
        case 5:
            dsa::CircularDoublyLinkedListTest();
            break;
        case 6:
            dsa::ArrayStackTest();
            break;
        case 7:
            dsa::ListStackTest();
            break;
        case 8:
            dsa::ArrayQueueTest();
            break;
        case 9:
            dsa::ListQueueTest();
            break;
        default:
            break;
    }
    return 0;
}
