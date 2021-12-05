#include "DataStructures/Array/ArrayRotationSolution.h"
#include "DataStructures/LinkedList/SinglyLinkedList.h"
#include "DataStructures/LinkedList/DoublyLinkedList.h"

int main() {
    int choice;
    std::cout << "1. Array rotation" << std::endl;
    std::cout << "2. Singly Linked list" << std::endl;
    std::cout << "3. Doubly Linked list" << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1:
            arrayRotationSolutionTests();
            break;
        case 2:
            SinglyLinkedListTest();
            break;
        case 3:
            DoublyLinkedListTests();
        default:
            break;
    }
    return 0;
}
