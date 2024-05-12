using System.Text;
using DSA.DataStructures.Nodes;

namespace DSA.DataStructures.LinkedLists;

public class SinglyLinkedList<T> : ISinglyLinkedList<T>
{
    public int Size { get; set; } = 0;
    public SinglyLinkedListNode<T>? Head { get; set; } = null;

    public void Add(T data)
    {
        if (IsEmpty())
        {
            Head = new SinglyLinkedListNode<T>(data);
        }
        else
        {
            var previous = Head;
            while (previous!.Next is not null)
            {
                previous = previous.Next;
            }

            previous.SetNextNode(new SinglyLinkedListNode<T>(data));
        }

        Size++;
    }

    public void AddToHead(T data)
    {
        if (IsEmpty())
        {
            Head = new SinglyLinkedListNode<T>(data);
        }
        else
        {
            var newHead = new SinglyLinkedListNode<T>(data, Head);
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

        var newNode = new SinglyLinkedListNode<T>(data, previous!.Next);
        previous.SetNextNode(newNode);
        Size++;
    }

    public void DeleteNodeAtPosition(int position)
    {
        throw new NotImplementedException();
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
        SinglyLinkedListNode<T>? previous = null;
        var current = Head;
        while (current is not null)
        {
            var next = current.Next;
            current.SetNextNode(previous);
            previous = current;
            current = next;
        }

        Head = previous;
    }
}