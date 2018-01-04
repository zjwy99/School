import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

/**
 * Class for holding and writing various statistics of program
 * @author Zach-Laptop
 * @version 1.0
 */
public class Statistics {
    /**
     * Number of cache hits
     */
    public static int cacheHits;
    
    /**
     * Number of cache misses
     */
    public static int cacheMisses;
    
    /**
     * Number of disk reads
     */
    public static int diskReads;
    
    /**
     * Number of disk writes
     */
    public static int diskWrites;
    
    /**
     * How long it takes for sort to finish
     */
    public static long executionTime;
    
    /**
     * Filename for stat file
     */
    public static String filename;
    
    /**
     * Default Constructor
     */
    public Statistics() {
        cacheHits = 0;
        cacheMisses = 0;
        diskReads = 0;
        diskWrites = 0;
        executionTime = 0;
    }
       
    /**
     * 
     * @param statFile name of file where data is to be written
     * @param sortFile name of sorted file
     * @throws IOException
     */
    public void writeToFile(String statFile, String sortFile) 
        throws IOException {
        File stat = new File(statFile);
        stat.createNewFile();
        FileWriter f = new FileWriter(stat.getAbsolutePath(), true);
        BufferedWriter write = new BufferedWriter(f);
        write.write("Filename: " + sortFile + "\n");
        write.write("Cache Hits: " + cacheHits + "\n");
        write.write("Cache Misses: " + cacheMisses + "\n");
        write.write("Disk Reads: " + diskReads + "\n");
        write.write("Disk Writes: " + diskWrites + "\n");
        write.write("Execution Time: " + executionTime + "\n");
        write.write("\n");
        write.close();
    }
    
}
