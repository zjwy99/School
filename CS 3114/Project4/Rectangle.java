/**
 * Rectangle class to hold information about a rectangle
 * @author Zach-Laptop
 * @version 1.0
 */
public class Rectangle implements java.io.Serializable {

    
    private int x;
    private int y;
    private int width;
    private int height;
    private String name;

    /**
     * Constructor
     * @param name name of the rectangle
     * @param x x position of the rectangle
     * @param y y position of the rectangle
     * @param w width of the rectangle
     * @param h height of the rectangle
     */
    public Rectangle(String name, int x, int y, int w, int h) {
        this.name = name;
        this.x = x;
        this.y = y;
        this.width = w;
        this.height = h;
    }

    /**
     * @return name of the rectangle
     */
    public String getName() {
        return this.name;
    }

    /**
     * @return x position of the rectangle
     */
    public int getX() {
        return this.x;
    }

    /**
     * @return y position of the rectangle
     */
    public int getY() {
        return this.y;
    }

    /**
     * @return width of the rectangle
     */
    public int getW() {
        return this.width;
    }

    /**
     * @return height of the rectangle
     */
    public int getH() {
        return this.height;
    }

    /**
     * Returns information about Rectangle as a string
     * @return string of information
     */
    public String toString() {
        return getName() + ", " + x + ", " + y + ", " + width + ", " + height;
    }

    /**
     * Sees if Rectangle objects are the same
     * @param other Object to be compared
     * @return true if equal, false otherwise
     */
    public boolean equals(Object other) {
        int checkX = ((Rectangle) other).getX();
        int checkY = ((Rectangle) other).getY();
        int checkW = ((Rectangle) other).getW();
        int checkH = ((Rectangle) other).getH();
        return (this.x == checkX && this.y == checkY && 
                this.width == checkW && this.height == checkH);
      
    }

}
