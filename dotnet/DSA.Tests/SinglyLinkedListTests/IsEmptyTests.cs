using DSA.DataStructures.LinkedLists;
using Xunit;

namespace DSA.Tests.SinglyLinkedListTests;

public class IsEmptyTests
{
    [Theory]
    [InlineData("", true)]
    [InlineData("1", false)]
    public void IsEmpty(string valuesString, bool expectedResult)
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
        Assert.Equal(expectedResult, singlyLinkedList.IsEmpty());
    }
}