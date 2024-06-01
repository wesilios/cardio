namespace DSA.DataStructures.Nodes;

public class DoublyLinkedListNode<T>
{
    public T Data { get; }
    public DoublyLinkedListNode<T>? Next { get; private set; }
    public DoublyLinkedListNode<T>? Previous { get; private set; }

    public DoublyLinkedListNode(T data)
    {
        Data = data;
    }

    public DoublyLinkedListNode(T data, DoublyLinkedListNode<T>? next)
    {
        Data = data;
        next?.SetPreviousNode(this);
    }
    
    public DoublyLinkedListNode(T data, DoublyLinkedListNode<T>? next, DoublyLinkedListNode<T>? previous)
    {
        Data = data;
        next?.SetPreviousNode(this);
        previous?.SetNextNode(this);
    }
    
    public void SetNextNode(DoublyLinkedListNode<T>? next)
    {
        Next = next;
        next?.SetPreviousNode(this);
    }
    
    public void SetPreviousNode(DoublyLinkedListNode<T>? previous)
    {
        Previous = previous;
        previous?.SetNextNode(this);
    }
}