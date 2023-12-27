using Algorithms.SortingAlgorithms;
using Xunit;

namespace DSA.Tests;

public class SortTests
{
    private readonly int[] _input = { 4, 3, 2, 10, 12, 1, 5, 6 };
    private readonly int[] _expectedResults = { 1, 2, 3, 4, 5, 6, 10, 12 };

    [Fact]
    public void InsertionSort()
    {
        var insertionSort = new InsertionSort();

        var result = insertionSort.Sort(_input);
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(_expectedResults[i], result[i]);
        }
    }
    
    [Fact]
    public void SelectionSort()
    {
        var insertionSort = new SelectionSort();

        var result = insertionSort.Sort(_input);
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(_expectedResults[i], result[i]);
        }
    }
    
    [Fact]
    public void BubbleSort()
    {
        var insertionSort = new BubbleSort();

        var result = insertionSort.Sort(_input);
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(_expectedResults[i], result[i]);
        }
    }
    
    [Fact]
    public void MergeSort()
    {
        var insertionSort = new MergeSort();

        var result = insertionSort.Sort(_input);
        for (var i = 0; i < result.Length; i++)
        {
            Assert.Equal(_expectedResults[i], result[i]);
        }
    }
}