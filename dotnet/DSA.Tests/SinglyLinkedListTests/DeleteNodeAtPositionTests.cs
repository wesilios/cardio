using System;
using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.SinglyLinkedListTests;

public class DeleteNodeAtPositionTests
{
    [Theory]
    [InlineData("", 0, "")]
    [InlineData("1", 0, "")]
    [InlineData("1,2,3,4,5", 2, "[1] -> [2] -> [4] -> [5]")]
    [InlineData("1,2,3,4,5", 4, "[1] -> [2] -> [3] -> [4]")]
    public void DeleteNodeAtPosition_ExpectSuccess(string valuesString, int position, string expectedPrintedResult)
    {
        // Arrange
        ISinglyLinkedList<string> singlyLinkedList = new SinglyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                singlyLinkedList.Add(t);
            }
        }

        // Act
        singlyLinkedList.DeleteNodeAtPosition(position);
        var printedResult = singlyLinkedList.Print();

        // Assert
        Assert.Equal(expectedPrintedResult, printedResult);
    }

    [Theory]
    [InlineData("1", -1)]
    public void DeleteNodeAtPosition_ExpectArgumentException(string valuesString, int position)
    {
        // Arrange
        ISinglyLinkedList<string> singlyLinkedList = new SinglyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                singlyLinkedList.Add(t);
            }
        }

        // Act & Assert
        Assert.Throws<ArgumentException>(() => singlyLinkedList.DeleteNodeAtPosition(position));
    }

    [Theory]
    [InlineData("", 1)]
    [InlineData("1", 1)]
    [InlineData("1,2,3,4,5", 5)]
    public void DeleteNodeAtPosition_ExpectArgumentOutOfRangeException(string valuesString, int position)
    {
        // Arrange
        ISinglyLinkedList<string> singlyLinkedList = new SinglyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                singlyLinkedList.Add(t);
            }
        }

        // Act & Assert
        Assert.Throws<ArgumentOutOfRangeException>(() => singlyLinkedList.DeleteNodeAtPosition(position));
    }
}