namespace DSA.DataStructures.Nodes;

public class SinglyLinkedListNode<T>
{
    public T Data { get; }
    public SinglyLinkedListNode<T>? Next { get; private set; }

    public SinglyLinkedListNode(T data)
    {
        Data = data;
        Next = null;
    }

    public SinglyLinkedListNode(T data, SinglyLinkedListNode<T>? next)
    {
        Data = data;
        Next = next;
    }

    public void SetNextNode(SinglyLinkedListNode<T>? next)
    {
        Next = next;
    }
}