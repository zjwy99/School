import java.io.File;
import java.io.IOException;
import java.util.Scanner;

/**
 * Class used to handle commands given in a file
 * @author Zachary Yee
 * @version 2.0
 */
public class Commands {
    
    private Database db;
    
    /**
     * Runs all commands in command file
     * @param args command arguments used to run program
     * @throws IOException
     */
    public void run(String[] args) throws IOException {
        db = new Database(args[1], Integer.parseInt(args[2]),
                Integer.parseInt(args[3]));
        try {
            Scanner scan = new Scanner(new File(args[0]));

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
    
                    
                    case "insert":

                        tempName = scan.next();
                        tempX = scan.nextInt();
                        tempY = scan.nextInt();
                        tempW = scan.nextInt();
                        tempH = scan.nextInt();
    
                        insert(tempName, tempX, tempY, tempW, tempH);
                        break;
                        
                    case "intersections":
                        intersections();
                        break;
                        
                    case "regionsearch":
                        tempX = scan.nextInt();
                        tempY = scan.nextInt();
                        tempW = scan.nextInt();
                        tempH = scan.nextInt();
                        regionSearch(tempX, tempY, tempW, tempH);
                        break;
                        
                    case "search":
                        tempName = scan.next();
                        search(tempName);
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

    
                    case "dump":
                        db.dump();
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
        db.flushList();
    }
    
    /**
     * Inserts rectangle into skip list
     * @param name name of rectangle
     * @param x x position of rectangle
     * @param y y position of rectangle
     * @param w width of rectangle
     * @param h height of rectangle
     * @throws IOException 
     * @throws ClassNotFoundException 
     */
    public void insert(String name, int x, int y, int w, int h)
        throws ClassNotFoundException, IOException {
        Rectangle temp;
        if (!db.check(x, y, w, h)) {
            System.out.println("Rectangle rejected: (" + name + ", " + x + 
                    ", " + y + ", " + w + ", " + h + ")");
        }
        else {
            System.out.println("Rectangle inserted: (" + name + ", " + x + 
                    ", " + y + ", " + w + ", " + h + ")");
            temp = new Rectangle(name, x, y, w, h);
            db.insert(name, temp);
        }

    }
    
    /**
     * Search for all rectangles with a given name
     * @param key name of rectangles to search for
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void search(String key) throws ClassNotFoundException,
            IOException {
        String[] result = db.searchAll(key);
        if (result.length == 1) {
            System.out.println("Rectangle not found: " + key);
        }
        else {
            for (int i = 0; i < result.length; i++) {
                if (result[i] != null) {
                    System.out.println("(" + result[i] + ")");
                }
            }
        }
    }
    
    /**
     * Removes a rectangle in list given a name
     * @param name name that needs to be removed in skip list
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void removeByName(String name) 
        throws ClassNotFoundException, IOException {
        Rectangle result = db.remove(name);
        if (result != null) {
            System.out.println("Rectangle removed: (" + 
                    result.toString() + ")");
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
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void removeByCoordinates(int x, int y, int w, int h) 
        throws ClassNotFoundException, IOException {
        Rectangle tempRect = new Rectangle("", x, y, w, h);
        if (!db.check(x, y, w, h)) {
            System.out.println("Rectangle rejected: (" + x + ", " + y + ", "
                    + w + ", " + h + ")");
        }
        else {
            Rectangle result = db.remove(tempRect);
            if (result != null) {
                System.out.println("Rectangle removed: (" + 
                        result.toString() + ")");
            } 
            else {
                System.out.println("Rectangle not removed: " + 
                        "(" + tempRect.toString().substring(2) + ")");
            }
        }
    }
    
    /**
     * Finds intersections and prints them out
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void intersections() throws ClassNotFoundException, IOException {
        System.out.println("Intersection pairs:");
        Rectangle[] rList = new Rectangle[db.listSize()];
        for (int i = 0; i < rList.length; i++) {
            rList[i] = db.get(i);
        }
        for (int i = 0; i < db.listSize(); i++) {
            for (int x = i; x < db.listSize(); x++) {
                if (x != i) {
                    Rectangle t1 = rList[i];
                    Rectangle t2 = rList[x];
                    
                 // To get coordinates for the right side of the rectangle
                    int totalX1 = t1.getX() + t1.getW();
                    int totalY1 = t1.getY() + t1.getH();
                    int totalX2 = t2.getX() + t2.getW();
                    int totalY2 = t2.getY() + t2.getH();
                    
                    
                    
                    //Prints proper messages if rectangles intersect
                    if (t2.getX() <= totalX1 &&  totalX2 >= t1.getX() &&
                            t2.getY() <= totalY1 && 
                            totalY2 >= t1.getY()) {
                        System.out.println("(" + t1.toString() + " | " + 
                                t2.toString() + ")");
                    }
                    
                }       
            }
        }        
    }
    
    /**
     * Searches for all rectangles in a given region
     * @param x x-position for the region search rectangle   
     * @param y y-position for the region search rectangle 
     * @param w width for the region search rectangle
     * @param h height for the region search rectangle
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void regionSearch(int x, int y, int w, int h)
        throws ClassNotFoundException, IOException {
        if (w <= 0 ||
                h <= 0) { // Checks for valid region search
            System.out.println("Rectangle rejected: (" + x + ", " + y + ", "
                    + w + ", " + h + ")");
        }
        else {
            System.out.print("Rectangles intersecting region (");
            System.out.print(x + ", " + y + ", " + w + ", " + h + "):\n");

            // Gets list of all nodes in skip list

            Rectangle t1 = new Rectangle("", x, y, w, h);
            for (int i = 1; i < db.listSize(); i++) {
                Rectangle t2 = db.get(i);

                
                // To get coordinates for the right side of the rectangle
                int totalX1 = t1.getX() + t1.getW();
                int totalY1 = t1.getY() + t1.getH();
                int totalX2 = t2.getX() + t2.getW();
                int totalY2 = t2.getY() + t2.getH();

                // Prints proper messages if rectangles intersect
                if ((totalX1 >= t2.getX() && totalX2 >= t1.getX()
                        && totalY1 >= t2.getY() && totalY2 >= t1.getY())) {
                    System.out.println("(" + t2.toString() + ")");
                }
            }
        }

    }
    
    

    /**
     * Tests if string is a number
     * @param test string to test
     * @return true if it is a number, false otherwise
     */
    private static boolean isNumber(String test) {

        try {
            Integer.parseInt(test);
            return true;
        } 
        catch (Exception e) {
            return false;
        }

    }

}
