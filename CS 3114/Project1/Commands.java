import java.io.*;
import java.util.*;

/**
 * This class is responsible for parsing the commands from the text file and
 * running those commands
 * 
 * @author Zachary Yee
 * @version 0.1
 */
public class Commands {
    private SkipList<String, Rectangle> list;
    private String filename;

    /**
     * Constructor for Commands class, initializes scanner for reading file
     * 
     * @param filename
     *            the name of the file to be read
     */
    public Commands(String filename) {
        this.filename = filename;
        list = new SkipList<String, Rectangle>();
       

    }

    /**
     * Runs the commands in the given file
     */
    public void runCommands() {

        /*
         * Reads each word from given text file
         */
        try {
            Scanner scan = new Scanner(new File(filename));

            // variables to hold the rectangle data temporarily
            String tempName = "";
            int tempX;
            int tempY;
            int tempW;
            int tempH;
            while (scan.hasNext()) {

                // holds the command name
                String s = scan.next();

                // switch statement that processes each command
                switch (s) {
    
                    /*
                     * Attempts to insert the Rectangle in the SkipList
                     */
                    case "insert":
    
                        /*
                         * Gets the next five inputs that are the name, 
                         * x position, y position, width, 
                         * and height of the rectangle
                         */
                        tempName = scan.next();
                        tempX = scan.nextInt();
                        tempY = scan.nextInt();
                        tempW = scan.nextInt();
                        tempH = scan.nextInt();
    
                        insert(tempName, tempX, tempY, tempW, tempH);
                        break;
    
                    case "remove":
                        tempName = scan.next();
                        if (isNumber(tempName)) {
                            tempX = Integer.parseInt(tempName);
                            tempY = scan.nextInt();
                            tempW = scan.nextInt();
                            tempH = scan.nextInt();
                            removeByCoordinates(tempX, tempY, tempW, tempH);
                        } 
                        else {
                            removeByName(tempName);
                        }
                        break;
    
                    case "regionsearch":
                        tempX = scan.nextInt();
                        tempY = scan.nextInt();
                        tempW = scan.nextInt();
                        tempH = scan.nextInt();
                        regionSearch(tempX, tempY, tempW, tempH);
                        break;
    
                    case "intersections":
                        intersections();
                        break;
    
                    case "search":
                        tempName = scan.next();
                        search(tempName);
                        break;
    
                    case "dump":
                        dump();
                        break;
    
                    default:
                        break;
                }

            }
            scan.close();
        } 
        catch (Exception e) {
            e.printStackTrace();
        }

    }
    
    /**
     * Inserts a rectangle into skip list
     * @param name name of rectangle to be inserted
     * @param x x-position of rectangle to be inserted
     * @param y y-position of rectangle to be inserted
     * @param w width of rectangle to be inserted
     * @param h height of rectangle to be inserted 
     */
    public void insert(String name, int x, int y, int w, int h) {
        Rectangle temp;
        if (check(x, y, w, h) == false)
            System.out.println("Rectangle rejected: (" + name + ", " + x + 
                    ", " + y + ", " + w + ", " + h + ")");
        else {
            System.out.println("Rectangle inserted: (" + name + ", " + x + 
                    ", " + y + ", " + w + ", " + h + ")");
            temp = new Rectangle(name, x, y, w, h);
            list.insert(name, temp);
        }

    }

    /**
     * 
     * @param name name that needs to be removed in skip list
     */
    public void removeByName(String name) {
        SkipNode<String, Rectangle> result = list.remove(name);
        if (result != null) {
            System.out.println("Rectangle removed: (" + 
                    result.element.toString() + ")");
        } 
        else {
            System.out.println("Rectangle not removed: " + name);
        }
    }

    /**
     * 
     * @param x x-position of rectangle that needs to be removed
     * @param y y-position of rectangle that needs to be removed
     * @param w width of rectangle that needs to be removed
     * @param h height of rectangle that needs to be removed
     */
    public void removeByCoordinates(int x, int y, int w, int h) {
        Rectangle tempRect = new Rectangle("", x, y, w, h);
        if (check(x, y, w, h) == false)
            System.out.println("Rectangle rejected: (" + x + ", " + y + ", "
                    + w + ", " + h + ")");
        else {
            SkipNode<String, Rectangle> result = list.remove(tempRect);
            if (result != null) {
                System.out.println("Rectangle removed: (" + 
                        result.element.toString() + ")");
            } 
            else {
                System.out.println("Rectangle not removed: " + 
                        "(" + tempRect.toString().substring(2) + ")");
            }
        }
    }

    /**
     * Searches for name in skip list
     * @param name name to search for in skip list
     */
    public void search(String name) {
        // Gets list of all search results
        SkipNode<String, Rectangle>[] result = list.find(name);
        if (result.length == 1) // If skip list is empty
            System.out.println("Rectangle not found: " + name);
        else { // Prints search results
            for (int i = 0; i < result.length; i++) {
                if (result[i] != null) {
                    System.out.print("(" + result[i].element.getName());
                    System.out.print(", " + result[i].element.getX());
                    System.out.print(", " + result[i].element.getY());
                    System.out.print(", " + result[i].element.getW());
                    System.out.print(", " + result[i].element.getH() + ")\n");
                }
            }
        }

    }

    /**
     * Prints out skip list with depth length and associated element
     */
    public void dump() {
        System.out.println("SkipList dump: ");
        SkipNode<String, Rectangle>[] result2 = list.dump();
        if (result2.length == 1) { // If skip list is empty
            System.out.println("Node has depth 1, Value (null)");
        } 
        else { // Prints all necessary information for each node 
            for (int i = 0; i < result2.length; i++) {
                System.out.print("Node has depth " + 
                        (result2[i].forward.length) + ", Value (");
                if (result2[i].element != null)
                    System.out.print(result2[i].element.toString() + ")\n");
                else
                    System.out.print(result2[i].element + ")\n");

            }
        }
        System.out.println("SkipList size is: " + (result2.length - 1));
    }

    /**
     * Finds intersections and prints them out
     */
    public void intersections() {
        System.out.println("Intersection pairs:");
        
        // Gets a list of all the nodes in the skip list
        SkipNode<String, Rectangle>[] dumpList = list.dump(); 
        for (int i = 1; i < dumpList.length; i++) {
            for (int x = 1; x < dumpList.length; x++) {
                
                /* 
                 * Will make a comparison with all other rectangles
                 * except for itself
                 */
                if (x != i) {
                    Rectangle t1 = dumpList[i].element;
                    Rectangle t2 = dumpList[x].element;
                    
                    // To get coordinates for the right side of the rectangle
                    int totalX1 = t1.getX() + t1.getW();
                    int totalY1 = t1.getY() + t1.getH();
                    int totalX2 = t2.getX() + t2.getW();
                    int totalY2 = t2.getY() + t2.getH();
                    
                    //Prints proper messages if rectangles intersect
                    if (!(totalX1 <= t2.getX() || totalX2 <= t1.getX() || 
                            totalY1 <= t2.getY() || totalY2 <= t1.getY()))
                        System.out.println("(" + t1.toString() + " | " + 
                                t2.toString() + ")");

                }
            }

        }
    }

    /**
     * 
     * @param x x-position for the region search rectangle
     * @param y y-position for the region search rectangle
     * @param w width for the region search rectangle
     * @param h height for the region search rectangle
     */
    public void regionSearch(int x, int y, int w, int h) {
        if (w <= 0 || h <= 0) // Checks for valid region search
            System.out.println("Rectangle rejected: (" + x + ", " + y + ", " +
                    w + ", " + h + ")");
        else {
            System.out.print("Rectangles intersecting region (");
            System.out.print(x + ", " + y + ", " + w + ", " + h + "):\n");
            
            // Gets list of all nodes in skip list
            SkipNode<String, Rectangle>[] dumpList = list.dump();
            Rectangle t1 = new Rectangle("", x, y, w, h);
            for (int i = 1; i < dumpList.length; i++) {
                Rectangle t2 = dumpList[i].element;
                
                // To get coordinates for the right side of the rectangle
                int totalX1 = t1.getX() + t1.getW();
                int totalY1 = t1.getY() + t1.getH();
                int totalX2 = t2.getX() + t2.getW();
                int totalY2 = t2.getY() + t2.getH();
                
                //Prints proper messages if rectangles intersect
                if (!(totalX1 <= t2.getX() || totalX2 <= t1.getX() || 
                        totalY1 <= t2.getY() || totalY2 <= t1.getY()))
                    System.out.println("(" + t2.toString() + ")");
            }
        }

    }

    /**
     * Checks to see if rectangle is valid
     * 
     * @param x
     *            x position of the rectangle to be checked, needs to be zero or
     *            greater to be valid
     * @param y
     *            y position of the rectangle to be checked, needs to be zero or
     *            greater to be valid
     * @param w
     *            width of rectangle, needs to be greater than zero to be valid
     * @param h
     *            height of rectangle, needs to be greater than zero to be valid
     * @return true if rectangle has a valid position and dimensions do not
     *         cause rectangle to go outside of the boundary
     */
    private boolean check(int x, int y, int w, int h) {

        if (x < 0 || x >= 1024 || y < 0 || y >= 1024 || w <= 0 || h <= 0)
            return false;
        else if ((x + w) > 1024 || (y + h) > 1024)
            return false;
        else
            return true;
    }

    /**
     * 
     * @param test string that will be tested
     * @return true if given string is a number, false otherwise
     */
    private boolean isNumber(String test) {

        try {
            Integer.parseInt(test);
            return true;
        } 
        catch (Exception e) {
            return false;
        }

    }

}
