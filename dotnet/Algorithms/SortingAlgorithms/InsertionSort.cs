namespace Algorithms.SortingAlgorithms;

/*
 * Time complexity: Best case O(n), Average case O(n2), Worst case O(n2)
 * Space complexity: O(1)
 * Stable: Yes
 */
public class InsertionSort : ISort
{
    
    public int[] Sort(int[] input)
    {
        for (var i = 1; i < input.Length; i++)
        {
            var last = input[i];
            var j = i;
            while (j > 0 && input[j - 1] > last)
            {
                input[j] = input[j - 1];
                j -= 1;
                input[j] = last;
            }
        }

        return input;
    }
}