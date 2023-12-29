namespace Algorithms.SortingAlgorithms;

public interface IHeapSort
{
    void Heapify(int[] input, int length, int root);
}

public class HeapSort : ISort, IHeapSort
{
    /*
     * Time complexity: Best case O(nlogn), Average case O(nlogn), Worst case O(nlogn)
     * Space complexity: 1
     * Stable: No
     */
    public int[] Sort(int[] input)
    {
        var length = input.Length;
        for (var i = length / 2 - 1; i >= 0; i--)
        {
            Heapify(input, length, i);
        }

        for (var i = length - 1; i > 0; i--)
        {
            (input[0], input[i]) = (input[i], input[0]);
            Heapify(input, i, 0);
        }

        return input;
    }

    public void Heapify(int[] input, int length, int root)
    {
        var largest = root;
        var left = 2 * root + 1;
        var right = 2 * root + 2;

        if (left < length && input[left] > input[largest])
            largest = left;

        if (right < length && input[right] > input[largest])
            largest = right;

        if (largest != root)
        {
            (input[root], input[largest]) = (input[largest], input[root]);
            Heapify(input, length, largest);
        }
    }
}