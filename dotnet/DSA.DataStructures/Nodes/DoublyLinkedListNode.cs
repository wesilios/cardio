namespace DSA.DataStructures.Nodes;

public class DoublyLinkedListNode<T>
{
    public T Data { get; }
    public SinglyLinkedListNode<T>? Next { get; set; }
    public SinglyLinkedListNode<T>? Previous { get; set; }

    public DoublyLinkedListNode(T data)
    {
        Data = data;
    }

    public DoublyLinkedListNode(T data, SinglyLinkedListNode<T>? next)
    {
        Data = data;
        Next = next;
    }
    
    public DoublyLinkedListNode(T data, SinglyLinkedListNode<T>? next, SinglyLinkedListNode<T>? previous)
    {
        Data = data;
        Next = next;
        Previous = previous;
    }
}