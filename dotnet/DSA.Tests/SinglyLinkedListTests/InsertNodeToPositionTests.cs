using System;
using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.SinglyLinkedListTests;

public class InsertNodeToPositionTests
{
    [Theory]
    [InlineData("", "0", 0, "[0]")]
    [InlineData("1", "0", 1, "[1] -> [0]")]
    [InlineData("1,2,3,4,5", "0", 0, "[0] -> [1] -> [2] -> [3] -> [4] -> [5]")]
    [InlineData("1,2,3,4,5", "0", 5, "[1] -> [2] -> [3] -> [4] -> [5] -> [0]")]
    [InlineData("1,2,3,4,5", "0", 3, "[1] -> [2] -> [0] -> [3] -> [4] -> [5]")]
    public void Add_ExpectSuccess(string valuesString, string value, int position, string expectedPrintedResult)
    {
        // Arrange
        var singlyLinkedList = new SinglyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                singlyLinkedList.Add(t);
            }
        }

        // Act
        singlyLinkedList.InsertNodeToPosition(value, position);
        var printedResult = singlyLinkedList.Print();

        // Assert
        Assert.Equal(expectedPrintedResult, printedResult);
    }

    [Theory]
    [InlineData("", "0", 1)]
    [InlineData("1", "0", 2)]
    [InlineData("1,2,3,4,5", "0", 6)]
    public void Add_ExpectArgumentOutOfRangeException(string valuesString, string value, int position)
    {
        // Arrange
        var singlyLinkedList = new SinglyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                singlyLinkedList.Add(t);
            }
        }

        // Act & Assert
        Assert.Throws<ArgumentOutOfRangeException>(() => singlyLinkedList.InsertNodeToPosition(value, position));
    }
    
    [Theory]
    [InlineData("", "0", -1)]
    [InlineData("1", "0", -2)]
    [InlineData("1,2,3,4,5", "0", -100)]
    public void Add_ExpectArgumentException(string valuesString, string value, int position)
    {
        // Arrange
        var singlyLinkedList = new SinglyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                singlyLinkedList.Add(t);
            }
        }

        // Act & Assert
        Assert.Throws<ArgumentException>(() => singlyLinkedList.InsertNodeToPosition(value, position));
    }
}