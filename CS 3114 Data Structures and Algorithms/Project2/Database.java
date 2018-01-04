/**
 * This class handles the various commands for modifying both a skip list and
 * a quadtree.
 * @author Zach
 * @version 1.0
 */
        
public class Database {
    private SkipList<String, Point> list; //contains the skip list
    private QuadTree tree; // contains the quadtree
    
    /**
     * Default constructor that initializes the skip list and quadtree
     */
    public Database() {
        list = new SkipList<String, Point>();
        tree = new QuadTree();
    }
    
    /**
     * Inserts a point into both the skip list and quadtree
     * @param element Point to be inserted
     */
    public void insert(Point element) {
        list.insert(element.getName(), element);
        tree.insert(element);
    }
    
    /**
     * Removes a point given an xy coordinate
     * @param x x coordinate for point to be removed
     * @param y y coordinate for point to be removed
     * @return Point that was removed, null if no point removed
     */
    public Point removeByPoint(int x, int y) {
        
        Point toRemove = tree.remove(x, y);
        if (toRemove == null)
            return null;
        list.remove(toRemove);
        return toRemove;
    }
    
    /**
     * Removes a point given a name 
     * @param name name that needs to be removed in skip list
     * @return the result of the removal 
     */
    public Point removeByName(String name) {
        SkipNode<String, Point> result = list.remove(name);
        if (result != null) {
            tree.remove(result.element);
            return result.element;
        } 
        else {
            return null;
        }
    }
    
    /**
     * Returns a dump for both the skip list and the quadtree
     * @return A linked list of both skip list and quadtrees dump
     */
    public LinkedList<String> dump() {
        LinkedList<String> result = new LinkedList<String>();
        result.insert("SkipList dump:");
        SkipNode<String, Point>[] result2 = list.dump();
        if (result2.length == 1) {
            result.insert("Node has depth 1, Value (null)");
        } 
        else {
            for (int i = 0; i < result2.length; i++) {
                
                if (result2[i].element != null) {
                    
                    result.insert("Node has depth " + 
                            (result2[i].forward.length) + 
                            ", Value (" + 
                            result2[i].element.toString() + ")");
                }
                else {
                    result.insert("Node has depth " + 
                            (result2[i].forward.length) + ", Value (" 
                            + result2[i].element + ")");
                }

            }
        }
        result.insert("SkipList size is: " + (result2.length - 1));
        LinkedList<String> treeDump = tree.dump();
        result.insert("QuadTree dump:");
        for (int i = 0; i < treeDump.getSize(); i++) {
            result.insert(treeDump.get(i));
        }
        return result;
    }
    
    /**
     * Searches for a point given a name. Only uses skip list for searching
     * @param name name to search for in skip list
     * @return a list of all found points
     */
    public LinkedList<String> search(String name) {
        SkipNode<String, Point>[] result = list.find(name);
        LinkedList<String> toReturn = new LinkedList<String>();
        if (result.length == 1)
            toReturn.insert("Point not found: " + name);
        else {
            for (int i = 0; i < result.length; i++) {
                if (result[i] != null) {
                    toReturn.insert("Found (" + result[i].key() + 
                            ", " + result[i].element().getX() + 
                            ", " + result[i].element.getY() + ")");
                }
            }
        }
        return toReturn;

    }
    
    /**
     * Searches for all points given a rectangle
     * @param x x coordinate for region to be searched
     * @param y y coordinate for region to be searched
     * @param width of the region to be searched
     * @param height of the region to be searched
     * @return a list of all points in the rectangle
     */
    public LinkedList<String> regionSearch(int x, int y, 
            int width, int height) {
        int[] numOfVisits = {0};
        LinkedList<Point> result = 
                tree.regionSearch(x, y, width, height, numOfVisits);
        LinkedList<String> output = new LinkedList<String>();
        output.insert("Points intersecting region (" + x + ", " + y + ", "
                + width + ", " + height + "):");
        for (int i = 0; i < result.getSize(); i++) {
            output.insert("Point found: (" + result.get(i).toString() + ")");
        }
        output.insert(numOfVisits[0] + " quadtree nodes visited");
        return output;
    }
    
    /**
     * @return a list of all the duplicate points that were inserted
     */
    public LinkedList<String> duplicates() {
        return tree.duplicates();
    }
    
    /**
     * Checks to see if point is valid.
     * x and y must be greater than or equal to zero and less than 1024
     * @param x x coordinate of the point
     * @param y y coordinate of the point
     * @return Result of the check
     */
    public boolean pointCheck(int x, int y) {
        return (x < 1024 && x >= 0 && y < 1024 && y >= 0);
            
    }
    
   
    
    

}
