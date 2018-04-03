package searchclient;

public class Box {
    public char chr;
    public Pair<Integer, Integer> location;
    public String color;

    Box(char chr, Pair<Integer, Integer> location)
    {
        this.chr = chr;
        this.location = location;
    }

    Box(Box box){
        this.chr = box.chr;
        this.location = box.location;
    }

    public void setLocation(Pair<Integer, Integer> location)
    {
        // Check if we are moving box away from goal
        for(Goal g : Node.goals){
            if(this.location.equals(g.location)){
                g.goalState = false;
            } 
        }
        this.location = location;
        // Check if we are mocing box onto goal
        for(Goal g : Node.goals){
            if(this.location.equals(g.location)){
                g.goalState = true;
            } 
        }
    }

    public boolean equals(Box box){
        return this.chr == box.chr && this.location.equals(box.location);
    } 
}
