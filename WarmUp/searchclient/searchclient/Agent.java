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

    public Agent(Agent agt)
    {
        this.num = agt.num;
        this.rank = 0;
        this.location = agt.location;
        this.color = null;
    }

    @Override
    public int hashCode(){
        return (this.location.getLeft()+this.location.getRight())*31;
    }

    public void setLocation(Pair<Integer, Integer> location)
    {
        if(location.getLeft() == null && location.getRight() == null)
            throw new NullPointerException("Cannot be null");
        this.location = location;
    }
}
