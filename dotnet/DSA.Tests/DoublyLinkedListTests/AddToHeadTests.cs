using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.DoublyLinkedListTests;

public class AddToHeadTests
{
    [Theory]
    [InlineData("", "1", "[1]")]
    [InlineData("1,2,3,4,5", "0", "[0] -> [1] -> [2] -> [3] -> [4] -> [5]")]
    public void AddToHead_ExpectSuccess(string valuesString, string value, string expectedPrintedResult)
    {
        // Arrange
        IDoublyLinkedList<string> doublyLinkedList = new DoublyLinkedList<string>();
        if (!string.IsNullOrEmpty(valuesString))
        {
            var values = valuesString.Split(",");
            foreach (var t in values)
            {
                doublyLinkedList.Add(t);
            }
        }

        // Act
        doublyLinkedList.AddToHead(value);
        var printedResult = doublyLinkedList.Print();

        // Assert
        Assert.Equal(expectedPrintedResult, printedResult);
    }
}