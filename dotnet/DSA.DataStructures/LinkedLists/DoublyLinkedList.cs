using DSA.DataStructures.Nodes;

namespace DSA.DataStructures.LinkedLists;

public class DoublyLinkedList<T> : IDoublyLinkedList<T>
{
    private int Size { get; set; } = 0;
    private DoublyLinkedListNode<T>? Head { get; set; } = null;
    private DoublyLinkedListNode<T>? Tail { get; set; } = null;

    public void Add(T data)
    {
        throw new NotImplementedException();
    }

    public void AddToHead(T data)
    {
        throw new NotImplementedException();
    }

    public void InsertNodeToPosition(T data, int position)
    {
        throw new NotImplementedException();
    }

    public void DeleteNodeAtPosition(int position)
    {
        throw new NotImplementedException();
    }

    public bool IsEmpty()
    {
        throw new NotImplementedException();
    }

    public string Print()
    {
        throw new NotImplementedException();
    }
}