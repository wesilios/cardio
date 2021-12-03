#include "DataStructures/Array/ArrayRotationSolution.h"
#include "DataStructures/LinkedList/SinglyLinkedList.h"

int main() {
    int choice;
    std::cout << "1. Array rotation" << std::endl;
    std::cout << "2. Linked list" << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1:
            arrayRotationSolutionTests();
            break;
        case 2:
            linkedListTest();
        default:
            break;
    }
    return 0;
}
