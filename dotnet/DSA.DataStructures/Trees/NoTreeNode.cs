namespace DSA.DataStructures.Trees;

public class NoTreeNode : ITreeNode
{
    public static ITreeNode Value { get; } = new NoTreeNode();

    private NoTreeNode()
    {
    }

    public ITreeNode AddLeft(ITreeNode other) => other;

    public ITreeNode AddRight(ITreeNode other) => other;
}