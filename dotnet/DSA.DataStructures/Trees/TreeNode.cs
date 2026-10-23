namespace DSA.DataStructures.Trees;

public class TreeNode : IComparable<TreeNode>, ITreeNode
{
    public int Data { get; set; }
    public ITreeNode Left { get; set; }
    public ITreeNode Right { get; set; }

    public TreeNode(int data, ITreeNode? left = null, ITreeNode? right = null)
    {
        Data = data;
        Left = left ?? ITreeNode.Null;
        Right = right ?? ITreeNode.Null;
    }

    public ITreeNode AddLeft(ITreeNode other)
    {
        while (true)
        {
            if (other is not TreeNode treeNode) return other.AddLeft(this);
            other = treeNode;
        }
    }

    public ITreeNode AddRight(ITreeNode other)
    {
        while (true)
        {
            if (other is not TreeNode treeNode) return other.AddRight(this);
            other = treeNode;
        }
    }

    public int CompareTo(TreeNode? other)
    {
        if (ReferenceEquals(this, other)) return 0;
        if (ReferenceEquals(null, other)) return 1;
        return Data.CompareTo(other.Data);
    }
}