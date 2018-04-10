package searchclient;

public class Agent
{
    public int num;
    public int rank;
    public Pair<Integer, Integer> location;
    public String color;

    public Agent(int num, int rank, Pair<Integer, Integer> location, String color)
    {
        this.num = num;
        this.rank = rank;
        this.location = location;
        this.color = color;
    }

    public Agent(int num, Pair<Integer, Integer> location, String color)
    {
        this.num = num;
        this.rank = 0;
        this.location = location;
        this.color = color;
    }

    public Agent(int num, Pair<Integer, Integer> location)
    {
        this.num = num;
        this.rank = 0;
        this.location = location;
        this.color = "";
    }

    public Agent(Agent agt)
    {
        this.num = agt.num;
        this.rank = 0;
        this.location = agt.location;
        this.color = "";
    }

    @Override
    public int hashCode()
    {
        int result = 17;
        result = 31 * result + num;
        result = 31 * result + rank;
        result = 31 * result + location.getLeft();
        result = 31 * result + location.getRight();
        result = 31 * result + color.hashCode();
        return result;
    }

    @Override
    public boolean equals(Object o)
    {
        if (o == this) return true;
        if (!(o instanceof Agent)) {
            return false;
        }

        Agent agent = (Agent) o;

        return agent.num == num && agent.rank == rank && agent.color.equals(color) && agent.location.equals(location);
    }


    public void setLocation(Pair<Integer, Integer> location)
    {
        if(location.getLeft() == null && location.getRight() == null)
            throw new NullPointerException("Cannot be null");
        this.location = location;
    }
}
