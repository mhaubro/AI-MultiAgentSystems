package searchclient;

public class Agent
{
    public int num;
    public int rank;
    public Pair<Integer, Integer> location;
    public String color;

    public Agent(int num, Pair<Integer, Integer> location)
    {
        this.num = num;
        this.rank = 0;
        this.location = location;
        this.color = null;
    }

    public void setLocation(Pair<Integer, Integer> location)
    {
        if(location.getLeft() == null && location.getRight() == null)
            throw new NullPointerException("Cannot be null");
        this.location = location;
    }
}
