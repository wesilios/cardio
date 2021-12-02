#include "ArraySolution.h"

#ifndef ALGORITHMS_REVERSALALGORITHMSSOLUTION_H
#define ALGORITHMS_REVERSALALGORITHMSSOLUTION_H

class ReversalAlgorithmsSolution : ArraySolution {
private:
    static void reverse(int *arr, int start, int end);

public:
    void leftRotate(int *arr, int d, int n);
};

#endif //ALGORITHMS_REVERSALALGORITHMSSOLUTION_H
