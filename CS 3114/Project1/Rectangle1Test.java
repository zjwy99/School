import student.TestCase;

/**
 * Class used for testing program 
 * @author Zachary Yee
 * @version 2
 */
public class Rectangle1Test extends TestCase {

    /**
     * This method sets up the tests that follow.
     */
    public void setUp() {

    }

    /**
     * Tests project 1 with given test cases and custom test cases 
     */
    public void testRInit() {
        Rectangle1 dum = new Rectangle1();
        assertNotNull(dum);
        Rectangle1.main(new String[] { "" });
        Rectangle1.main(new String[] { "SyntaxTest.txt" });
        
        /*
         * Repeats tests 100 times to check for any errors 
         * when inserting and removing elements from skip list.
         * This is because when making new entries for the skip list
         * the number of skip nodes per entry is randomly chosen.
         */
        for (int i = 0; i < 100; i++) {
            Rectangle1.main(new String[] { "MyTests.txt" });
        }
        
    }
    
   
}
