namespace Algorithms.SortingAlgorithms;

public static class SortingFactory
{
    public static ISort FactoryMethod(string sortName)
    {
        return sortName switch
        {
            nameof(InsertionSort) => new InsertionSort(),
            nameof(BubbleSort) => new BubbleSort(),
            nameof(SelectionSort) => new SelectionSort(),
            _ => new MergeSort()
        };
    }
}