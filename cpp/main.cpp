#include "DataStructures/Array/ArrayRotationSolution.h"
#include "DataStructures/LinkedList/SinglyLinkedList.h"
#include "DataStructures/LinkedList/DoublyLinkedList.h"
#include "DataStructures/LinkedList/CircularSinglyLinkedList.h"

int main() {
    int choice;
    std::cout << "1. Array rotation" << std::endl;
    std::cout << "2. Singly Linked list" << std::endl;
    std::cout << "3. Doubly Linked list" << std::endl;
    std::cout << "4. Circular Singly Linked list" << std::endl;
    std::cout << "5. Circular Doubly Linked list" << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1:
            arrayRotationSolutionTests();
            break;
        case 2:
            SinglyLinkedListTest();
            break;
        case 3:
            DoublyLinkedListTest();
            break;
        case 4:
            CircularSinglyLinkedListTest();
            break;
        case 5:
            CircularDoublyLinkedListTest();
            break;
        default:
            break;
    }
    return 0;
}
