using Algorithms.SortingAlgorithms;
using Xunit;

namespace DSA.Tests;

public class SortTests
{
    [Fact]
    public void InsertionSort()
    {
        // Arrange
        int[] input =
        {
            -2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8, -14, 12, -2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8,
            -14, 12, -2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8, -14, 12
        };
        int[] expectedResults =
        {
            -14, -14, -14, -14, -14, -14, -13, -13, -13, -7, -7, -7, -4, -4, -4, -2, -2, -2, 0, 0, 0, 0, 0, 0, 5, 5, 5,
            7, 7, 7, 7, 7, 7, 8, 8, 8, 12, 12, 12, 15, 15, 15, 15, 15, 15
        };
        var sortAlgo = SortingFactory.FactoryMethod(nameof(InsertionSort));

        // Act
        var result = sortAlgo.Sort(input);

        // Assert
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(expectedResults[i], result[i]);
        }
    }

    [Fact]
    public void SelectionSort()
    {
        // Arrange
        int[] input = { 4, 3, 2, 10, 12, 1, 5, 6 };
        int[] expectedResults = { 1, 2, 3, 4, 5, 6, 10, 12 };
        var sortAlgo = SortingFactory.FactoryMethod(nameof(SelectionSort));

        // Act
        var result = sortAlgo.Sort(input);

        // Assert
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(expectedResults[i], result[i]);
        }
    }

    [Fact]
    public void BubbleSort()
    {
        // Arrange
        int[] input = { 4, 3, 2, 10, 12, 1, 5, 6 };
        int[] expectedResults = { 1, 2, 3, 4, 5, 6, 10, 12 };
        var sortAlgo = SortingFactory.FactoryMethod(nameof(BubbleSort));

        // Act
        var result = sortAlgo.Sort(input);

        // Assert
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(expectedResults[i], result[i]);
        }
    }

    [Fact]
    public void MergeSort()
    {
        // Arrange
        int[] input = { 4, 3, 2, 10, 12, 1, 5, 6 };
        int[] expectedResults = { 1, 2, 3, 4, 5, 6, 10, 12 };
        var sortAlgo = SortingFactory.FactoryMethod(nameof(MergeSort));

        // Act
        var result = sortAlgo.Sort(input);

        // Assert
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(expectedResults[i], result[i]);
        }
    }

    [Fact]
    public void HeapSort()
    {
        // Arrange
        int[] input = { 4, 3, 2, 10, 12, 1, 5, 6 };
        int[] expectedResults = { 1, 2, 3, 4, 5, 6, 10, 12 };
        var sortAlgo = SortingFactory.FactoryMethod(nameof(HeapSort));

        // Act
        var result = sortAlgo.Sort(input);

        // Assert
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(expectedResults[i], result[i]);
        }
    }

    [Fact]
    public void TimSort()
    {
        // Arrange
        int[] input =
        {
            -2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8, -14, 12, -2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8,
            -14, 12, -2, 7, 15, -14, 0, 15, 0, 7, -7, -4, -13, 5, 8, -14, 12
        };
        int[] expectedResults =
        {
            -14, -14, -14, -14, -14, -14, -13, -13, -13, -7, -7, -7, -4, -4, -4, -2, -2, -2, 0, 0, 0, 0, 0, 0, 5, 5, 5,
            7, 7, 7, 7, 7, 7, 8, 8, 8, 12, 12, 12, 15, 15, 15, 15, 15, 15
        };
        var sortAlgo = SortingFactory.FactoryMethod(nameof(TimSort));

        // Act
        var result = sortAlgo.Sort(input);

        // Assert
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(expectedResults[i], result[i]);
        }
    }
}