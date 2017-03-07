/**
 * Object to represent free blocks 
 * @author Zachary Yee
 * @version 1.0
 */
public class Freespace {
    
    private int handle;
    private int size;
    
    /**
     * Constructor
     * @param handle starting position of free space
     * @param size number of free space bytes
     */
    public Freespace(int handle, int size) {
        this.handle = handle;
        this.size = size;
    }
    
    /**
     * @return handle position
     */
    public int getHandle() {
        return handle;
    }
    
    /**
     * 
     * @return number of free bytes
     */
    public int size() {
        return size;
    }
    
    /**
     * Sets new handle position for free space
     * @param handle new handle position to set
     */
    public void setHandle(int handle) {
        this.handle = handle;
    }
    
    /**
     * Sets new size for free space
     * @param size new size to set
     */
    public void setSize(int size) {
        this.size = size;
    }
    
    

}
