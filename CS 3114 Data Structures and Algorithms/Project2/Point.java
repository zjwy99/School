/**
 * Class for handling points
 * @author Zach
 * @version 1.0
 */
public class Point implements Comparable {
    
    private int xCoordinate;
    private int yCoordinate;
    private String name;
    
    /**
     * Constructor for point class
     * @param x coordinate for the point 
     * @param y coordinate for the point
     * @param name of the point 
     */
    public Point(int x, int y, String name) {
        xCoordinate = x;
        yCoordinate = y;
        this.name = name;
    }
        
    /**
     * @return the x coordinate for the point
     */
    public int getX() {
        return xCoordinate;
    }
    
    /**
     * @return the y coordinate for the point
     */
    public int getY() {
        return yCoordinate;
    }
    
    /**
     * @return the name of the point
     */
    public String getName() {
        return name;
    }
    
    
    /**
     * Converts the information of the point into a String
     * @return string of the information
     */
    public String toString() {
        return getName() + ", " + xCoordinate + ", " + yCoordinate;
    }
    
    
    /**
     * Compares all fields to check for equivalence
     * @param other the object to be compared with this point
     * @return result of the comparison
     */
    public boolean equals(Object other) {
        int checkX = ((Point) other).xCoordinate;
        int checkY = ((Point) other).yCoordinate;
        if (this.xCoordinate == checkX && this.yCoordinate == checkY && 
                this.name.compareTo(((Point) other).name) == 0)
            return true;
        else
            return false;
        
    }

    
    /**
     * Checks only x and y coordiantes for comparison
     * @param o the object to be compared with this point
     * @return result of the comparison
     */
    public int compareTo(Object o) {
        int checkX = ((Point) o).xCoordinate;
        int checkY = ((Point) o).yCoordinate;
        
        if (checkX == this.xCoordinate && checkY == this.yCoordinate)
            return 0;
        else
            return -1;
    }

   

}
