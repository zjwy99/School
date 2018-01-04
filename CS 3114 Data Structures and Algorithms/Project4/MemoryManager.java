import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * 
 * @author Zach-Laptop
 * @version 1.0
 */
public class MemoryManager {
    
    private int buffSize;
    private BufferManager manager;
    private LinkedList<Freespace> freeBlocks;
    private int poolSize;
    
    /**
     * Constructor for memory manager
     * @param buffSize Buffer size
     * @param manager Buffer manager that the memory manager will use
     * @throws FileNotFoundException 
     */
    public MemoryManager(int buffSize, BufferManager manager) 
        throws FileNotFoundException {
        this.buffSize = buffSize;
        this.manager = manager;
        this.freeBlocks = new LinkedList<Freespace>();
        freeBlocks.insert(new Freespace(0, buffSize));
        this.poolSize = buffSize;
    }
    
    /**
     * 
     * @param data byte array to store 
     * @param size number of bytes to store
     * @return handle position for the given data
     * @throws IOException 
     */
    public int insert(byte[] data, int size) throws IOException {

        //Creates array to insert into file
        int startIndex = bestSpace(size);
        byte[] toInsert = new byte[size + 2];
        byte[] sizeByte = new byte[2];
        sizeByte = ByteBuffer.allocate(2).putShort((short)size).array();
        toInsert[0] = sizeByte[0];
        toInsert[1] = sizeByte[1];
        System.arraycopy(data, 0, toInsert, 2, size);
         
        if (startIndex != -1) { //Found free space
            
            int handlePos = freeBlocks.get(startIndex).getHandle();
            for (int i = 0; i < toInsert.length; i++) {
                manager.set(handlePos + i, toInsert[i]);
            }
            manager.flush();
            
            int newFreeSize = freeBlocks.get(startIndex).size() - (size + 2);
            if (newFreeSize > 0) {
                freeBlocks.get(startIndex).setSize(newFreeSize);
                int temp = freeBlocks.get(startIndex).getHandle();
                freeBlocks.get(startIndex).setHandle(temp + size + 2);
            }
            else {
                freeBlocks.delete(startIndex);
            }
            return handlePos;
            
        }
        else { //Creates new freespace and merges it into free block list
            
            freeBlocks.insert(new Freespace(poolSize, buffSize));
            poolSize += buffSize;
            merge();
            return insert(data, size);
            
        }
        
        
    }
    
    /**
     * Inserts data when best handle position is known
     * @param data the bytes to be inserted
     * @param size number of bytes to store
     * @param handle position to store data, 
     *        this information is known before inserting
     * @return handle position
     * @throws IOException 
     */
    public int insert(byte[] data, int size, int handle) throws IOException {
        byte[] toInsert = new byte[size + 2];
        byte[] sizeByte = new byte[2];
        sizeByte = ByteBuffer.allocate(2).putShort((short) size).array();
        
        toInsert[0] = sizeByte[0];
        toInsert[1] = sizeByte[1];
        System.arraycopy(data, 0, toInsert, 2, size);
        
        //places bytes into file
        for (int i = 0; i < toInsert.length; i++) {
            manager.set(handle + i, toInsert[i]);
        }
        manager.flush();
        
        for (int i = 0; i < freeBlocks.getSize(); i++) {
            if (freeBlocks.get(i).getHandle() == handle) {
                int newFreeSize = freeBlocks.get(i).size() - (size + 2);
                if (newFreeSize > 0) {
                    freeBlocks.get(i).setSize(newFreeSize);
                    int temp = freeBlocks.get(i).getHandle();
                    freeBlocks.get(i).setHandle(temp + size + 2);
                }
                else {
                    freeBlocks.delete(i);
                }
            }
        }
        
        return handle;
    }
    
    /**
     * Gets the bytes associated with the handle
     * @param handle position of data
     * @return the bytes of the array
     * @throws IOException 
     */
    public byte[] get(int handle) throws IOException {
        byte[] length = new byte[2];
        
        
        //Gets bytes from file
        length[0] = manager.get(handle);
        length[1] = manager.get(handle + 1);
        handle += 2;
        int size = ByteBuffer.wrap(length).getShort();
        byte[] check = new byte[size];
        for (int i = 0; i < size; i++) {
            check[i] = manager.get(handle + i);
        }
        
        
        return check;
    }
        
    /**
     * Removes bytes 
     * @param handle starting position of bytes to remove
     * @return handle position of removed bytes
     * @throws IOException
     */
    public int remove(int handle) throws IOException {
        byte[] length = new byte[2];
        
        
        
        length[0] = manager.get(handle);
        manager.set(handle, (byte)0);
        length[1] = manager.get(handle + 1);
        manager.set(handle + 1, (byte)0);
        handle += 2;
        int size = ByteBuffer.wrap(length).getShort();
        for (int i = 0; i < size; i++) {
            manager.set(handle + i, (byte) 0);
        }
        manager.flush();
        
        Freespace newSpace = new Freespace(handle - 2, size + 2);
        freeBlocks.insert(newSpace);
        merge();
        return handle;
    }
    
    /**
     * Finds the best spot to put the data
     * @param size the number of bytes to insert
     * @return index in the list of free spaces
     */
    private int bestSpace(int size) {
        int current = -1;
        int freeSize = -1;
        int index = -1;
        
        //If list is empty then exit method
        if (freeBlocks.getSize() == 0) {
            return -1;
        }
        
        for (int i = 0; i < freeBlocks.getSize(); i++) {
            current = freeBlocks.get(i).size();
            if (current >= size + 2) {
                if (freeSize == -1) {
                    freeSize = current; 
                    index = i;
                }
                else if (current < freeSize) {
                    freeSize = current;
                    index = i;
                }
            }
        }
        return index;
    }
     
    /**
     * Merges all the free spaces 
     */
    private void merge() {
        
        if (freeBlocks.getSize() < 2) {
            return;
        }
        
        Freespace current = null;
        Freespace temp = null;
        
        for (int i = 0; i < freeBlocks.getSize(); i++) {
            
            current = freeBlocks.get(i);
            
            for (int x = i + 1; x < freeBlocks.getSize(); x++) {
                
                temp = freeBlocks.get(x);
                
                int endPoint = 0;
                boolean isConnected = false;
                //Determines if free space is on the left or right
                if (current.getHandle() < temp.getHandle()) {
                    endPoint = current.getHandle() + current.size() - 1;
                    isConnected = (endPoint == temp.getHandle() - 1);
                }
                else {
                    endPoint = temp.getHandle() + temp.size() - 1;
                    isConnected = (endPoint == current.getHandle() - 1);
                }
                
                if (isConnected) {
                    int newSize = current.size() + temp.size();
                    
                    int position = 0;
                    
                    if (current.getHandle() < temp.getHandle()) {
                        position = current.getHandle();
                    }
                    else {
                        position = temp.getHandle();
                    }
                    
                    Freespace newSpace = new Freespace(position, newSize);
                    freeBlocks.delete(x);
                    freeBlocks.delete(i);
                    freeBlocks.insert(newSpace);
                    
                    merge();
                    return;
                    
                }  
            }
        }  
    }
    

    
    /**
     * Memory manager dump
     */
    public void dump() {
        System.out.println("Freelist Blocks:");
        for (int i = 0; i < freeBlocks.getSize(); i++) {
            System.out.print("(" + freeBlocks.get(i).getHandle() + ", ");
            System.out.print(freeBlocks.get(i).size() + ")\n");
        }
    }
    
    /**
     * Tells buffer manager to flush 
     * @throws IOException
     */
    public void flush() throws IOException {
        manager.flush();
    }

}
