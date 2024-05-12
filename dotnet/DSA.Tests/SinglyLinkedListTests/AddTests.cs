using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.SinglyLinkedListTests;

public class AddTests
{
    [Theory]
    [InlineData("", "")]
    [InlineData("1", "[1]")]
    [InlineData("1,2,3,4,5", "[1] -> [2] -> [3] -> [4] -> [5]")]
    public void Add_ExpectSuccess(string valuesString, string expectedPrintedResult)
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
        var printedResult = singlyLinkedList.Print();

        // Assert
        Assert.Equal(expectedPrintedResult, printedResult);
    }
}