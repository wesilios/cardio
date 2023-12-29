using System.Diagnostics;

namespace Algorithms.SortingAlgorithms;

public interface ITimSort : ISort
{
    void InsertSort(int[] input, int left, int right);
    void MergeSort(int[] input, int left, int middle, int right);
}

/*
 * Time complexity: Best case O(n), Average case O(nlogn), Worst case O(nlogn)
 * Space complexity: O(n)
 * Stable: No
 */
public class TimSort : ITimSort
{
    private const int Run = 32;
    private readonly IMergeSort _mergeSort = (IMergeSort)SortingFactory.FactoryMethod(nameof(MergeSort));

    // Iterative TimSort function to sort the 
    // array[0...n-1] (similar to merge sort) 
    public int[] Sort(int[] input)
    {
        Debug.Print("Start [{0}]", string.Join(", ", input));
        var length = input.Length;
        for (var i = 0; i < input.Length; i++)
        {
            Debug.Print("InsertSort");
            InsertSort(input, i, Math.Min(i + Run - 1, length - 1));
        }

        // Start merging from size RUN (or 32). 
        // It will merge 
        // to form size 64, then 
        // 128, 256 and so on .... 
        for (var size = Run; size < length; size = 2 * size)
        {
            // Pick starting point of 
            // left sub array. We 
            // are going to merge 
            // arr[left..left+size-1] 
            // and arr[left+size, left+2*size-1] 
            // After every merge, we increase 
            // left by 2*size 
            for (var left = 0; left < length; left += 2 * size)
            {
                // Find ending point of left sub array 
                // mid+1 is starting point of 
                // right sub array 
                var middle = left + size - 1;
                var right = Math.Min(left + 2 * size - 1, length - 1);

                // Merge sub array arr[left.....mid] & 
                // arr[mid+1....right] 
                if (middle < right)
                {
                    Debug.Print("MergeSort");
                    MergeSort(input, left, middle, right);
                }
            }
        }

        return input;
    }

    public void InsertSort(int[] input, int left, int right)
    {
        for (var i = left + 1; i <= right; i ++)
        {
            var temp = input[i];
            var j = i - 1;
            while (j >= left && input[j] > temp)
            {
                input[j + 1] = input[j];
                j--;
            }

            input[j + 1] = temp;
            Debug.Print("InsertSort [{0}]", string.Join(", ", input));
        }
    }

    public void MergeSort(int[] input, int left, int middle, int right)
    {
        _mergeSort.Merge(input, left, middle, right);
        Debug.Print("InsertSort [{0}]", string.Join(", ", input));
    }
}