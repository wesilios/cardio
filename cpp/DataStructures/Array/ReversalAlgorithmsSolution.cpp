#include "ReversalAlgorithmsSolution.h"

void ReversalAlgorithmsSolution::reverse(int *arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void ReversalAlgorithmsSolution::leftRotate(int *arr, int d, int n) {
    reverse(arr, 0, d - 1);
    reverse(arr, d, n - 1);
    reverse(arr, 0, n - 1);
    printResult(arr, n);
}
