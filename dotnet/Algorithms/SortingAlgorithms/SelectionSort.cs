namespace Algorithms.SortingAlgorithms;

public class SelectionSort : ISort
{
    /*
     * Time complexity: Best case O(n), Average case O(n2), Worst case O(n2)
     * Space complexity: 1
     * Stable: no
     */
    public int[] Sort(int[] input)
    {
        for (var i = 0; i < input.Length - 1; i++)
        {
            var min = i;
            for (var j = i + 1; j < input.Length; j++)
            {
                if (input[min] > input[j]) min = j;
            }

            (input[i], input[min]) = (input[min], input[i]);
        }

        return input;
    }
}