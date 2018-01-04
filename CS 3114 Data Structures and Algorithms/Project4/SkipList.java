import java.io.IOException;
import java.util.Random;

/**
 * Skip list implementation
 * @author Zachary Yee
 * @version 1.0
 * @param <K>
 * @param <V>
 */
public class SkipList<K extends Comparable<K>, V> {

    private int head; // handle position of start skip node
    private int level; // Length of deepest node in skip list
    private int size; // Holds the number of elements in skip list
    private Random rnd; // Random number generator
    private MemoryManager memManager;

    /**
     * Constructor
     * @param memManager Memory manager that skip list will use
     * @throws IOException
     */
    public SkipList(MemoryManager memManager) throws IOException {
        
        this.memManager = memManager;
        SkipNode temp = new SkipNode(-1, 0);
        byte[] headBytes = Serializer.serialize(temp);
        int headHandle = memManager.insert(headBytes, headBytes.length);
        head = headHandle; // empty skip node
        rnd = new Random(); // initializes random number generator
        level = 0; // initial depth is zero
        size = 0; // initial size is zero 
        
    }
    
    /**
     * @return size of skip list
     */
    public int size() {
        return size;
    }

    /**
     * Adjusts head of skip list
     * @param newLevel length of start node
     * @throws IOException 
     * @throws ClassNotFoundException 
     */
    private void adjustHead(int newLevel) throws ClassNotFoundException,
        IOException {
        
        //Creates new head node
        SkipNode temp = (SkipNode) 
                Serializer.deserialize(memManager.get(head));
        memManager.remove(head);
        SkipNode newHead = new SkipNode(-1, newLevel);
        for (int i = 0; i <= level; i++) {
            newHead.forward[i] = temp.forward[i];
        }
        
        //Places new head node into the file
        byte[] headBytes = Serializer.serialize(newHead);
        int prevHead = head;
        head = memManager.insert(headBytes, headBytes.length);
        temp = (SkipNode) Serializer.deserialize(memManager.get(head));
        
        
        //Finds every reference to the old head and replaces it with the new one
        int x = temp.forward[0];
        while (x != -1) {
            boolean check = false;
            temp = (SkipNode) Serializer.deserialize(memManager.get(x));
            for (int i = 0; i < temp.forward.length; i++) {
                if (temp.forward[i] == prevHead) {
                    temp.forward[i] = head;
                    check = true;
                }
            }
            if (check) {
                byte[] newBytes = Serializer.serialize(temp);
                memManager.remove(x);
                memManager.insert(newBytes, newBytes.length, x);
            }
            x = temp.forward[0];
        }
        level = newLevel;
    }

    /**
     * @return a random number 
     */
    public int randomLevel() {
        int lev;
        for (lev = 0; rnd.nextInt(2) == 0; lev++) {
        } // Do nothing
        return lev;
    }

    /**
     * Inserts a skip node in list
     * @param key key of skip node
     * @param element contents of node
     * @throws IOException 
     * @throws ClassNotFoundException 
     */
    public void insert(K key, V element) throws IOException, 
        ClassNotFoundException {

        int newLevel = randomLevel(); // New node's level
        if (newLevel > level) { // If new node is deeper
            adjustHead(newLevel); // adjust the header
        }
        
        int[] update = findInsertPosition(key);
        
        //Replaces references so that new node is in skip list
        KVpair<K, V> kvInsert = new KVpair<K, V>(key, element);
        byte[] kvBytes = Serializer.serialize(kvInsert);
        int kvHandle = memManager.insert(kvBytes, kvBytes.length);
        SkipNode insertNode = new SkipNode(kvHandle, newLevel);
        byte[] insertBytes = Serializer.serialize(insertNode);
        int nodeHandle = memManager.insert(insertBytes, insertBytes.length);
        for (int i = 0; i <= newLevel; i++) {
            
            if (update[i] == -1) {
                insertNode.forward[i] = -1;
            }
            else {
                SkipNode temp = (SkipNode) Serializer.deserialize(memManager
                        .get(update[i]));
                insertNode.forward[i] = temp.forward[i];
                temp.forward[i] = nodeHandle;
                memManager.remove(update[i]);
                byte[] updateTemp = Serializer.serialize(temp);
                memManager.insert(updateTemp, updateTemp.length, update[i]);
            }
        }
        
        //Inserts complete new node into the file
        memManager.remove(nodeHandle);
        insertBytes = Serializer.serialize(insertNode);
        memManager.insert(insertBytes, insertBytes.length, nodeHandle);
       
        size++; // Increment dictionary size
    }
    
    /**
     * Finds insert position
     * @param key key used to find insert position
     * @return array of skip node handles that will need to be updated
     * @throws ClassNotFoundException
     * @throws IOException
     */
    private int[] findInsertPosition(K key) throws ClassNotFoundException,
            IOException {
        // Creates new array to contain new references 
        int[] update = new int[level + 1];

        for (int i = 0; i < update.length; i++) {
            update[i] = -1;
        }

        //Populates update array with proper references
        int findInsertPos = head;
        SkipNode temp = (SkipNode) Serializer.deserialize(memManager
                .get(findInsertPos));
        for (int i = level; i >= 0; i--) {
            boolean cont = true;
            while (temp.forward[i] != -1 && cont) {
                SkipNode temp2 = (SkipNode) Serializer.deserialize(memManager
                        .get(temp.forward[i]));
                @SuppressWarnings("unchecked")
                KVpair<K, V> kv = (KVpair<K, V>) Serializer
                        .deserialize(memManager.get(temp2.kvpair));
                if (key.compareTo(kv.key()) > 0) {
                    findInsertPos = temp.forward[i];
                    temp = temp2;
                } 
                else {
                    cont = false;
                }
            }
            update[i] = findInsertPos;
        }
        return update;

    }
    
    /**
     * Gets a single element from skip list, this is for getting elements
     * when you know the contents of the list
     * @param index position in skip list
     * @return element associated with the list
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public V get(int index) throws ClassNotFoundException, IOException {
        SkipNode temp = (SkipNode) 
                Serializer.deserialize(memManager.get(head));
        for (int i = 0; i <= index; i++) {
            temp = (SkipNode) Serializer.deserialize(memManager
                    .get(temp.forward[0]));
        }
        @SuppressWarnings("unchecked")
        KVpair<K, V> kv = (KVpair<K, V>) Serializer.deserialize(memManager
                .get(temp.kvpair));
        return kv.value();
    }
    
    /**
     * Removes a skip node given a key
     * @param key key to remove node by
     * @return handle position of removed node
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public int remove(K key) throws ClassNotFoundException, IOException {
        int toFind = head;
        int toRemove = searchNode(key);
        
        if (toRemove == -1) {
            return -1;
        }
        
        int[] replace = ((SkipNode) Serializer.deserialize(memManager
                .get(toRemove))).forward;
        SkipNode temp = null;
        int currentLevel = ((SkipNode) Serializer.deserialize(memManager
                .get(toFind))).forward.length - 1;
        while (toFind != -1) {
            for (int i = currentLevel; i >= 0; i--) {
                temp = (SkipNode) Serializer
                        .deserialize(memManager.get(toFind));
                if (temp.forward[i] != -1 && temp.forward[i] == toRemove) {
                    temp.forward[i] = replace[i];
                    memManager.remove(toFind);
                    byte[] update = Serializer.serialize(temp);
                    memManager.insert(update, update.length, toFind);
                }
            }
            toFind = temp.forward[0];
            if (toFind != -1) {
                temp = (SkipNode) Serializer
                        .deserialize(memManager.get(toFind));
                currentLevel = temp.forward.length - 1;
            }
        }
        
        size--;
        temp = (SkipNode) Serializer.deserialize(memManager.get(toRemove));
        memManager.remove(toRemove);
        return temp.kvpair;
    }
    
    /**
     * Removes node by a given object
     * @param obj object to remove node by
     * @return handle of removed node
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public int remove(Object obj) throws ClassNotFoundException, IOException {
        int toFind = head;
        int toRemove = searchNode(obj);
        
        if (toRemove == -1) {
            return -1;
        }
        
        int[] replace = ((SkipNode) Serializer.deserialize(memManager
                .get(toRemove))).forward;
        SkipNode temp = null;
        int currentLevel = ((SkipNode) Serializer.deserialize(memManager
                .get(toFind))).forward.length - 1;
        while (toFind != -1) {
            for (int i = currentLevel; i >= 0; i--) {
                temp = (SkipNode) Serializer
                        .deserialize(memManager.get(toFind));
                if (temp.forward[i] != -1 && temp.forward[i] == toRemove) {
                    temp.forward[i] = replace[i];
                    memManager.remove(toFind);
                    byte[] update = Serializer.serialize(temp);
                    memManager.insert(update, update.length, toFind);
                }
            }
            toFind = temp.forward[0];
            if (toFind != -1) {
                temp = (SkipNode) Serializer
                        .deserialize(memManager.get(toFind));
                currentLevel = temp.forward.length - 1;
            }
        }
        
        size--;
        temp = (SkipNode) Serializer.deserialize(memManager.get(toRemove));
        memManager.remove(toRemove);
        return temp.kvpair;
    }
    
    /**
     * Finds a node given a key
     * @param key key that will be used to find node
     * @return handle position of found node, -1 if not found
     * @throws ClassNotFoundException
     * @throws IOException
     */
    private int searchNode(K key) throws ClassNotFoundException, IOException {
        int nodeToFind = head;
        SkipNode temp = (SkipNode) Serializer.deserialize(memManager
                .get(nodeToFind));
        for (int i = level; i >= 0; i--) {
            boolean cont = true;
            while (temp.forward[i] != -1 && cont) {
                SkipNode temp2 = (SkipNode) Serializer.deserialize(memManager
                        .get(temp.forward[i]));
                @SuppressWarnings("unchecked")
                KVpair<K, V> kv = (KVpair<K, V>) Serializer
                        .deserialize(memManager.get(temp2.kvpair));
                if (key.compareTo(kv.key()) > 0) {
                    nodeToFind = temp.forward[i];
                    temp = temp2;
                }
                else {
                    cont = false;
                }
                    
            }
        }
        try {
            nodeToFind = temp.forward[0];
            temp = (SkipNode) Serializer.deserialize(memManager
                    .get(temp.forward[0]));
            @SuppressWarnings("unchecked")
            KVpair<K, V> kv = (KVpair<K, V>) Serializer.deserialize(memManager
                    .get(temp.kvpair));
            if (key.compareTo(kv.key()) == 0) {
                return nodeToFind;
            }
        }
        catch (Exception e) {
            return -1;
        }
        
        return -1;
    }
    
    /**
     * Searches for node given object 
     * @param obj object to search for 
     * @return skip node handle
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public int searchNode(Object obj) throws ClassNotFoundException, 
        IOException {
        int nodeToFind = head;
        int currentLevel = level - 1;
        SkipNode temp = (SkipNode) Serializer.deserialize(memManager
                .get(nodeToFind));
        if (temp.forward.length != 1) {
            while (nodeToFind != -1) {
                
                for (int i = currentLevel; i >= 0; i--) {
                    temp = (SkipNode) Serializer.deserialize(memManager
                            .get(nodeToFind));
                    if (temp.forward[i] != -1) {
                        int check = temp.forward[i];
                        SkipNode temp2 = (SkipNode) Serializer
                                .deserialize(memManager.get(check));
                        @SuppressWarnings("unchecked")
                        KVpair<K, V> kv = (KVpair<K, V>) Serializer
                                .deserialize(memManager.get(temp2.kvpair));
                        if (obj.equals(kv.value())) {
                            nodeToFind = check;
                            return nodeToFind;
                        }
                    }
                }
                nodeToFind = temp.forward[0];
                
                if (nodeToFind != -1) {
                    SkipNode temp2 = (SkipNode) Serializer
                            .deserialize(memManager.get(nodeToFind));
                    currentLevel = temp2.forward.length - 1;
                }
            }
        }
        else { //Searches when forward array length are all size one
            while (nodeToFind != -1) {
                temp = (SkipNode) Serializer.deserialize(memManager
                        .get(nodeToFind));
                if (temp.forward[0] == -1) {
                    return -1;
                }
                int check = temp.forward[0];
                temp = (SkipNode) Serializer.deserialize(memManager
                        .get(temp.forward[0]));
                @SuppressWarnings("unchecked")
                KVpair<K, V> kv = (KVpair<K, V>) Serializer
                        .deserialize(memManager.get(temp.kvpair));
                if (obj.equals(kv.value())) {
                    nodeToFind = check;
                    return nodeToFind;
                }
                nodeToFind = check;
            }
            
        }
        
        return -1;
    }
    
    
    /**
     * Prints out contents of skip list
     * @throws IOException
     * @throws ClassNotFoundException
     */
    public void dump() throws IOException, ClassNotFoundException {
        System.out.println("SkipList dump:");
        int x = head;

        while (x != -1) {
            SkipNode temp = (SkipNode) Serializer
                    .deserialize(memManager.get(x));
            System.out.print("Node has depth: " + temp.forward.length
                    + ", Value ");
            if (temp.kvpair == -1) {
                System.out.print("(null)\n");
            } 
            else {
                @SuppressWarnings("unchecked")
                KVpair<K, V> tempKV = (KVpair<K, V>) Serializer
                        .deserialize(memManager.get(temp.kvpair));
                System.out.print("(" + tempKV.value().toString() + ")\n");
            }
            x = temp.forward[0];
        }
        System.out.println("SkipList size is: " + size);

    }
    
    /**
    * Finds a nodes in the skip list
    * @param key key to search by
    * @return list of all found nodes
     * @throws IOException 
     * @throws ClassNotFoundException 
    */
    public String[] find(K key) throws ClassNotFoundException, IOException {

        String[] result = new String[1];
        int searchHandle = head;
        int startLevel = level;
        while (searchHandle != -1) {
            SkipNode temp = (SkipNode) Serializer.deserialize(memManager
                    .get(searchHandle));
            for (int i = startLevel; i >= 0; i--) {
                boolean cont = true;
                try {
                    while (temp.forward[i] != -1 && cont) {
                        SkipNode temp2 = (SkipNode) Serializer
                                .deserialize(memManager.get(temp.forward[i]));
                        @SuppressWarnings("unchecked")
                        KVpair<K, V> kv = (KVpair<K, V>) Serializer
                                .deserialize(memManager.get(temp2.kvpair));
                        if (key.compareTo(kv.key()) > 0) {
                            searchHandle = temp.forward[i];
                            temp = temp2;
                        } 
                        else {
                            cont = false;
                        }
                    }
                }
                catch (Exception e) {
                    return result;
                }
            }
            searchHandle = temp.forward[0];
            
            if (searchHandle != -1) {
                temp = (SkipNode) Serializer.deserialize(memManager
                        .get(searchHandle));
                @SuppressWarnings("unchecked")
                KVpair<K, V> kv = (KVpair<K, V>) Serializer
                        .deserialize(memManager.get(temp.kvpair));
                if (key.compareTo(kv.key()) == 0) {
                    String [] tempRes = new String[result.length + 1];
                    System.arraycopy(result, 0, tempRes, 0, result.length);
                    tempRes[tempRes.length - 1] = kv.value().toString();
                    result = tempRes;
                    startLevel = temp.forward.length - 1;
                }
            }

        }
        return result;

    }
}
