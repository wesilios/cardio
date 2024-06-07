namespace DSA.DataStructures.LinkedLists;

public interface ILinkedList<in T>
{
    void Add(T data);
    void AddToHead(T data);
    void InsertNodeToPosition(T data, int position);
    void DeleteNodeAtPosition(int position);
    bool IsEmpty();
    string Print();
    void Reverse();
}