#include <iostream>
#include "ArraySolution.h"

#ifndef ALGORITHMS_ARRAYROTATIONSOLUTION_H
#define ALGORITHMS_ARRAYROTATIONSOLUTION_H

class ArrayRotationSolution : ArraySolution {
private:
    int findGreatestCommonDivisor(int d, int n);

public:
    void rotateUsingTempArray(int *arr, int d, int n);

    void leftRotateByOne(int *arr, int d, int n);

    void rotateWithJugglingAlgorithms(int *arr, int d, int n);
};

#endif //ALGORITHMS_ARRAYROTATIONSOLUTION_H
