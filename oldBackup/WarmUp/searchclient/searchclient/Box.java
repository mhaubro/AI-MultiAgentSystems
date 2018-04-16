package searchclient;

public class Box {
    public char chr;
    public Pair<Integer, Integer> location;
    public String color;

    Box(char chr, Pair<Integer, Integer> location, String color)
    {
        this.chr = chr;
        this.location = location;
        this.color = color;
    }

    Box(char chr, Pair<Integer, Integer> location)
    {
        this.chr = chr;
        this.location = location;
        this.color = "";
    }

    Box(Box box){
        this.chr = box.chr;
        this.location = box.location;
        this.color = "";
    }

    public void setLocation(Pair<Integer, Integer> location)
    {
        this.location = location;
    }

    @Override
    public int hashCode()
    {
        int result = 17;
        result = 31 * result + Character.getNumericValue(chr);
        result = 31 * result + location.getLeft();
        result = 31 * result + location.getRight();
        result = 31 * result + color.hashCode();
        return result;
    }

    @Override
    public boolean equals(Object o)
    {
        if (o == this) return true;
        if (!(o instanceof Box)) {
            return false;
        }

        Box box = (Box) o;

        return box.chr == chr && box.color.equals(color) && box.location.equals(location);
    } 
}
