package searchclient;

/**
 * Source: https://stackoverflow.com/questions/521171/a-java-collection-of-value-pairs-tuples
 * @param <L>
 * @param <R>
 */
public class Pair<L,R>
{
    private final L left;
    private final R right;

    public Pair(L left, R right)
    {
        this.left = left;
        this.right = right;
    }

    public L getLeft() { return left; }
    public R getRight() { return right; }

   public boolean equals(Pair<L,R> loc2){
       return this.left == loc2.getLeft() && this.right == loc2.getRight(); 
   }
}
