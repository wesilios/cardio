using DSA.DataStructures.Nodes;

namespace DSA.DataStructures.LinkedLists;

public class SinglyLinkedList<T> : ILinkedList<T>
{
    public int Size { get; set; }
    public SinglyLinkedListNode<T> Head { get; set; }

    public SinglyLinkedList(SinglyLinkedListNode<T> head)
    {
        Head = head;
    }

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

    public void PrintInReverse()
    {
        throw new NotImplementedException();
    }

    public void Print()
    {
        throw new NotImplementedException();
    }

    public void Reverse()
    {
        throw new NotImplementedException();
    }
}