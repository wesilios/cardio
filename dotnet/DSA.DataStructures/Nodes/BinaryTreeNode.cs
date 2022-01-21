namespace DSA.DataStructures.Nodes;

public class BinaryTreeNode<T>
{
    public T Data { get; } 
    public BinaryTreeNode<T>? Left { get; set; }
    public BinaryTreeNode<T>? Right { get; set; }
    
    public BinaryTreeNode(T data)
    {
        Data = data;
    }

    public BinaryTreeNode(T data, BinaryTreeNode<T>? left)
    {
        Data = data;
        Left = left;
    }
    
    public BinaryTreeNode(T data, BinaryTreeNode<T>? left, BinaryTreeNode<T>? right)
    {
        Data = data;
        Left = left;
        Right = right;
    }
}