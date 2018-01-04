import java.io.IOException;

import student.TestCase;

/**
 * @author CS 3114 Staff
 * @version April 13, 2016
 */
public class RectangleDiskTest extends TestCase {

    /**
     * This method sets up the tests that follow.
     */
    public void setUp() {
        // no op
    }
    
    /**
     * Test bad input
     * @throws IOException 
     * @throws NumberFormatException 
     */
    public void testSyntax() throws NumberFormatException, IOException {
        String [] param = {"SyntaxTest.txt", "d", "1", "64"};
        RectangleDisk.main(param);
        assertEquals("Rectangle rejected: (r_r, -1, -20, 3, 4)\n"
                + "Rectangle rejected: (rec, 7, -8, 1, 3)\n"
                + "Rectangle rejected: (virtual_rec0, 1, 1, 0, 0)\n"
                + "Rectangle rejected: (virtual_REC0, 0, 0, 11, 0)\n"
                + "Rectangle rejected: (inExistRec_0, 1, 1, -1, -2)\n"
                + "Rectangle rejected: (11, 11, 0, 0)\n"
                + "Rectangle rejected: (11, 11, 11, 0)\n"
                + "Intersection pairs:\n"
                + "Rectangle not found: r_r\n"
                + "Rectangle not removed: r_r\n"
                + "Rectangle rejected: (1, 1, 0, 0)\n"
                + "Rectangles intersecting region (-5, -5, 20, 20):\n",
                systemOut().getHistory());

    }
    
    /**
     * This method gets you credit for testing the Serializer initialization.
     *
     * @throws IOException
     * @throws ClassNotFoundException
     */
    public void testSerialize() throws IOException, ClassNotFoundException {
        Serializer serializer = new Serializer();
        assertNotNull(serializer);
        Integer pre = new Integer(10);
        byte[] serialized = Serializer.serialize(pre);
        Integer post = (Integer) Serializer.deserialize(serialized);
        assertEquals(pre.intValue(), post.intValue());
    }
      
    /**
     * Uses custom test file to test 
     * @throws IOException
     * @throws ClassNotFoundException
     */
    public void testS() throws IOException, ClassNotFoundException {
        String [] param = {"InsertTests.txt", "d", "1", "64"};
        RectangleDisk.main(param);
        assertEquals("Rectangle inserted: (rectA, 1, 0, 2, 4)\n"
                + "Rectangle inserted: (b, 2, 0, 4, 8)\n"
                + "Rectangle inserted: (rectC, 4, 0, 9, 6)\n",
                systemOut().getHistory());
        
        
    }
    
    /**
     * Tests remove, tested multiple time because skip list can have 
     * variables level sizes 
     * @throws NumberFormatException
     * @throws IOException
     */
    public void testRemove() throws NumberFormatException, IOException {
        String [] param = {"removetests.txt", "d", "1", "64"};
        for (int i = 0; i < 50; i++) {
            RectangleDisk.main(param);
        }
        assertTrue(systemOut().getHistory().endsWith(
                  "Rectangle not removed: c\n"
                + "Rectangle not removed: (1, 1, 1, 1)\n"
                + "Rectangle removed: (rectA, 1, 0, 2, 4)\n"
                + "Rectangle removed: (rectC, 4, 0, 9, 6)\n"
                + "Rectangle removed: (b, 2, 0, 4, 8)\n"));
    }
    
    /**
     * 
     * @throws NumberFormatException
     * @throws IOException
     */
    public void testList() throws NumberFormatException, IOException {
        String [] param = {"listtests.txt", "d", "1", "1"};
        RectangleDisk.main(param);
        assertTrue(systemOut().getHistory().endsWith(
                "Intersection pairs:\n"
              + "(b, 2, 0, 4, 8 | rectA, 1, 0, 2, 4)\n"
              + "(b, 2, 0, 4, 8 | rectC, 4, 0, 9, 6)\n"
              + "Rectangles intersecting region (1, 1, 1, 1):\n"
              + "(rectA, 1, 0, 2, 4)\n"));
    }
    
    /**
     * @throws IOException 
     * @throws NumberFormatException 
     * 
     */
    public void testDump() throws NumberFormatException, IOException {
        String [] param = {"dumptest.txt", "d", "1", "64"};
        RectangleDisk.main(param);
        assertTrue(systemOut().getHistory().contains("SkipList size is: 3\n"
                + "Freelist Blocks:\n"
                + "("));
    }
    
    /**
     * Test designed to test certain aspects of region search and intersections
     * @throws NumberFormatException
     * @throws IOException
     */
    public void testC() throws NumberFormatException, IOException {
        String [] param = {"coveragetest.txt", "d", "1", "64"};
        RectangleDisk.main(param);
        assertTrue(systemOut().getHistory().contains(
                  "Rectangles intersecting region (900, 5, 15, 15):\n"
                + "(b, 0, 0, 910, 10)\n"
                + "(c, 0, 0, 1000, 10)\n"
                + "Rectangles intersecting region (9, 900, 5, 500):\n"
                + "(d, 0, 0, 10, 1000)\n"
                + "Intersection pairs:\n"
                + "(a, 0, 0, 1000, 10 | a, 10, 10, 15, 15)\n"
                + "(a, 0, 0, 1000, 10 | a, 10, 10, 15, 15)\n"
                + "(a, 0, 0, 1000, 10 | b, 0, 0, 910, 10)\n"
                + "(a, 0, 0, 1000, 10 | c, 0, 0, 1000, 10)\n"
                + "(a, 0, 0, 1000, 10 | d, 0, 0, 10, 1000)\n"
                + "(a, 10, 10, 15, 15 | a, 10, 10, 15, 15)\n"
                + "(a, 10, 10, 15, 15 | b, 0, 0, 910, 10)\n"
                + "(a, 10, 10, 15, 15 | b, 11, 11, 5, 5)\n"
                + "(a, 10, 10, 15, 15 | b, 11, 11, 5, 5)\n"
                + "(a, 10, 10, 15, 15 | c, 0, 0, 1000, 10)\n"
                + "(a, 10, 10, 15, 15 | d, 0, 0, 10, 1000)\n"
                + "(a, 10, 10, 15, 15 | b, 0, 0, 910, 10)\n"
                + "(a, 10, 10, 15, 15 | b, 11, 11, 5, 5)\n"
                + "(a, 10, 10, 15, 15 | b, 11, 11, 5, 5)\n"
                + "(a, 10, 10, 15, 15 | c, 0, 0, 1000, 10)\n"
                + "(a, 10, 10, 15, 15 | d, 0, 0, 10, 1000)\n"
                + "(b, 0, 0, 910, 10 | c, 0, 0, 1000, 10)\n"
                + "(b, 0, 0, 910, 10 | d, 0, 0, 10, 1000)\n"
                + "(b, 11, 11, 5, 5 | b, 11, 11, 5, 5)\n"
                + "(c, 0, 0, 1000, 10 | d, 0, 0, 10, 1000)\n"));
    }
    
    
}
