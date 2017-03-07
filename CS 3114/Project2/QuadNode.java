/**
 * Interface for the nodes in the quadtree
 * @author Zach
 * @version 1.0
 */
public interface QuadNode {
    
    /**
     * Adds a point to the tree
     * @param element to be inserted
     * @param currentX current x value being checked in the tree
     * @param currentY current y value being check in the tree
     * @param split length of the region in the tree
     * @return QuadNode that was searched
     */
    public abstract QuadNode add(Point element, int currentX,
            int currentY, int split);
    

    
    /**
     * Used to create a dump of all the information in the quadtree
     * @param currentX current x value being checked in the tree
     * @param currentY current y value being checked in the tree
     * @param split length of a region in the tree
     * @param list the contents of the quadtree in string format
     * @param numOfIndents used to create the spacing in the quadtree
     * @param numOfVisits array used to hold number of nodes visited
     * @return list of the dump 
     */
    public abstract LinkedList<String> getContents(int currentX, int currentY,
            int split, LinkedList<String> list, int numOfIndents, 
            int[] numOfVisits);
    
    /**
     * Finds all duplicates in the quadtree. Only shows one instance of point
     * @param list of all duplicate points in quadtree in string format
     * @return the list of all duplicate points
     */
    public abstract LinkedList<String> findDuplicates(LinkedList<String> list);
    
    /**
     * Removes one point from quadtree given an xy coordinate
     * @param x x coordinate of point to be removed
     * @param y y coordinate of point to be removed
     * @param currentX current x value being checked in the tree
     * @param currentY current y value being checked in the tree
     * @param split length of the region in the tree
     * @param removed point that was removed, was put in array in order 
     *                for java to pass it by reference
     * @return current quadnode being searched 
     */
    public abstract QuadNode remove(int x, int y, int currentX,
            int currentY, int split, Point[] removed);
    
    /**
     * Removes one point from quadtree given the point itself
     * @param toRemove point to be removed
     * @param currentX current x value being checked in the tree
     * @param currentY current y value being checked in the tree
     * @param split length of the region in the tree
     * @param removed point that was removed, was put in array in order 
     *                for java to pass it by reference
     * @return current quadnode being checked
     */
    public abstract QuadNode remove(Point toRemove, int currentX,
            int currentY, int split, Point[] removed);
    
    /**
     Searches for all points given a rectangle
     * @param x x coordinate for region to be searched
     * @param y y coordinate for region to be searched
     * @param width of the region to be searched
     * @param height of the region to be searched
     * @param result list of all points in region
     * @param currentX current x value being checked in the tree
     * @param currentY current y value being checked in the tree
     * @param split length of the region in the tree
     * @param numOfVisits number of nodes visited while searching
     * @return list of all points in the region
     */
    public abstract LinkedList<Point> regionSearch(int x, int y, int width,
            int height, LinkedList<Point> result, int currentX, int currentY,
            int split, int[] numOfVisits); 
    
   
   

}

