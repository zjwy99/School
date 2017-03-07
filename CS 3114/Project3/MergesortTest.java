import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import student.TestCase;

/**
 * 
 * @author Zach-Laptop
 * @version 1.0
 */
public class MergesortTest extends TestCase {

    /**
     * Set up
     */
    public void setUp() {

    }
    
    /**
     * Tests if something can be set in buffer and if both get methods 
     * works as intended
     * @throws IOException
     */
    public void testSet() throws IOException {
        String[] args = new String[3];
        args[0] = "big";
        args[1] = "15";
        args[2] = "stat";
        BufferManager test = new BufferManager(new File(args[0]), 
                Integer.parseInt(args[1]));
        byte[] temp = new byte[4];
        temp[0] = 1;
        temp[1] = 1;
        temp[2] = 1;
        temp[3] = 1;
        test.set(0, temp);
        temp = test.getBytes(0);
        for (int i = 0; i < 4; i++)
            assertTrue(temp[i] == 1);
        int key = test.get(0);
        assertTrue(key == 257);
        
    }
    
    /**
     * Tests if data can be written to file and be read properly
     * @throws IOException
     */
    public void testWriteRead() throws IOException {
        String[] args = new String[3];
        args[0] = "big";
        args[1] = "15";
        args[2] = "stat";
        BufferPool test = new BufferPool(Integer.parseInt(args[1]), 
                new File(args[0]));
        byte[] temp = new byte[4];
        temp[0] = 1;
        temp[1] = 1;
        temp[2] = 1;
        temp[3] = 1;
        test.insert(0, temp);
        test.write(0, temp);
        temp = test.read(0, 4);
        for (int i = 0; i < 4; i++)
            assertTrue(temp[i] == 1);
    }
    
    /**
     * Tests if buffers can be inserted into pool and be removed properly
     * @throws IOException
     */
    public void testInsert() throws IOException {
        String[] args = new String[3];
        args[0] = "big";
        args[1] = "15";
        args[2] = "stat";
        BufferPool test = new BufferPool(Integer.parseInt(args[1]), 
                new File(args[0]));
        byte[] temp = new byte[4];
        temp[0] = 1;
        temp[1] = 1;
        temp[2] = 1;
        temp[3] = 1;
        for (int i = 0; i < 16; i++)
            test.insert(0, temp);
        temp = test.read(0, 4);
        for (int i = 0; i < 4; i++)
            assertTrue(temp[i] == 1);
    }

    /**
     * Tests if file can be sorted 
     * @throws Exception
     */
    public void testSort() throws Exception {
        String[] args = new String[3];
        args[0] = "big";
        args[1] = "15";
        args[2] = "stat";
        Mergesort merge = new Mergesort();
        merge.main(args);
        CheckFile check = new CheckFile();
        assertTrue(check.checkFile(args[0]));
    }
}