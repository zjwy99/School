import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Client file for buffer manager to sort a given binary file
 * by using the merge sort algorithm 
 * @author Zach-Laptop
 * @version 2.0
 */
public class ExternalMergeSort {
    
    private BufferManager manager;
    private BufferManager manager2;
    private File tempFile;
    private int elementsToSort;

    /**
     * Constructor
     * @param filename name of file to be sorted
     * @param bufferSize max size of buffer pool
     * @throws FileNotFoundException
     * @throws IOException
     */
    public ExternalMergeSort(String filename, int bufferSize) 
        throws IOException {
        
        //Initializes Buffer manager for file to be sorted
        manager = new BufferManager(new File(filename), bufferSize);
        File toSort = new File(filename);
        elementsToSort =  (int) toSort.length() / manager.ELEMENT_SIZE;
        
        //Creates temp file and initializes Buffer manager for it
        tempFile = new File("temp");
        tempFile.createNewFile();
        tempFile.deleteOnExit();
        manager2 = new BufferManager(tempFile, bufferSize);

    }
    
    /**
     * Method to start the merge sort
     * @throws IOException
     */
    public void sort() throws IOException {
        mergeSort(0, elementsToSort - 1);
        manager.flush();
    }
    
    /**
     * Recursive method to split merge the elements of the disk file
     * @param low the start index
     * @param high the end index
     * @throws IOException
     */
    private void mergeSort(int low, int high) throws IOException {
        if (low < high) {
            int middle = (low + high) / 2;

            mergeSort(low, middle);
            mergeSort(middle + 1, high);

            merge(low, middle + 1, high);

        }
    }
    
    /**
     * Method to merge parts of the sub lists in file
     * @param low start index
     * @param middle index in the middle
     * @param high end index
     * @throws IOException
     */
    private void merge(int low, int middle, int high) throws IOException {

        int lowEnd = middle - 1;
        int k = low;

        // Number of elements in sub array
        int num = high - low + 1;

        //Sorts elements and places them in temp file
        while (low <= lowEnd && middle <= high) {
            if (manager.get(low) < manager.get(middle))
                manager2.set(k++, manager.getBytes(low++));
            else
                manager2.set(k++, manager.getBytes(middle++));
        }

        //Places the rest of the elements in temp file
        while (low <= lowEnd)
            manager2.set(k++, manager.getBytes(low++));

        while (middle <= high)
            manager2.set(k++, manager.getBytes(middle++));

        //Put elements back into the file that needs to be sorted
        for (int i = 0; i < num; i++, high--)
            manager.set(high, manager2.getBytes(high));
    }

}
