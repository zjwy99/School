import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Client file for buffer pool, 
 * handles the extraction of data from buffer pool
 * @author Zach-Laptop
 * @version 2.0
 */
public class BufferManager {
    
    private static final int BLOCK_SIZE = 4096;
    
    /**
     * Size of each piece of data in disk
     */
    public static final int ELEMENT_SIZE = 4;
    
    private BufferPool pool;
    
    /**
     * Pointer to stat file
     */
    public Statistics stat;
  
    /**
     * Constructor 
     * @param file to be managed by buffer pool
     * @param maxBuffers maximum size of buffers in pool
     * @throws FileNotFoundException
     */
    public BufferManager(File file, int maxBuffers) 
        throws FileNotFoundException {
        
        this.pool = new BufferPool(maxBuffers, file);
        stat = new Statistics();      
    }
 
    /**
     * Gets key value in buffer
     * @param index to get key value from
     * @return key value as an integer
     * @throws FileNotFoundException
     * @throws IOException
     */
    public int get(int index) throws IOException {
        Buffer buffer = this.getBuffer(index);
        byte[] bArray = buffer.getBytes();        
        int offset = (index * ELEMENT_SIZE) % BLOCK_SIZE;
        int key = ByteBuffer.wrap(bArray).getShort(offset);
        return key;
    }
    
    /**
     * Return entire element in buffer
     * @param index of element
     * @return element as a byte array
     * @throws IOException
     */
    public byte[] getBytes(int index) throws IOException {
        Buffer buffer = this.getBuffer(index);
        int start = (index * ELEMENT_SIZE) - buffer.getIndex();
        byte[] bytes = buffer.getBytes();
        byte[] temp = new byte[ELEMENT_SIZE];
        for (int i = 0; i < ELEMENT_SIZE; ++i) {
            temp[i] = bytes[start + i];
        }
        return temp;
    }
    
    /**
     * Sets a value in the buffer
     * @param index of value to be set
     * @param bytes data that will be set
     * @throws IOException
     */
    public void set(int index, byte[] bytes) throws IOException {
        Buffer buffer = this.getBuffer(index);
        int start = (index * ELEMENT_SIZE) % BLOCK_SIZE;
        for (int i = 0; i < ELEMENT_SIZE; i++) {
            buffer.setByte(start + i, bytes[i]);
        }
    }
   
    /**
     * Writes buffers to file and empties buffer pool
     * @throws IOException
     */
    public void flush() throws IOException {
        int writes = pool.flush();
        stat.diskWrites += writes;
    }
 
    /**
     * Gets a buffer in buffer pool
     * @param index of buffer to be obtained
     * @return buffer in the buffer pool
     * @throws IOException
     */
    private Buffer getBuffer(int index) throws IOException {
        
        //Sees if buffer exists in pool
        Buffer temp = pool.getBuffer(index * ELEMENT_SIZE); 
        
        //If buffer does not exist then create a new one
        if (temp == null) {
            
            //Obtains data from file
            int startBlock = (index * ELEMENT_SIZE) / BLOCK_SIZE;
            byte[] bArray = pool.read(startBlock * BLOCK_SIZE, BLOCK_SIZE);
            
            stat.diskReads++;  
            
            //Insert in pool
            pool.insert(startBlock * BLOCK_SIZE, bArray);
           
            stat.cacheMisses++;
            
            //Buffer now exists in buffer pool and gets buffer
            temp = pool.getBuffer(index * ELEMENT_SIZE); 
        } 
        else 
            stat.cacheHits++;  
        return temp;
    }
}
