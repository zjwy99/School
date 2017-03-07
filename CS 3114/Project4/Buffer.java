/**
 * Class to represent a single buffer
 * 
 * @author Zach-Laptop
 * @version 2.0
 */
public class Buffer {
    private boolean dirty;
    private int index;
    private byte[] bytes;

    /**
     * Constructor for Buffer
     * @param index start position of data in disk
     * @param byteArray the bytes of the data
     */
    public Buffer(int index, byte[] byteArray) {
        this.bytes = byteArray;
        this.index = index;
        this.dirty = false;
    }

    /**
     * Sets the buffer to be dirty or not dirty
     * @param set what to set dirty to
     */
    public void setDirty(boolean set) {
        this.dirty = set;
    }

    /**
     * Checks if buffers needs to be written to disk
     * @return dirty value
     */
    public boolean isDirty() {
        return this.dirty;
    }

    /**
     * @return index of data in disk
     */
    public int getIndex() {
        return this.index;
    }

    /**
     * Sets index of the buffer
     * @param index new index for buffer
     */
    public void setIndex(int index) {
        this.index = index;
    }

    /**
     * @return Data of the buffer returned as an array
     */
    public byte[] getBytes() {
        return this.bytes;
    }

    /**
     * Set data to given data
     * @param byteArray value to be set
     */
    public void setBytes(byte[] byteArray) {
        this.bytes = new byte[byteArray.length];
        for (int i = 0; i < byteArray.length; ++i) {
            this.bytes[i] = byteArray[i];
        }
    }

    /**
     * @return size of the data
     */
    public int getSize() {
        return this.bytes.length;
    }

    /**
     * Sets particular values in data to given value
     * @param setIndex position of data that needs to be set
     * @param value data to be set
     */
    public void setByte(int setIndex, byte value) {
        this.bytes[setIndex] = value;
        setDirty(true);
    }

    /**
     * Makes a new copy of buffer
     * @return new copy of the buffer
     */
    public Buffer makeCopy() {
        Buffer copy = new Buffer(this.index, this.bytes);
        if (this.dirty) {
            copy.setDirty(true);
        }
        return copy;
    }
}