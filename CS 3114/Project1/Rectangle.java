/**
 * This class is responsible for holding information about Rectangles
 * 
 * @author Zachary Yee
 * @version 1.0
 */

public class Rectangle {

    /*
     * Variables to hold information about the particular Rectangle
     */
    private int x;
    private int y;
    private int width;
    private int height;
    private String name;

    /**
     * Creates a new Rectangle object.
     * 
     * @param name
     *            stores the name of the rectangle in order to identify itself
     * @param x
     *            stores the x position of the rectangle
     * @param y
     *            stores the y position of the rectangle,
     * @param w
     *            stores the width of the rectangle,
     * @param h
     *            stores the height of the rectangle
     */
    public Rectangle(String name, int x, int y, int w, int h) {
        this.name = name;
        this.x = x;
        this.y = y;
        this.width = w;
        this.height = h;
    }

    /**
     * 
     * @return name the name of the rectangle
     */
    public String getName() {
        return this.name;
    }

    /**
     * 
     * @return x the x position of the rectangle
     */
    public int getX() {
        return this.x;
    }

    /**
     * 
     * @return y the y position of the rectangle
     */
    public int getY() {
        return this.y;
    }

    /**
     * 
     * @return width the width of the rectangle
     */
    public int getW() {
        return this.width;
    }

    /**
     * 
     * @return height the height of the rectangle
     */
    public int getH() {
        return this.height;
    }

    /**
     * @return string of necessary info
     */
    public String toString() {
        return name + ", " + x + ", " + y + ", " + width + ", " + height;
    }

    @Override
    /**
     * @param other object that is supposed to be a rectangle
     * @return true if this rectangle and other rectangle have same dimensions
     */
    public boolean equals(Object other) {
        int checkX = ((Rectangle) other).getX();
        int checkY = ((Rectangle) other).getY();
        int checkW = ((Rectangle) other).getW();
        int checkH = ((Rectangle) other).getH();
        if (this.x == checkX && this.y == checkY && 
                this.width == checkW && this.height == checkH)
            return true;
        else
            return false;
    }

}
