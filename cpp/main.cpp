#include "DataStructures/Array/ArrayRotationSolution.h"
#include "DataStructures/Array/ReversalAlgorithmsSolution.h"

int main() {
    int t, n, d, choice;
    std::cin >> choice >> t;
    while (t > 0) {
        std::cin >> n >> d;
        d = d % n;
        int arr[n];
        for (int i = 0; i < n; i++) {
            std::cin >> arr[i];
        }
        switch (choice) {
            case 1:
                ArrayRotationSolution arrayRotationSolution;
                arrayRotationSolution.rotateUsingTempArray(arr, d, n);
                arrayRotationSolution.leftRotateByOne(arr, d, n);
                arrayRotationSolution.rotateWithJugglingAlgorithms(arr, d, n);
                break;
            case 2:
                ReversalAlgorithmsSolution reversalAlgorithmsSolution;
                reversalAlgorithmsSolution.leftRotate(arr, d, n);
                break;
            default:
                break;
        }
        t--;
    }
    return 0;
}
