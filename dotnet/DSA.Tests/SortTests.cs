using Algorithms.SortingAlgorithms;
using Xunit;

namespace DSA.Tests;

public class SortTests
{
    [Fact]
    public void InsertionSort()
    {
        // Arrange
        int[] input = { 4, 3, 2, 10, 12, 1, 5, 6 };
        int[] expectedResults = { 1, 2, 3, 4, 5, 6, 10, 12 };
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
}