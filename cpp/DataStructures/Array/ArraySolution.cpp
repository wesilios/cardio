#include <iostream>

#include "ArraySolution.h"

void ArraySolution::printResult(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
