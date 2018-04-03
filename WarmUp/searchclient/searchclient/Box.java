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
        this.location = location;
    }

    @Override
    public int hashCode(){
        return (this.location.getLeft()+this.location.getRight() + (int)this.chr)*31;
    }

    public boolean equals(Box box){
        return this.location.equals(box.location) && this.chr == box.chr;
    } 
}
