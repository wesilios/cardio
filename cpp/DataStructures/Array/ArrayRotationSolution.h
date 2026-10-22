#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#ifndef ALGORITHMS_ARRAYROTATIONSOLUTION_H
#define ALGORITHMS_ARRAYROTATIONSOLUTION_H

class ArrayRotationSolution {
  private:
    static int findGreatestCommonDivisor(int d, int n) {
        if (d == 0) {
            throw std::runtime_error("Division by zero error!");
        }

        int remain = n % d;
        if (remain != 0) {
            return findGreatestCommonDivisor(remain, d);
        }

        return d;
    }

    static void reverseArray(int *arr, int start, int end) {
        while (start < end) {
            int temp = arr[start];
            arr[start] = arr[end];
            arr[end] = temp;
            start++;
            end--;
        }
    }

    static void swap(int *arr, int start, int end, int d) {
        for (int i = 0; i < d; ++i) {
            int temp = arr[start + i];
            arr[start + i] = arr[end + i];
            arr[end + i] = temp;
        }
    }

  public:
    static void printResult(int *arr, int n) {
        for (int i = 0; i < n; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    static void rotateUsingTempArray(int *arr, int d, int n) {
        if (d == 0 || n == 0) {
            return;
        }

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

    static void leftRotateByOne(int *arr, int d, int n) {
        for (int i = 0; i < d; ++i) {
            int temp = arr[0];
            for (int j = 0; j < n - 1; j++) {
                arr[j] = arr[j + 1];
            }
            arr[n - 1] = temp;
        }
    }

    static void rotateWithJugglingAlgorithm(int *arr, int d, int n) {
        if (d == 0 || n == 0) {
            return;
        }

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

    static void rotateWithReversalAlgorithm(int *arr, int d, int n) {
        reverseArray(arr, 0, d - 1);
        reverseArray(arr, d, n - 1);
        reverseArray(arr, 0, n - 1);
    }

    static void rotateWithBlockSwapAlgorithm(int *arr, int d, int n) {
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

    static void rotateWithBlockSwapIterativeAlgorithm(int *arr, int d, int n) {
        if (d == 0 || n == d) {
            return;
        }

        int i = d;
        int j = n - d;
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
};

void arrayRotationSolutionTests() {
    using RotateFn = void (*)(int *, int, int);
    const std::pair<const char *, RotateFn> algorithms[] = {
        {"Rotate using temp array", ArrayRotationSolution::rotateUsingTempArray},
        {"Rotate one by one", ArrayRotationSolution::leftRotateByOne},
        {"Rotate using Juggling Algorithm", ArrayRotationSolution::rotateWithJugglingAlgorithm},
        {"Rotate using Reversal Algorithm", ArrayRotationSolution::rotateWithReversalAlgorithm},
        {"Rotate using Block swap Algorithm", ArrayRotationSolution::rotateWithBlockSwapAlgorithm},
        {"Rotate using Block swap Iterative Algorithm", ArrayRotationSolution::rotateWithBlockSwapIterativeAlgorithm},
    };

    int t, n, d;
    std::cout << "Number of test cases: ";
    std::cin >> t;
    while (t > 0) {
        std::cout << "Array size and rotation (n d): ";
        std::cin >> n >> d;
        d = d % n;
        std::vector<int> original(n);
        std::cout << "Enter " << n << " values: ";
        for (int i = 0; i < n; i++) {
            std::cin >> original[i];
        }

        // Each algorithm rotates its own copy, so every result starts from the original input.
        for (const auto &algorithm : algorithms) {
            std::vector<int> arr = original;
            std::cout << algorithm.first << ": " << std::endl;
            algorithm.second(arr.data(), d, n);
            ArrayRotationSolution::printResult(arr.data(), n);
        }
        t--;
    }
}

#endif // ALGORITHMS_ARRAYROTATIONSOLUTION_H
