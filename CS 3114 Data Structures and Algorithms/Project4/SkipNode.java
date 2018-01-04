
/**
 * Skip node implementation
 * @author Zach-Laptop
 * @version 1.0
 */
public class SkipNode implements java.io.Serializable {
   

    /**
     * Array of integers that represent the handles for various skip nodes
     */
    public int [] forward;
    
    /**
     * Handle for the kv pair associated with this skip node
     */
    public int kvpair;

    /**
     * Constructor
     * @param kvpair handle for kv pair associated with this skip node
     * @param level number of levels in new skip node
     */
    public SkipNode(int kvpair, int level) {
        this.kvpair = kvpair;
        forward = new int [level + 1];
        for (int i = 0; i < level + 1; i++) {
            forward[i] = -1;
        }
    }
    

}