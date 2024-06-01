using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.SinglyLinkedListTests;

public class AddToHeadTests
{
    [Theory]
    [InlineData("", "1", "[1]")]
    [InlineData("1,2,3,4,5", "0", "[0] -> [1] -> [2] -> [3] -> [4] -> [5]")]
    public void AddToHead_ExpectSuccess(string valuesString, string value, string expectedPrintedResult)
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
        singlyLinkedList.AddToHead(value);
        var printedResult = singlyLinkedList.Print();

        // Assert
        Assert.Equal(expectedPrintedResult, printedResult);
    }
}