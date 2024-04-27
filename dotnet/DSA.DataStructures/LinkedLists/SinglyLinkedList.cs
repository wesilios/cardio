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
        if (IsEmpty())
        {
            Head = new SinglyLinkedListNode<T>(data);
        }
        else
        {
            var previous = Head;
            while (previous.Next is not null)
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

        if (position == 0)
        {
            AddToHead(data);
            return;
        }

        if (position == Size)
        {
            Add(data);
            return;
        }

        var previous = Head;
        for (var i = 0; i < position; i++)
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