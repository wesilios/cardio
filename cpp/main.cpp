#include "DataStructures/Array/ArrayRotationSolution.h"

int main() {
    int t, n, d;
    std::cin >> t;
    while (t > 0) {
        std::cin >> n >> d;
        d = d % n;
        int arr[n];
        for (int i = 0; i < n; i++) {
            std::cin >> arr[i];
        }
        ArrayRotationSolution arrayRotationSolution;
//        arrayRotationSolution.rotateUsingTempArray(arr, d, n);
//        printResult(arr, n);
//        arrayRotationSolution.leftRotateByOne(arr, d, n);
//        printResult(arr, n);
//        arrayRotationSolution.rotateWithJugglingAlgorithm(arr, d, n);
//        printResult(arr, n);
//        arrayRotationSolution.rotateWithReversalAlgorithm(arr, d, n);
//        printResult(arr, n);
        arrayRotationSolution.rotateWithBlockSwapAlgorithm(arr, d, n);
        printResult(arr, n);
        t--;
    }
    return 0;
}
