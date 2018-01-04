import student.TestCase;

/**
 * @author CS3114 staff
 * @version 2
 */
public class Point2Test
    extends TestCase
{

    /**
     * This method sets up the tests that follow.
     */
    public void setUp()
    {
        
    }

    // ----------------------------------------------------------
    /**
     * Test command parser syntax on mostly bad input
     *
     * @throws Exception
     */
    public void testSyntax1() throws Exception {
        String[] args = new String[1];
        args[0] = "P2SyntaxTest1.txt";
        Point2.main(args);
        assertEquals("Point rejected: (r_r, -1, -20)\n"
                + "Point rejected: (rec, 7, -8)\n" + "Duplicate points:\n"
                + "SkipList dump:\n" + "Node has depth 1, Value (null)\n"
                + "SkipList size is: 0\n" + "QuadTree dump:\n"
                + "Node at 0, 0, 1024: Empty\n" + "1 quadtree nodes printed\n"
                + "Point not found: r_r\n" + "Point not removed: r_r\n"
                + "Point rejected: (1, -1)\n" + "Point not found: (1, 1)\n"
                + "Points intersecting region (-5, -5, 20, 20):\n"
                + "1 quadtree nodes visited\n"
                + "Rectangle rejected: (5, 5, 4, -2)\n", systemOut()
                .getHistory());
    }

    // ----------------------------------------------------------
    /**
     * Simple test of all the "good" command outputs
     *
     * @throws Exception
     */
    public void testSyntax2() throws Exception {
        String[] args = new String[1];
        args[0] = "P2SyntaxTest2.txt";
        Point2.main(args);
        assertTrue(systemOut().getHistory().endsWith(
                "SkipList size is: 2\n" + "QuadTree dump:\n"
                        + "Node at 0, 0, 1024:\n" + "(r_42, 1, 20)\n"
                        + "(far, 200, 200)\n" + "1 quadtree nodes printed\n"));
    }

    /**
     * tests for no input
     */
    public void testError() {
        String[] args = new String[1];
        args[0] = "";
        Point2.main(args);
    }

    /**
     * Removal tests
     */
    public void testRemoval() {
        String[] args = new String[1];
        args[0] = "removeTest2.txt";
        Point2.main(args);
        assertTrue(systemOut().getHistory().endsWith(
                "Node at 0, 0, 1024: Empty\n1 quadtree nodes printed\n"));
        QuadTree tree = new QuadTree();

        /*
         * Tests if the same point was removed from both the skip list & quad
         * tree. Points have same location, different name
         */
        SkipList<String, Point> list = new SkipList<String, Point>();
        Point a = new Point(1, 1, "test1");
        Point b = new Point(1, 1, "test2");
        Point c = new Point(1, 1, "test3");
        Point d = new Point(1, 1, "test4");
        tree.insert(a);
        tree.insert(b);
        tree.insert(c);
        tree.insert(d);
        list.insert(a.getName(), a);
        list.insert(b.getName(), b);
        list.insert(c.getName(), c);
        list.insert(d.getName(), d);
        Point listR = list.remove("test3").element;
        Point treeR = tree.remove(listR);
        assertTrue(listR == treeR);
        listR = list.remove("test1").element;
        treeR = tree.remove(listR);
        assertTrue(listR == treeR);
        listR = list.remove("test2").element;
        treeR = tree.remove(listR);
        assertTrue(listR == treeR);
        listR = list.remove("test4").element;
        treeR = tree.remove(listR);
        assertTrue(listR == treeR);
        assertTrue(systemOut().getHistory().endsWith(
                "Node at 0, 0, 1024: Empty\n1 quadtree nodes printed\n"));

        /*
         * Points have same name, different location
         */
        a = new Point(0, 0, "test1");
        b = new Point(0, 1, "test1");
        c = new Point(1, 0, "test1");
        d = new Point(1, 1, "test1");
        tree.insert(a);
        tree.insert(b);
        tree.insert(c);
        tree.insert(d);
        list.insert(a.getName(), a);
        list.insert(b.getName(), b);
        list.insert(c.getName(), c);
        list.insert(d.getName(), d);
        treeR = tree.remove(1, 1);
        listR = list.remove(treeR).element;
        assertTrue(listR == treeR);
        treeR = tree.remove(1, 0);
        listR = list.remove(treeR).element;
        assertTrue(listR == treeR);
        treeR = tree.remove(0, 1);
        listR = list.remove(treeR).element;
        assertTrue(listR == treeR);
        treeR = tree.remove(0, 0);
        listR = list.remove(treeR).element;
        assertTrue(listR == treeR);

    }

    /**
     * runs test 100 times, for skiplist because of its random nature
     */
    public void testRepeat() {
        for (int i = 0; i < 100; i++)
            testRemoval();
    }

    /**
     * Tests if you can put more than 3 of the same point in a node
     */
    public void testSameInsert() {
        String[] args = new String[1];
        args[0] = "sameInsert.txt";
        Point2.main(args);
        assertTrue(systemOut().getHistory().endsWith(
                "(a, 500, 500)\n" + "(b, 500, 500)\n" + "(c, 500, 500)\n"
                        + "(d, 500, 500)\n" + "(e, 500, 500)\n"
                        + "1 quadtree nodes printed\n"));

    }

    /**
     * Tests region search
     */
    public void testRegionSearch() {
        String[] args = new String[1];
        args[0] = "regionSearch.txt";
        Point2.main(args);
        String temp = systemOut().getHistory();
        assertTrue(temp.contains("Point found: (a, 0, 0)\n"));
        assertTrue(temp.contains("Point found: (a, 200, 0)\n"));
        assertTrue(temp.contains("Point found: (a, 0, 200)\n"));
        assertTrue(temp.contains("Point found: (a, 200, 200)\n"));

    }

    /**
     * More removal tests, to see if merging works
     */
    public void testMoreRemoval() {
        String[] args = new String[1];
        args[0] = "moreRemoval.txt";
        Point2.main(args);
        String temp = systemOut().getHistory();
        assertTrue(temp.contains("(a, 0, 0)\n(b, 200, 0)\n(c, 0, 200)\n1 "
                + "quadtree nodes printed"));
        assertTrue(temp.contains("(a, 0, 0)\n(b, 200, 0)\n(d, 200, 200)\n1 "
                + "quadtree nodes printed"));
        assertTrue(temp.contains("(a, 0, 0)\n(c, 0, 200)\n(d, 200, 200)\n1 "
                + "quadtree nodes printed"));
        
    }
    
    /**
     * More removal to test for remove by name
     */
    public void testevenMoreRemoval() {
        String[] args = new String[1];
        args[0] = "evenMoreRemoval.txt";
        Point2.main(args);
        String temp = systemOut().getHistory();
        assertTrue(temp.contains("(a, 0, 0)\n(c, 0, 200)\n(d, 200, 200)\n1 "
                + "quadtree nodes printed"));
        assertTrue(temp.contains("(a, 0, 0)\n(b, 200, 0)\n(d, 200, 200)\n1 "
                + "quadtree nodes printed"));
        assertTrue(temp.contains("(a, 0, 0)\n(b, 200, 0)\n(c, 0, 200)\n1 "
                + "quadtree nodes printed"));
    }
    
    /**
     * Tests if it can find duplicates on all internal nodes
     */
    public void testDuplicate() {
        String[] args = new String[1];
        args[0] = "duplicate.txt";
        Point2.main(args);
        String temp = systemOut().getHistory();
        assertTrue(temp.contains("0, 0\n200, 0\n0, 200\n"
                + "200, 200"));
    }
}
