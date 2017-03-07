import java.io.File;
import java.io.IOException;

/**
 * Database to handle storing of rectangles in a skip list
 * @author Zach-Laptop
 * @version 1.0
 */
public class Database {

    private SkipList<String, Rectangle> list;
    private MemoryManager memManager;

    /**
     * Constructor
     * @param diskFile file that the buffer manager will manipulate
     * @param numBuffs number of buffers in buffer pool
     * @param buffSize size of buffers in buffer pool
     * @throws IOException
     */
    public Database(String diskFile, int numBuffs, int buffSize)
        throws IOException {
        File file = new File(diskFile);
        BufferManager buffManager = new BufferManager(file, numBuffs, buffSize);
        memManager = new MemoryManager(buffSize, buffManager);
        list = new SkipList<String, Rectangle>(memManager);
    }

    /**
     * Inserts a rectangle in skip list
     * @param name name of rectangle
     * @param rectangle retangle object to insert
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void insert(String name, Rectangle rectangle)
        throws ClassNotFoundException, IOException {
        list.insert(name, rectangle);
    }

    /**
     * Calls the skip list and memory manager dump command
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public void dump() throws ClassNotFoundException, IOException {
        list.dump();
        memManager.dump();
    }
    
    /**
     * Tells memory manager to flush 
     * @throws IOException
     */
    public void flushList() throws IOException {
        memManager.flush();
    }

    /**
     * @return number of elements in skip list
     */
    public int listSize() {
        return list.size();
    }

    /**
     * Returns a rectangle given an index
     * @param index position in skip list
     * @return rectangle that was desired
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public Rectangle get(int index) throws ClassNotFoundException, 
        IOException {
        return list.get(index);
    }
    
    /**
     * Returns all rectangles with a given name 
     * @param key name of the rectangle to search for
     * @return result of search
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public String[] searchAll(String key) throws ClassNotFoundException,
        IOException {
        return list.find(key);
    }
    
    /**
     * Removes a rectangle by a given name
     * @param key name of rectangle to remove
     * @return rectangle that was removed
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public Rectangle remove(String key) throws ClassNotFoundException,
        IOException {
        int result = list.remove(key);
        if (result == -1) {
            return null;
        }
        else {
            @SuppressWarnings("unchecked")
            KVpair<String, Rectangle> temp = (KVpair<String, Rectangle>) 
                    Serializer.deserialize(memManager.get(result));
            memManager.remove(result);
            memManager.flush();
            return temp.value();
        }
    }
    
    /**
     * Removes a rectangle by its coordinates
     * @param rect rectangle to remove
     * @return rectangle that was removed
     * @throws ClassNotFoundException
     * @throws IOException
     */
    public Rectangle remove(Rectangle rect) throws ClassNotFoundException, 
        IOException {
        int result = list.remove(rect);
        if (result == -1) {
            return null;
        }
        else {
            @SuppressWarnings("unchecked")
            KVpair<String, Rectangle> temp = (KVpair<String, Rectangle>) 
                    Serializer.deserialize(memManager.get(result));
            memManager.remove(result);
            memManager.flush();
            return temp.value();
        }
    }
    
   
    /**
     * Checks if rectangle is valid
     * @param x x position of rectangle
     * @param y y position of rectangle
     * @param w width of rectangle
     * @param h height of rectangle
     * @return result of check
     */
    public boolean check(int x, int y, int w, int h) {
        return !(x < 0 || x >= 1024 || y < 0 || y >= 1024 || w <= 0 || h <= 0
                || (x + w) > 1024 || (y + h) > 1024);
        
    }

}
