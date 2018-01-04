// On my honor: 
//
// - I have not used source code obtained from another student, 
// or any other unauthorized source, either modified or 
// unmodified. 
//
// - All source code and documentation used in my program is
// either my original work, or was derived by me from the 
// source code published in the textbook for this course. 
// 
// - I have not discussed coding details about this project with 
// anyone other than my partner (in the case of a joint 
// submission), instructor, ACM/UPE tutors or the TAs assigned 
// to this course. I understand that I may discuss the concepts 
// of this program with other students, and that another student 
// may help me debug my program so long as neither of us writes 
// anything during the discussion or modifies any computer file
// during the discussion. I have violated neither the spirit nor
// letter of this restriction.
/**
 * { your description of the project here }
 */

/**
 * The class containing the main method, the entry point of the application.
 *
 * @author Zachary Yee
 * @version 1.0
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 * 
 * @author Zach
 * @version 1.0
 */
public class Point2
{

    /**
     * The entry point of the application.
     *
     * @param args
     *            The name of the command file passed in as a command line
     *            argument.
     */
    public static void main(String[] args)
    {   
        
        LinkedList<String> result;
        Point tempPoint;
        String filename = args[0];
        Database database = new Database();
        Scanner scan = null;
        try {
            scan = new Scanner(new File(filename));
            while (scan.hasNext()) {
                
                String s = scan.next();
                String tempName = "";
                int tempX;
                int tempY;
                int tempW;
                int tempH;
                switch (s) {
                    
                    case "insert":
                        
                        tempName = scan.next();
                        tempX = scan.nextInt();
                        tempY = scan.nextInt();
                        Point temp = new Point(tempX, tempY, tempName);
                        
                        if (database.pointCheck(tempX, tempY)) {
                            System.out.println("Point inserted: (" + 
                                    temp.toString() + ")");
                            database.insert(temp);
                        }
                        else 
                            System.out.println("Point rejected: (" +
                                    temp.toString() + ")");
                        
                        
                        break;
                    case "remove":
                        tempName = scan.next();
                        if (isNumber(tempName)) {
                            tempX = Integer.parseInt(tempName);
                            tempY = scan.nextInt();
                            if (!database.pointCheck(tempX, tempY)) {
                                System.out.println("Point rejected: (" + 
                                        tempX + ", " + tempY + ")");
                            }
                            else {
                                tempPoint = database.removeByPoint(tempX, 
                                        tempY);
                                if (tempPoint == null)
                                    System.out.println("Point not found: (" + 
                                            tempX + ", " + tempY + ")");
                                else {
                                    System.out.println("Point removed: (" + 
                                            tempPoint.toString() + ")");
                                }
                            }
                            
                        } 
                        else {
                            tempPoint = database.removeByName(tempName);
                            if (tempPoint == null)
                                System.out.println("Point not removed: " 
                                        + tempName);
                            else {
                                System.out.println("Point removed: (" + 
                                        tempPoint.toString() + ")");
                            }
                        }
                        break;
                    case "regionsearch":
                        tempX = scan.nextInt();
                        tempY = scan.nextInt();
                        tempW = scan.nextInt();
                        tempH = scan.nextInt();
                        if (tempW > 0
                                && tempH > 0) {
                            result = database.regionSearch(tempX, tempY,
                                    tempW, tempH);
                            for (int i = 0; i < result.getSize(); i++)
                                System.out.println(result.get(i));
                        }
                        else {
                            System.out.println("Rectangle rejected: (" + 
                                    tempX + ", " + tempY + ", " +
                                    tempW + ", " + tempH + ")");
                        }
                        break;
                    case "search":
                        tempName = scan.next();
                        result = database.search(tempName);
                        for (int i = 0; i < result.getSize(); i++) {
                            System.out.println(result.get(i));
                        }
                        break;
                    case "duplicates":
                        System.out.println("Duplicate points:");
                        result = database.duplicates();
                        for (int i = 0; i < result.getSize(); i++) {
                            System.out.println(result.get(i));
                        }
                        break;
                    
                    case "dump":
                        result = database.dump();
                        for (int i = 0; i < result.getSize(); i++)
                            System.out.println(result.get(i));
                        break;
                    default:
                        break;
                }
            }
        } 
        catch (FileNotFoundException e) {
            
            e.printStackTrace();
        }
      
    }    
    /**
     * 
     * @param test
     * @return true if given string is a number, false otherwise
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
