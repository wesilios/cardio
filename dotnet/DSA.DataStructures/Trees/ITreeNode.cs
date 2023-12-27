namespace DSA.DataStructures.Trees;

public interface ITreeNode
{
    ITreeNode AddLeft(ITreeNode other);
    ITreeNode AddRight(ITreeNode other);
    public static ITreeNode Null => NoTreeNode.Value;
}