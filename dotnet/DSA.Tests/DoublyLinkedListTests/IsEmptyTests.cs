using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.DoublyLinkedListTests;

public class IsEmptyTests
{
    [Theory]
    [InlineData("", true)]
    [InlineData("1", false)]
    public void IsEmpty(string valuesString, bool expectedResult)
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

        // Act & Assert
        Assert.Equal(expectedResult, doublyLinkedList.IsEmpty());
    }
}