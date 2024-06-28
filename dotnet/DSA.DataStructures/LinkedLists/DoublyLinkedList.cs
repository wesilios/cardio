using System.Text;
using DSA.DataStructures.Nodes;

namespace DSA.DataStructures.LinkedLists;

public class DoublyLinkedList<T> : IDoublyLinkedList<T>
{
    private int Size { get; set; } = 0;
    private DoublyLinkedListNode<T>? Head { get; set; } = null;
    private DoublyLinkedListNode<T>? Tail { get; set; } = null;

    public void Add(T data)
    {
        if (IsEmpty())
        {
            Head = new DoublyLinkedListNode<T>(data);
            Tail = Head;
        }
        else
        {
            var next = new DoublyLinkedListNode<T>(data);
            Tail!.SetNextNode(next);
            next.SetPreviousNode(Tail);
            Tail = next;
        }

        Size++;
    }

    public void AddToHead(T data)
    {
        if (IsEmpty())
        {
            Head = new DoublyLinkedListNode<T>(data);
            Tail = Head;
        }
        else
        {
            var newHead = new DoublyLinkedListNode<T>(data, Head);
            Head = newHead;
        }

        Size++;
    }

    public void InsertNodeToPosition(T data, int position)
    {
        if (position < 0)
        {
            throw new ArgumentException("Illegal position");
        }

        if ((position == 0 && IsEmpty()) || position == 0 && !IsEmpty())
        {
            AddToHead(data);
            return;
        }

        if (position == Size)
        {
            Add(data);
            return;
        }

        if (position > Size)
        {
            throw new ArgumentOutOfRangeException(nameof(position));
        }

        var previous = Head;
        for (var i = 1; i < position - 1; i++)
        {
            previous = previous!.Next;
        }

        var newNode = new DoublyLinkedListNode<T>(data, previous!.Next);
        previous.SetNextNode(newNode);
        newNode.SetPreviousNode(previous);
        Size++;
    }

    public void DeleteNodeAtPosition(int position)
    {
        if (position < 0)
        {
            throw new ArgumentException("Illegal position");
        }

        if (position == 0 && IsEmpty())
        {
            return;
        }

        if (position == 0)
        {
            Head = Head!.Next;
            return;
        }

        if (position >= Size)
        {
            throw new ArgumentOutOfRangeException(nameof(position));
        }

        DoublyLinkedListNode<T>? previous = null;
        var current = Head;
        for (var i = 0; i < position; i++)
        {
            previous = current;
            current = current!.Next;
        }

        previous!.SetNextNode(current!.Next);
        current.SetPreviousNode(previous);
        Size--;
    }

    public bool IsEmpty()
    {
        return Size == 0;
    }

    public string Print()
    {
        if (IsEmpty()) return string.Empty;
        var stringBuilder = new StringBuilder();
        var node = Head;
        while (node is not null)
        {
            if (node.Next is null)
            {
                stringBuilder.Append($"[{node.Data}]");
            }
            else
            {
                stringBuilder.Append($"[{node.Data}] -> ");
            }

            node = node.Next;
        }

        return stringBuilder.ToString();
    }

    public void Reverse()
    {
        if (IsEmpty()) return;
        DoublyLinkedListNode<T>? previous = null;
        var current = Head;
        while (current is not null)
        {
            var next = current.Next;
            current.SetNextNode(previous);
            current.SetPreviousNode(next);
            previous = current;
            current = next;
        }
        
        (Head, Tail) = (Tail, Head);
    }
}