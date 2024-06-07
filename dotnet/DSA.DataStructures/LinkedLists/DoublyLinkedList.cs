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
        throw new NotImplementedException();
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
        throw new NotImplementedException();
    }
}