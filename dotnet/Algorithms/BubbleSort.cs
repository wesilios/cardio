namespace Algorithms;

public class BubbleSort : ISort
{
    public int[] Sort(int[] input)
    {
        for (var i = 0; i < input.Length - 1; i++)
        {
            var swapped = false;
            for (var j = 0; j < input.Length - i - 1; j++)
            {
                if (input[j] <= input[j + 1]) continue;
                (input[j], input[j + 1]) = (input[j + 1], input[j]);
                swapped = true;
            }

            if (swapped == false) break;
        }

        return input;
    }
}