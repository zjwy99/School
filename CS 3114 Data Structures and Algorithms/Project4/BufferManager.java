import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Client file for buffer pool, 
 * handles the extraction of data from buffer pool
 * @author Zach-Laptop
 * @version 2.0
 */
public class BufferManager {
    
    private int blockSize; 
    
 
    private BufferPool pool;
    
    /**
     * Constructor
     * @param file disk file that the buffer manager will manipulate
     * @param maxBuffers maximum number of buffers in buffer pool
     * @param blockSize size of each block in buffer pool
     * @throws FileNotFoundException
     */
    public BufferManager(File file, int maxBuffers, int blockSize)
        throws FileNotFoundException {
        
        this.pool = new BufferPool(maxBuffers, file);
        this.blockSize = blockSize;
    }
 
    /**
     * Gets byte value in buffer
     * @param index to get key value from
     * @return key value as an integer
     * @throws FileNotFoundException
     * @throws IOException
     */
    public byte get(int index) throws IOException {
        Buffer buffer = this.getBuffer(index);
        int start = index - buffer.getIndex();
        return buffer.getBytes()[start];
    }
    
    
    /**
     * Sets a value in the buffer
     * @param index of value to be set
     * @param toSet byte that will be set
     * @throws IOException
     */
    public void set(int index, byte toSet) throws IOException {
        Buffer buff = getBuffer(index);
        
        int start = index - buff.getIndex();
        
        buff.setByte(start, toSet);
    }
   
    /**
     * Writes buffers to file and empties buffer pool
     * @throws IOException
     */
    public void flush() throws IOException {
        pool.flush();
    }
 
    /**
     * Gets a buffer in buffer pool
     * @param index of buffer to be obtained
     * @return buffer in the buffer pool
     * @throws IOException
     */
    private Buffer getBuffer(int index) throws IOException {
        
        //Sees if buffer exists in pool
        Buffer temp = pool.getBuffer(index); 
        
        //If buffer does not exist then create a new one
        if (temp == null) {
            
            //Obtains data from file
            int startBlock = (index) / blockSize;
            byte[] bArray = pool.read(startBlock * blockSize, blockSize);
            
            
            //Insert in pool
            pool.insert(startBlock * blockSize, bArray);
           
            
            //Buffer now exists in buffer pool and gets buffer
            temp = pool.getBuffer(index); 
        } 
        return temp;
    }
}
