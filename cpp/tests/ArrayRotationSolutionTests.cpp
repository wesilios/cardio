#include "DataStructures/Array/ArrayRotationSolution.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace {
using RotateFn = void (*)(int *, int, int);

struct Algorithm {
    std::string name;
    RotateFn rotate;
};

// Lets GoogleTest print the algorithm name instead of raw bytes.
void PrintTo(const Algorithm &algorithm, std::ostream *os) { *os << algorithm.name; }

const Algorithm kAlgorithms[] = {
    {"TempArray", ArrayRotationSolution::rotateUsingTempArray},
    {"LeftRotateByOne", ArrayRotationSolution::leftRotateByOne},
    {"Juggling", ArrayRotationSolution::rotateWithJugglingAlgorithm},
    {"Reversal", ArrayRotationSolution::rotateWithReversalAlgorithm},
    {"BlockSwap", ArrayRotationSolution::rotateWithBlockSwapAlgorithm},
    {"BlockSwapIterative", ArrayRotationSolution::rotateWithBlockSwapIterativeAlgorithm},
};

std::vector<int> range(int n) {
    std::vector<int> values(n);
    std::iota(values.begin(), values.end(), 1);
    return values;
}

// Left-rotates a copy of `input` by `d` using std::rotate, the reference answer.
std::vector<int> expectedRotation(std::vector<int> input, int d) {
    std::rotate(input.begin(), input.begin() + d, input.end());
    return input;
}

std::vector<int> rotateWith(const Algorithm &algorithm, std::vector<int> input, int d) {
    algorithm.rotate(input.data(), d, static_cast<int>(input.size()));
    return input;
}
} // namespace

class ArrayRotation : public ::testing::TestWithParam<Algorithm> {};

TEST_P(ArrayRotation, RotatesSampleInput) {
    // Same cases as DataStructures/Array/testcases.txt
    EXPECT_EQ(rotateWith(GetParam(), range(12), 3), (std::vector<int>{4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3}));
    EXPECT_EQ(rotateWith(GetParam(), range(12), 9), (std::vector<int>{10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST_P(ArrayRotation, MatchesStdRotateForEveryValidShift) {
    // Mix of sizes: primes, powers of two and composites exercise the different
    // gcd paths (Juggling) and recursion shapes (BlockSwap).
    for (int n : {2, 3, 7, 8, 12, 16, 17}) {
        for (int d = 1; d < n; ++d) {
            EXPECT_EQ(rotateWith(GetParam(), range(n), d), expectedRotation(range(n), d)) << "n=" << n << " d=" << d;
        }
    }
}

TEST_P(ArrayRotation, HandlesDuplicateValues) {
    const std::vector<int> input = {1, 1, 2, 2, 3, 3};
    EXPECT_EQ(rotateWith(GetParam(), input, 2), expectedRotation(input, 2));
}

// Regression tests: d == 0 used to hit `int temp[0]` (TempArray) and `n % 0` (Juggling);
// d == n used to loop forever (BlockSwapIterative).
TEST_P(ArrayRotation, ZeroShiftLeavesArrayUnchanged) {
    EXPECT_EQ(rotateWith(GetParam(), range(5), 0), range(5));
}

TEST_P(ArrayRotation, FullShiftLeavesArrayUnchanged) {
    EXPECT_EQ(rotateWith(GetParam(), range(5), 5), range(5));
}

INSTANTIATE_TEST_SUITE_P(AllAlgorithms, ArrayRotation, ::testing::ValuesIn(kAlgorithms),
                         [](const ::testing::TestParamInfo<Algorithm> &info) { return info.param.name; });
