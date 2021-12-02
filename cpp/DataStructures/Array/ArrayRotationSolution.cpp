#include "ArrayRotationSolution.h"

void printResult(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
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
}

int findGreatestCommonDivisor(int d, int n) {
    int remain = n % d;
    if (remain != 0) {
        return findGreatestCommonDivisor(remain, d);
    }
    return d;
}

// Time O(n), Space O(1)
void ArrayRotationSolution::rotateWithJugglingAlgorithm(int *arr, int d, int n) {
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
}

void reverseArray(int *arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void ArrayRotationSolution::rotateWithReversalAlgorithm(int *arr, int d, int n) {
    reverseArray(arr, 0, d - 1);
    reverseArray(arr, d, n - 1);
    reverseArray(arr, 0, n - 1);
}

void swap(int *arr, int start, int end, int d) {
    for (int i = 0; i < d; ++i) {
        int temp = arr[start + i];
        arr[start + i] = arr[end + i];
        arr[end + i] = temp;
    }
}

void ArrayRotationSolution::rotateWithBlockSwapAlgorithm(int *arr, int d, int n) {
    if (d == 0 || n == 0) {
        return;
    }

    if (n - d == 0) {
        swap(arr, 0, n - d, d);
        return;
    }

    if (d < n - d) {
        swap(arr, 0, n - d, d);
        rotateWithBlockSwapAlgorithm(arr, d, n - d);
        return;
    }

    swap(arr, 0, d, n - d);
    rotateWithBlockSwapAlgorithm(arr + n - d, 2 * d - n, d);
}

void ArrayRotationSolution::rotateWithBlockSwapIterativeAlgorithm(int *arr, int d, int n) {
    int i, j;
    if (d == 0 || n == 0) {
        return;
    }

    i = d;
    j = n - d;
    while (i != j) {
        if (i < j) {
            swap(arr, d - i, d + j - i, i);
            j -= i;
            continue;
        }

        swap(arr, d - i, d, j);
        i -= j;
    }
    swap(arr, d - i, d, i);
}

