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

    public void setLocation(Pair<Integer, Integer> location)
    {
        this.location = location;
    }
}
