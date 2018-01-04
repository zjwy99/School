import java.io.*;



public class CheckFile
{
    
    public CheckFile()
    {
        // empty constructor
    }


    
    public boolean checkFile(String filename)
        throws Exception
    {
        boolean isError = false;
        DataInputStream in =
            new DataInputStream(new BufferedInputStream(new FileInputStream(
                filename)));
        // Prime with the first record
        short key2 = in.readShort();
        in.readShort();
        int reccnt = 0;
        try
        {
            while (true)
            {
                short key1 = key2;
                reccnt++;
                key2 = in.readShort();
                in.readShort();
                if (key1 > key2)
                {
                    isError = true;
                }
            }
        }
        catch (EOFException e)
        {
            System.out.println(reccnt + " records processed");
        }
        in.close();
        return !isError;
    }
}
