/**
 * Implementation of a quadtree that handles points
 * @author Zach
 * @version 1.0
 */
public class QuadTree {

    /*
     * a pointer to an empty node all empty nodes are
     * pointed to this 
     */
    private QuadNode flynode; 
    private QuadNode root; //the begining of the tree
    private int numOfElements; //number of elements in the tree
    

    /**
     * Default constructor for quadtree, initializes tree
     */
    public QuadTree() {
        flynode = new EmptyNode();
        root = flynode;
        numOfElements = 0;
    }
    
    /**
     * Inserts an element into the tree
     * @param element to add into the tree
     */
    public void insert(Point element) {
        root = root.add(element, 0, 0, 1024);
        numOfElements++;
    }

    /**
     * Removes an element from the tree
     * @param x x coordinate of point to remove
     * @param y y coordinate of point to remove
     * @return result of the 
     */
    public Point remove(int x, int y) {
        Point[] removed = {null};
        root = root.remove(x, y, 0, 0, 1024, removed);
        if (removed[0] != null) {
            numOfElements--;
            return removed[0];
        }
        return null;
    }
    
    /**
     * Removes a point given by the Point object
     * @param toRemove point to remove
     * @return Point object that was removed, returns null if no object found
     */
    public Point remove(Point toRemove) {
        Point[] removed = {null};
        root = root.remove(toRemove, 0, 0, 1024, removed);
        numOfElements--;
        return removed[0];
        
        
    }
    
    /**
     * @return a list of all duplicate points in the tree
     */
    public LinkedList<String> duplicates() {
        LinkedList<String> results = new LinkedList<String>();
        root.findDuplicates(results);
        return results;
    }

    /**
     * Searches for all points in a given region
     * @param x x coordinate of region 
     * @param y y coordinate of region 
     * @param width width of the region
     * @param height height of the region
     * @param numOfVisits number of nodes visited in the tree
     * @return list of all points found in given region
     */
    public LinkedList<Point> regionSearch(int x, int y, int width, int height,
            int[] numOfVisits) {
        LinkedList<Point> result = new LinkedList<Point>();
        result = root.regionSearch(x, y, width, height, 
                result, 0, 0, 1024, numOfVisits);
        
        return result;
    }

    /**
     * @return contents of the tree in string format
     */
    public LinkedList<String> dump() {
        int[] numOfVisits = {0};
        LinkedList<String> list = new LinkedList<String>();
        if (numOfElements == 0) {
            String temp = "Node at 0, 0, 1024: Empty";
            list.insert(temp);
            temp = "1 quadtree nodes printed";
            list.insert(temp);
        } 
        else {
            root.getContents(0, 0, 1024, list, 0, numOfVisits);
            list.insert(numOfVisits[0] + 
                    " quadtree nodes printed");
        }

        return list;
    }

    /**
     * Implementation of QuadNode, this implementation represents an empty node
     * @author Zach
     * @version 1.0
     */
    public class EmptyNode implements QuadNode {

        /**
         * Default constructor
         */
        public EmptyNode() {

        }

        /**
         * Creates a new leaf node and adds the element to it 
         * @param element point to add into the tree
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordiante of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @return Current quadnode
         */
        public QuadNode add(Point element, int currentX, int currentY, 
                int split) {

            LeafNode temp = new LeafNode();
            temp.add(element, currentX, currentY, split);
            return temp;
        }

        /**
         * Tries to search for point in current node, since this is an empty 
         * node no point is found
         * @param x x coordinate of point to search for
         * @param y y coordinate of point to search for
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordiante of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param result list of all found points
         * @return list of all found points
         */
        public LinkedList<Point> find(int x, int y, int currentX, 
                int currentY, int split, LinkedList<Point> result) {
            return result;
        }

        /**
         * Gets contents of node and return them in string format
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param list list of strings representing the contents of the tree
         * @param numOfIndents used in spacing in the dump output, determined
         *                     by how far down the tree that is being looked at
         * @param numOfVisits array used to hold number of visits
         * @return contents of the quadtree
         */
        public LinkedList<String> getContents(int currentX, int currentY,
                int split, LinkedList<String> list, int numOfIndents,
                int[] numOfVisits) {
            String temp = "";
            for (int i = 0; i < numOfIndents; i++) {
                temp = temp + "  ";
            }
            temp = temp + "Node at " + ((Integer) currentX).toString() +
                    ", " + ((Integer) currentY).toString() + ", "
                    + ((Integer) split).toString() + ": Empty";
            list.insert(temp);
            numOfVisits[0]++;
            return list;
        }

        /**
         * Since this is an empty node, no points were found
         * @param list list of all duplicate points in the tree
         * @return list of duplicate points
         */
        public LinkedList<String> findDuplicates(LinkedList<String> list) {

            return list;
        }

        /**
         * Tries to remove point in node.
         * This is an empty node so nothing removed
         * @param x x coordinate of point to be removed
         * @param y y coordinate of point to be removed
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param removed Point that was removed
         * @return current node
         */
        public QuadNode remove(int x, int y, int currentX, int currentY, 
                int split, Point[] removed) {
            return this;
        }
        
        /**
         * Tries to remove point in node.
         * This is an empty node so nothing removed
         * @param toRemove Point to be removed
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param removed Point that was removed
         * @return current node
         */
        public QuadNode remove(Point toRemove, int currentX, int currentY,
                int split, Point[] removed) {
            return this;
        }

        /**
         * Tries to find point in given region.
         * This is an empty node, so nothing found.
         * @param x x coordinate for region to be searched
         * @param y y coordinate for region to be searched
         * @param width of the region to be searched
         * @param height of the region to be searched
         * @param result list of points in given region
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param numOfVisits used in spacing in the dump output, determined
         *                     by how far down the tree that is being looked at
         * @return list of points in the region
         */
        public LinkedList<Point> regionSearch(int x, int y, int width, 
                int height, LinkedList<Point> result,
                int currentX, int currentY, int split, int[] numOfVisits) {    
            numOfVisits[0]++;
            return result;
        }

       

    }

    /**
     * Implementation of QuadNode, this implmentation represents 
     * @author Zach
     * @version 1.0
     */
    public class LeafNode implements QuadNode {

        /**
         * list of all points in the node
         */
        public LinkedList<Point> list; 

        /**
         * Default constructor, initializes linked list
         */
        public LeafNode() {
            list = new LinkedList<Point>();
        }
        
        /**
         * Constructor to create new leaf node 
         * @param newList list 
         */
        public LeafNode(LinkedList<Point> newList) {
            list = newList;
        }

        /**
         * Adds a new point to list in the node.
         * Max size of list is three, if another point would be added
         * this node would become an internal node and 4 child nodes would
         * be created. Exception to this is when all points in list have
         * the same xy coordinates and point to be inserted is also the 
         * same 
         * @param element point to add into the tree
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @return Current quadnode
         */
        public QuadNode add(Point element, int currentX,
                int currentY, int split) {
            if (list.getSize() == 0) {
                list.insert(element);
                return this;
            } 
            else if (list.getSize() < 3) {
                list.insert(element);
                return this;
            }
            else if (list.getSize() >= 3) {
                int same = 0;
                for (int i = 0; i < list.getSize(); i++) {
                    if ((list.get(i)).compareTo(element) == 0)
                        same++;
                   
                }
                
                if (same == list.getSize()) {
                    list.insert(element);
                    return this;
                }
            }
            
            QuadNode newInternal = new InternalNode();
            for (int i = 0; i < list.getSize(); i++) {
                newInternal = newInternal.add(list.get(i), 
                        currentX, currentY, split);
            }
            newInternal = newInternal.add(element, currentX, currentY, split);
            return newInternal;
            

        }
        
        
        /**
         * Gets contents of node and return them in string format
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param result list of strings representing the contents of the tree
         * @param numOfIndents used in spacing in the dump output, determined
         *                     by how far down the tree that is being looked at
         * @param numOfVisits array used to hold number of visits
         * @return contents of the quadtree
         */
        public LinkedList<String> getContents(int currentX, int currentY, 
                int split, LinkedList<String> result, int numOfIndents, 
                int[] numOfVisits) {
            String indents = "";
            String temp = "";
            for (int i = 0; i < numOfIndents; i++)
                indents = indents + "  ";
            temp = temp + indents;
            temp = temp + "Node at " + ((Integer) currentX).toString() + 
                    ", " + ((Integer) currentY).toString() + ", "
                    + ((Integer) split).toString() + ":";
            result.insert(temp);
            temp = "";
            for (int i = 0; i < this.list.getSize(); i++) {

                temp = indents + "(" + this.list.get(i).toString() + ")";
                result.insert(temp);
            }
            numOfVisits[0]++;
            return result;
        }

        /**
         * Since this is an empty node, no points were found
         * @param result list of all duplicate points in the tree
         * @return list of duplicate points
         */
        public LinkedList<String> findDuplicates(LinkedList<String> result) {

            for (int i = 0; i < this.list.getSize(); i++) {
                for (int j = i + 1; j < this.list.getSize(); j++) {
                    Point temp1 = this.list.get(i);
                    Point temp2 = this.list.get(j);
                    if (temp1.compareTo(temp2) == 0) {
                        
                        String temp = ((Integer)temp1.getX()).toString() + 
                                ", " + ((Integer)temp1.getY()).toString();
                        
                        if (result.exists(temp) == false)
                            result.insert(temp);
                    }
                }

            }

            return result;
        }

        /**
         * Tries to remove point in node.
         * Goes through list to find point
         * @param x x coordinate of point to be removed
         * @param y y coordinate of point to be removed
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param removed Point that was removed
         * @return current node
         */
        public QuadNode remove(int x, int y, int currentX, int currentY, 
                int split, Point[] removed) {
            int i = 0;
            for (i = 0; i < list.getSize(); i++) {
                if (x == list.get(i).getX() && y == list.get(i).getY()) {
                    removed[0] = list.get(i);
                    list.delete(i);
                    break;    
                }
            }
            if (list.getSize() == 0) {
                
                return flynode;
            }
            else 
                return this;
        }
        
        /**
         * Tries to remove point in node.
         * Goes through list to find point
         * @param toRemove Point to be removed
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param removed Point that was removed
         * @return current node
         */
        public QuadNode remove(Point toRemove, int currentX, 
                int currentY, int split, Point[] removed) {
            int index ;
            for (index = 0; index < list.getSize(); index++) {
                if (toRemove == list.get(index)) {
                    removed[0] = list.get(index);
                    list.delete(index);
                    break;    
                }
            }
            if (list.getSize() == 0) {
                
                return flynode;
            }
            else 
                return this;
        }

        /**
         * Tries to find point in given region.
         * Goes through list to find points in given region
         * @param x x coordinate for region to be searched
         * @param y y coordinate for region to be searched
         * @param width of the region to be searched
         * @param height of the region to be searched
         * @param result list of points in given region
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param numOfVisits used in spacing in the dump output, determined
         *                     by how far down the tree that is being looked at
         * @return list of points in the region
         */
        public LinkedList<Point> regionSearch(int x, int y, int height, 
                int width, LinkedList<Point> result, int currentX, 
                int currentY, int split, int[] numOfVisits) {
            
            numOfVisits[0]++;
            
            for (int i = 0; i < list.getSize(); i++) {
                int tempX = (list.get(i)).getX();
                int tempY = (list.get(i)).getY();

                if (tempX >= x && tempX < x + width && tempY >= y 
                        && tempY < y + height) {
                    result.insert(list.get(i));
                }
            }

            return result;
            
        }
    }

    
    /**
     * Implementation of a quadnode, this implementation is for
     * internal nodes in the tree
     * @author Zach
     * @version 1.0
     */
    public class InternalNode implements QuadNode {
        private QuadNode nW;
        private QuadNode nE;
        private QuadNode sW;
        private QuadNode sE;

        /**
         * Default constructor, sets each section to the flynode
         */
        public InternalNode() {
            nW = (QuadNode) flynode;
            nE = (QuadNode) flynode;
            sW = (QuadNode) flynode;
            sE = (QuadNode) flynode;
        }

        /**
         * Finds a area to place the point in 
         * @param element point to add into the tree
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordiante of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @return Current quadnode
         */
        public QuadNode add(Point element, int currentX, 
                int currentY, int split) {

            int bound = split / 2;
            int xBound = currentX + bound;
            int yBound = currentY + bound;
            if (element.getX() < xBound && element.getY() < yBound) {

                nW = nW.add(element, currentX, currentY, bound);
                return this;
            } 
            else if ( element.getX() >= xBound && element.getY() < yBound) {

                nE = nE.add(element, xBound, currentY, bound);
                return this;
            } 
            else if (element.getX() < xBound && element.getY() >= yBound) {

                sW = sW.add(element, currentX, yBound, bound);
                return this;
            } 
            else {

                sE = sE.add(element, xBound, yBound, bound);
                return this;
            }

        }



        /**
         * Gets contents of node and return them in string format
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param bound length of region being looked at
         * @param list list of strings representing the contents of the tree
         * @param numOfIndents used in spacing in the dump output, determined
         *                     by how far down the tree that is being looked at
         * @param numOfVisits array used to hold number of visits
         * @return contents of the quadtree
         */
        public LinkedList<String> getContents(int currentX, int currentY,
                int bound, LinkedList<String> list, int numOfIndents, 
                int[] numOfVisits) {
            int split = bound / 2;
            String temp = "";
            for (int i = 0; i < numOfIndents; i++)
                temp = temp + "  ";
            temp = temp + "Node at " + ((Integer) currentX).toString() + 
                    ", " + ((Integer) currentY).toString() + ", "
                    + ((Integer) bound).toString() + ": Internal";
            list.insert(temp);
            list = nW.getContents(currentX , currentY , split, list,
                    numOfIndents + 1, numOfVisits);
            list = nE.getContents(currentX + split, currentY , split, list,
                    numOfIndents + 1, numOfVisits);
            list = sW.getContents(currentX , currentY + split, split, list, 
                    numOfIndents + 1, numOfVisits); 
            list = sE.getContents(currentX + split, currentY + split, split,
                    list, numOfIndents + 1, numOfVisits);
            numOfVisits[0]++;
            return list;
        }

        /**
         * Goes to each child node to find duplicates
         * @param list list of duplicate points in the tree
         * @return list of duplicate points
         */
        public LinkedList<String> findDuplicates(LinkedList<String> list) {

            if (nW != flynode)
                list = nW.findDuplicates(list);
            
            if (nE != flynode)
                list = nE.findDuplicates(list);
            
            if (sW != flynode)
                list = sW.findDuplicates(list);
            
            if (sE != flynode)
                list = sE.findDuplicates(list);

            return list;
        }

        /**
         * Tries to remove point in node.
         * Goes to appropriate region where point might be found.
         * @param x x coordinate of point to be removed
         * @param y y coordinate of point to be removed
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param removed Point that was removed
         * @return current node
         */
        public QuadNode remove(int x, int y, int currentX, int currentY,
                int split, Point[] removed) {
            int half = split / 2;
            

            if (x < currentX + half && y < currentY + half)
                nW = nW.remove(x, y, currentX, currentY, half, removed);

            else if (x >= currentX + half && y < currentY + half)
                nE = nE.remove(x, y, currentX + half, currentY, half, removed);

            else if (x < currentX + half && y >= currentY + half)
                sW = sW.remove(x, y, currentX, currentY + half, half, removed);

            else
                sE = sE.remove(x, y, currentX + half, currentY + half, half, 
                        removed);

            return trim();
        }
        
        /**
         * Tries to remove point in node.
         * Goes to appropriate child node where point might be found
         * @param toRemove Point to be removed
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param removed Point that was removed
         * @return current node
         */
        public QuadNode remove(Point toRemove, int currentX, int currentY, 
                int split, Point[] removed) {
            int half = split / 2;
            int x = toRemove.getX();
            int y = toRemove.getY();

            if (x < currentX + half && y < currentY + half)
                nW = nW.remove(toRemove, currentX, currentY, 
                        half, removed);

            else if (x >= currentX + half && y < currentY + half)
                nE = nE.remove(toRemove, currentX + half, currentY, 
                        half, removed);

            else if (x < currentX + half && y >= currentY + half)
                sW = sW.remove(toRemove, currentX, currentY + half, 
                        half, removed);

            else
                sE = sE.remove(toRemove, currentX + half, currentY + half, 
                        half, removed);

            return trim();
        }

        /**
         * After removal if any child nodes can be merged, it merges them 
         * @return result of any merges if any, if no then returns current node
         */
        private QuadNode trim() {
            
            /*
             * Checks if there is only one node with things in it  
             */
            if (nW.getClass().getName().compareTo("QuadTree$LeafNode") == 0 
                    && nE == flynode && sW == flynode && sE == flynode) {
                return nW;
            }
            else if (nW == flynode && 
                    nE.getClass().getName().compareTo(
                            "QuadTree$LeafNode") == 0
                    && sW == flynode && sE == flynode) {
                return nE;  
            }
            else if (nW == flynode && nE == flynode && 
                    sW.getClass().getName().compareTo(
                            "QuadTree$LeafNode") == 0
                    && sE == flynode) {
                return sW;
            }
            else if (nW == flynode && nE == flynode && sW == flynode && 
                    sE.getClass().getName().compareTo(
                            "QuadTree$LeafNode") == 0) {
                return sE;
            }
            
            else {

                LinkedList<Point> results = new LinkedList<Point>(); 
        
                if (nW.getClass().getName().compareTo(
                        "QuadTree$LeafNode") == 0) {
                    for (int i = 0; i < ((LeafNode)nW).list.getSize(); i++) {
                        results.insert(((LeafNode)nW).list.get(i));
                    }
    
                }
                if (nE.getClass().getName().compareTo(
                        "QuadTree$LeafNode") == 0) {
                    for (int i = 0; i < ((LeafNode)nE).list.getSize(); i++) {
                        results.insert(((LeafNode)nE).list.get(i));
                    }
    
                }
                if (sW.getClass().getName().compareTo(
                        "QuadTree$LeafNode") == 0) {
                    for (int i = 0; i < ((LeafNode)sW).list.getSize(); i++) {
                        results.insert(((LeafNode)sW).list.get(i));
                    }
    
                }
                if (sE.getClass().getName().compareTo(
                        "QuadTree$LeafNode") == 0) {
                    for (int i = 0; i < ((LeafNode)sE).list.getSize(); i++) {
                        results.insert(((LeafNode)sE).list.get(i));
                    }
    
                }
                
                if (results.getSize() == 3) {
                    return new LeafNode(results);
                }
                return this;
            }
            
        }

        /**
         * Tries to find point in given region.
         * This is an empty node, so nothing found.
         * @param x x coordinate for region to be searched
         * @param y y coordinate for region to be searched
         * @param width of the region to be searched
         * @param height of the region to be searched
         * @param result list of points in given region
         * @param currentX x coordinate of the section of the tree 
         *                 that is being looked at
         * @param currentY y coordinate of the section of the tree
         *                 that is being looked at
         * @param split length of region being looked at
         * @param numOfVisits used in spacing in the dump output, determined
         *                     by how far down the tree that is being looked at
         * @return list of points in the region
         */
        public LinkedList<Point> regionSearch(int x, int y, int width, 
                int height, LinkedList<Point> result,
                int currentX, int currentY, int split, int[] numOfVisits) {
            numOfVisits[0]++;

            int half = split / 2;
            int xBound = currentX + half;
            int yBound = currentY + half;
            int yMax = y + height - 1;
            int xMax = x + width - 1;
            LinkedList<Point> found = result;

            
            if (xBound > x && yBound > y) {

                found = nW.regionSearch(x, y, width, height, found, 
                        currentX, currentY, half, numOfVisits);
                
            }

            if (xBound <= xMax && yBound > y) {

                found = nE.regionSearch(x, y, width, height, found, 
                        xBound, currentY, half, numOfVisits);
                
            }

            if (xBound > x && yBound <= yMax) {

                found = sW.regionSearch(x, y, width, height, found, 
                        currentX, yBound, half, numOfVisits);
                
            }

            if (xMax >= xBound && yMax >= yBound) {

                found = sE.regionSearch(x, y, width, height, found, 
                        xBound, yBound, half, numOfVisits);
                
            }
            
           

            return found;
        }

    }

}
