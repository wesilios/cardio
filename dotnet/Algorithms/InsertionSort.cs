namespace Algorithms;

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