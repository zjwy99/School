import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * 
 * @author Zach-Laptop
 * @version 2.0
 */
public class BufferPool {
    
    private Buffer[] buffers;
    private int maxBuffers;
    
    /**
     * Number of buffers in pool
     */
    public int cSize;
    
    private RandomAccessFile r;
    
    /**
     * Constructor for Buffer pool
     * @param maxBuffers maximum number of buffers in pool
     * @param file file to be handled by buffer pool
     * @throws FileNotFoundException
     */
    public BufferPool(int maxBuffers, File file) throws FileNotFoundException {
        this.buffers = new Buffer[maxBuffers];
        this.maxBuffers = maxBuffers;
        cSize = 0;
        r = new RandomAccessFile(file, "rw");
    }
  
    /**
     * Inserts a buffer into pool
     * @param index position of buffer 
     * @param byteArray data of the buffer
     * @throws IOException
     */
    public void insert(int index, byte[] byteArray) throws IOException {
        Buffer removedBuffer = null;
        
        //If buffer pool is full,
        //then remove last buffer and puts new buffer on top 
        if (cSize == this.maxBuffers) {
            
            removedBuffer = buffers[buffers.length - 1].makeCopy();
            
            //Creates new buffer and places on top
            Buffer newBuffer = buffers[buffers.length - 1];
            newBuffer.setIndex(index);
            newBuffer.setBytes(byteArray);
            newBuffer.setDirty(false);
            System.arraycopy(buffers, 0, buffers, 1, buffers.length - 1);
            this.buffers[0] = newBuffer;
            
            //Writes the removed buffer to file
            write(removedBuffer.getIndex(), removedBuffer.getBytes());
        } 
        else {
            //Puts new buffer on top
            System.arraycopy(buffers, 0, buffers, 1, buffers.length - 1);
            this.buffers[0] = new Buffer(index, byteArray);
            cSize++;
        }
    }
    
    /**
     * Gets the buffer that the index is in 
     * @param index position in file
     * @return buffer in buffer pool, null if not found
     */
    public Buffer getBuffer(int index) {
        Buffer currentBuffer = null;
        for (int i = 0; i < cSize; i++) {
            currentBuffer = buffers[i];
            
            int startPosition = currentBuffer.getIndex();
            int end = currentBuffer.getSize() + startPosition;
            if (index >= startPosition && index < end) {
                return currentBuffer;
            }
            
        }
        return null;
    }
    
    /**
     * Writes buffers to disk and clears buffer pool
     * @return number of times the disk has been written to
     * @throws IOException
     */
    public int flush() throws IOException {
       
        Buffer currentBuffer = null;
        int writes = 0;
        //Goes through buffers to see which buffers needs to be written to file
        for (int i = 0; i < cSize; ++i) {
            currentBuffer = buffers[i];
            
            //If buffer has been changed write to file
            if (currentBuffer.isDirty()) {
                byte[] bytes = currentBuffer.getBytes();
                write(currentBuffer.getIndex(), bytes);
                writes++;
                buffers[i] = null;
            }
        }
        //resets size of buffer pool
        cSize = 0;
        return writes;
    }
    
    /**
     * Reads data from disk
     * @param index position to be read
     * @param size of data to be read
     * @return data as a byte array
     * @throws IOException
     */
    public byte[] read(int index, int size) throws IOException {
        byte[] out = new byte[size];
        r.seek(index);
        r.read(out);
        
        return out;
    }
    
    /**
     * Writes data to disk
     * @param index position to be written to 
     * @param toWrite data to written on disk
     * @throws IOException
     */
    public void write(int index, byte[] toWrite) throws IOException {
        r.seek(index);
        r.write(toWrite);
    }

}
