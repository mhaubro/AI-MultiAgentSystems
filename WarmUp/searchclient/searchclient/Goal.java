package searchclient;

public class Goal {
    public char chr;
    public Pair<Integer, Integer> location;
    public Box box;
    public Agent agent;
    public boolean goalState;

    Goal(char chr, Pair<Integer, Integer> location)
    {
        this.chr = chr;
        this.location = location;
        this.box = null;
        this.goalState = false;
    }

    Goal(char chr, Pair<Integer, Integer> location, Box box)
    {
        this.chr = chr;
        this.location = location;
        this.box = box;
        this.goalState = false;
    }

    public void updateBox(Box box)
    {
        this.box = box;
    }

    public void setAgent(Agent a)
    {
        this.agent = a;
    }

    public void updateGoalState(boolean goalState)
    {
        this.goalState = goalState;
    }
}
