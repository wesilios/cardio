#include <iostream>

#ifndef ALGORITHMS_ARRAYROTATIONSOLUTION_H
#define ALGORITHMS_ARRAYROTATIONSOLUTION_H

int findGreatestCommonDivisor(int d, int n);

void reverseArray(int *arr, int start, int end);

void printResult(int *arr, int n);

class ArrayRotationSolution {

public:
    void rotateUsingTempArray(int *arr, int d, int n);

    void leftRotateByOne(int *arr, int d, int n);

    void rotateWithJugglingAlgorithm(int *arr, int d, int n);

    void rotateWithReversalAlgorithm(int *arr, int d, int n);

    void rotateWithBlockSwapAlgorithm(int *arr, int d, int n);
};

#endif //ALGORITHMS_ARRAYROTATIONSOLUTION_H
