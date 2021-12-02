#include "ArrayRotationSolution.h"

int ArrayRotationSolution::findGreatestCommonDivisor(int d, int n) {
    int remain = n % d;
    if (remain != 0) {
        return findGreatestCommonDivisor(remain, d);
    }
    return d;
}

//Time O(n), Space O(d)
void ArrayRotationSolution::rotateUsingTempArray(int *arr, int d, int n) {
    int temp[d];
    for (int i = 0; i < d; ++i) {
        temp[i] = arr[i];
    }

    int j = 0;
    for (int i = 0; i < n; ++i) {
        if (i < n - d) {
            arr[i] = arr[i + d];
            continue;
        }
        arr[i] = temp[j];
        j++;
    }

    printResult(arr, n);
}

//Time O (n * d), Space O(1)
void ArrayRotationSolution::leftRotateByOne(int *arr, int d, int n) {
    for (int i = 0; i < d; ++i) {
        int temp = arr[0];
        for (int j = 0; j < n - 1; j++) {
            arr[j] = arr[j + 1];
        }
        arr[n - 1] = temp;
    }

    printResult(arr, n);
}

// Time O(n), Space O(1)
void ArrayRotationSolution::rotateWithJugglingAlgorithms(int *arr, int d, int n) {
    int gcd = findGreatestCommonDivisor(d, n);
    for (int i = 0; i < gcd; ++i) {
        int temp = arr[i];
        int j = i;
        while (true) {
            int k = j + d;
            if (k >= n) {
                k = k - n;
            }
            if (k == i) {
                break;
            }
            arr[j] = arr[k];
            j = k;
        }
        arr[j] = temp;
    }

    printResult(arr, n);
}

