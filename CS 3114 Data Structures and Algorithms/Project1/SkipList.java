import java.lang.reflect.Array;
import java.util.Random;

/**
 * Class used to create and manage a skip list.
 * @author Zach
 * @version 1.0
 * @param <K>
 *            The key value
 * @param <V>
 *            Value to be stored with key
 */
public class SkipList<K extends Comparable<K>, V> {

    private SkipNode<K, V> head;
    private int level; // Length of deepest node in skip list
    private int size; // Holds the number of elements in skip list
    private Random rnd; // Random number generator

    /**
     * Default constructor for skip list
     */
    public SkipList() {
        head = new SkipNode<K, V>(null, null, 0); // empty skip node
        rnd = new Random(); // initializes random number generator
        level = 0; // initial depth is zero
        size = 0; // initial size is zero 
    }

    /**
     * Adjusts head node of skip list
     * @param newLevel new depth of head node
     */
    private void adjustHead(int newLevel) {
        SkipNode<K, V> temp = head;
        head = new SkipNode<K, V>(null, null, newLevel);
        for (int i = 0; i <= level; i++) // Copies the array
            head.forward[i] = temp.forward[i];
        level = newLevel;
    }

    /**
     * Pick a level using a geometric distribution
     * 
     * @return random level from 0 to 2
     */
    public int randomLevel() {
        int lev;
        for (lev = 0; rnd.nextInt(2) == 0; lev++) {
        } // Do nothing
        return lev;
    }

    /**
     * Insert value and key into SkipList
     * 
     * @param key
     *            key that will be inserted into list
     * @param element
     *            the element that will inserted into the list
     */
    public void insert(K key, V element) {

        int newLevel = randomLevel(); // New node's level
        if (newLevel > level) // If new node is deeper
            adjustHead(newLevel); // adjust the header
        
        //Creates new array 
        @SuppressWarnings("unchecked")
        SkipNode<K, V>[] update = (SkipNode<K, V>[]) 
                Array.newInstance(SkipNode.class, level + 1); 

        SkipNode<K, V> x = head; // Start at header node
        for (int i = level; i >= 0; i--) { // Find insert position
            while ((x.forward[i] != null) && 
                    (key.compareTo(x.forward[i].key()) > 0))
                x = x.forward[i];
            update[i] = x; // Track end at level i
        }
        x = new SkipNode<K, V>(key, element, newLevel);
        for (int i = 0; i <= newLevel; i++) { // Splice into list
            x.forward[i] = update[i].forward[i]; // Who x points to
            update[i].forward[i] = x; // Who y points to
        }
        size++; // Increment dictionary size
    }

    /**
     * Removes skip node from list
     * @param key determines the node to be removed
     * @return skip node that was removed, null if no node can be found
     */
    public SkipNode<K, V> remove(K key) {
        SkipNode<K, V> x = head;
        SkipNode<K, V> y = searchNode(key);
        if (y == null) // if no node can be found
            return y;
        SkipNode<K, V>[] replace = y.forward;
        int currentLevel = x.forward.length - 1; // Gets level 
        while (x != null) { // For traversing the skip list

            for (int i = currentLevel; i >= 0; i--) { // traverses list
                if (x.forward[i] != null) {
                    /*
                     * Every time there is a reference to the node as given
                     * by the key, replaces the pointers such that node is
                     * removed 
                     */
                    if ((x.forward[i] == y))
                        x.forward[i] = replace[i];
                }
            }

            x = x.forward[0]; // Move node forward by one
            if (x != null) // Gets new depth if x does not equal null
                currentLevel = x.forward.length - 1;
        }
        
        size--;
        return y; // If removal is successful, return skip node
    }

    /**
     * Removes skip node in list, determined by given object
     * @param remove the object that needs to be removed from skip list
     * @return skip node that needs to be removed, null if none found
     */
    public SkipNode<K, V> remove(Object remove) {
        SkipNode<K, V> check = head; // Start at start of list
        SkipNode<K, V> toRemove = searchNode(remove);
        if (toRemove == null)
            return toRemove;
        SkipNode<K, V>[] replace = toRemove.forward;
        int currentLevel = check.forward.length - 1;
        while (check != null) {

            for (int i = currentLevel; i >= 0; i--) { // traverses the list
                if (check.forward[i] != null) {
                    /*
                     * Every time there is a reference to the node as given
                     * by the key, replaces the pointers such that node is
                     * removed 
                     */
                    if ((check.forward[i] == toRemove))
                        check.forward[i] = replace[i];
                }

            }

            check = check.forward[0]; // Moves node forward by one
            if (check != null) // Gets new depth if x does not equal null
                currentLevel = check.forward.length - 1;
        }

        size--; //if successful remove, then size is reduced
        return toRemove;
    }

    /**
     * Finds first skip node that matched the given object
     * @param searchKey
     * @return skip node for the given key, null if none exist
     */
    private SkipNode<K, V> searchNode(K searchKey) {
        SkipNode<K, V> x = head; // Dummy header node
        for (int i = level; i >= 0; i--) // For each level...
            while ((x.forward[i] != null) && // go forward
                    (searchKey.compareTo(x.forward[i].key()) > 0))
                x = x.forward[i]; // Go one last step
        x = x.forward[0]; // Move to actual record, if it exists
        if ((x != null) && (searchKey.compareTo(x.key()) == 0))
            return x; // Node has been found
        else
            return null; // Not found
    }

    /**
     * Finds first skip node that matched the given object
     * @param find element used to find skip node
     * @return skip node for the object, null if no skip node is found
     */
    private SkipNode<K, V> searchNode(Object find) {
        SkipNode<K, V> node = head; // Start at start of list
        int currentLevel = level - 1;
        
        /*
         * This is for traversing the skip list when the largest depth size
         * is not one
         */
        if (node.forward.length != 1) {
            while (node != null) { // Traverses skip list

                // Checks each pointer in skip node
                for (int i = currentLevel; i >= 0; i--) { 
                    if (node.forward[i] != null) {
                        // if it found a matching node
                        if (find.equals(node.forward[i].element())) {
                            node = node.forward[i];
                            return node;
                        }
                    }
                }

                node = node.forward[0]; //moves one node
                /*
                 * If there are more nodes in the skip list, 
                 */
                if (node != null) { 
                    currentLevel = node.forward.length - 1;
                }
            }
        } 
        /*  
         * For traversing the skip list when each node of the skip list
         * only has a depth of one
         */
        else {
            while (node != null) {
                if (node.forward[0] == null)
                    return null; // if not found
                if (find.equals(node.forward[0].element())) { // if found
                    node = node.forward[0]; // moves one node
                    return node; //returns result
                }
                node = node.forward[0]; //moves one node forward

            }
        }
        return null; // if not found
    }

    /**
     * Returns list of all skip nodes
     * @return list of all the skip nodes in skip list
     */
    public SkipNode<K, V>[] dump() {
        SkipNode<K, V> temp = head;
        @SuppressWarnings("unchecked")
        SkipNode<K, V>[] list = new SkipNode[1]; 
        int length = 0;
        while (temp != null) { // traverses skip list
            if (length == 0) { // to set up the output array
                list[0] = new SkipNode<K, V>(temp.key(), temp.element(),
                        temp.forward.length);
                length++;
            } 
            else { // Creates bigger array and adds node to list
                @SuppressWarnings("unchecked")
                SkipNode<K, V>[] tempArray = new SkipNode[list.length + 1]; 
                System.arraycopy(list, 0, tempArray, 0, list.length);
                tempArray[tempArray.length - 1] = new SkipNode<K, V>(
                        temp.key(), temp.element(), temp.forward.length);
                list = tempArray;
            }
            temp = temp.forward[0]; 
        }
        return list; // Returns result 

    }

    /**
     * Finds all KVPairs given a search key
     * @param searchKey key used to find results
     * @return list of all KVPairs found using searchKey
     */
    public SkipNode<K, V>[] find(K searchKey) {
        @SuppressWarnings("unchecked")
        SkipNode<K, V>[] result = new SkipNode[1];
        SkipNode<K, V> x = head; // Dummy header node
        int startLevel = level;
        while (x != null) {
            for (int i = startLevel; i >= 0; i--) { // For each level...
                
                /*
                 * Code was placed in try catch blocks because when traversing
                 * the skip list errors may occur
                 */
                try {
                    while ((x.forward[i] != null) && // go forward
                            (searchKey.compareTo(x.forward[i].key()) > 0))
                        x = x.forward[i]; // Go one last step
                } 
                catch (Exception e) {
                    return result;
                }
            }
            x = x.forward[0]; // Move to actual record, if it exists
            
            /*
             * If the skip node contains the search, it places it in the
             * result array
             */
            if ((x != null) && (searchKey.compareTo(x.key()) == 0)) {
                @SuppressWarnings("unchecked")
                SkipNode<K, V>[] tempArray = new SkipNode[result.length + 1];
                System.arraycopy(result, 0, tempArray, 0, result.length);
                tempArray[tempArray.length - 1] = 
                        new SkipNode<K, V>(searchKey, x.element(), 0);
                result = tempArray;
                startLevel = x.forward.length - 1;
            } 
           
        }
        return result;

    }

}
