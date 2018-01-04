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
 * Main file for merge sort
 * @author Zach-Laptop
 * @version 2.0 
 */
public class Mergesort {
    
    /**
     * Main method for merge sort
     * @param args arguments for program
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        ExternalMergeSort test = new 
                ExternalMergeSort(args[0], Integer.parseInt(args[1]));
        Statistics stat = new Statistics();
        long maxSortTime = System.currentTimeMillis();
        test.sort();
        maxSortTime = System.currentTimeMillis() - maxSortTime;
        stat.executionTime = maxSortTime;
        stat.writeToFile(args[2], args[0]);
        
    }

}
