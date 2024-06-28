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
            nameof(HeapSort) => new HeapSort(),
            nameof(TimSort) => new TimSort(),
            nameof(ShellSort) => new ShellSort(),
            nameof(TreeSort) => new TreeSort(),
            _ => new MergeSort()
        };
    }
}