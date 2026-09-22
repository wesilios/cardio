namespace Algorithms.SortingAlgorithms;

public interface IMergeSort : ISort
{
    void Merge(int[] input, int left, int middle, int right);
    void Sort(int[] input, int left, int right);
}

/*
 * Time complexity: Best case O(nlogn), Average case O(nlogn), Worst case O(nlogn)
 * Space complexity: O(n)
 * Stable: Yes
 */
public class MergeSort : IMergeSort
{
    public int[] Sort(int[] input)
    {
        Sort(input, 0, input.Length - 1);
        return input;
    }

    public void Merge(int[] input, int left, int middle, int right)
    {
        int i, j;
        var firstArrayLength = middle - left + 1;
        var secondArrayLength = right - middle;

        var firstArray = new int[firstArrayLength];
        var secondArray = new int[secondArrayLength];

        for (i = 0; i < firstArrayLength; i++)
        {
            firstArray[i] = input[left + i];
        }

        for (j = 0; j < secondArrayLength; j++)
        {
            secondArray[j] = input[middle + 1 + j];
        }

        i = j = 0;
        var k = left;
        while (i < firstArrayLength && j < secondArrayLength)
        {
            if (firstArray[i] <= secondArray[j])
            {
                input[k] = firstArray[i];
                i++;
            }
            else
            {
                input[k] = secondArray[j];
                j++;
            }

            k++;
        }

        while (i < firstArrayLength)
        {
            input[k] = firstArray[i];
            i++;
            k++;
        }

        while (j < secondArrayLength)
        {
            input[k] = secondArray[j];
            j++;
            k++;
        }
    }

    public void Sort(int[] input, int left, int right)
    {
        if (left >= right) return;
        var middle = left + (right - left) / 2;
        Sort(input, left, middle);
        Sort(input, middle + 1, right);

        Merge(input, left, middle, right);
    }
}