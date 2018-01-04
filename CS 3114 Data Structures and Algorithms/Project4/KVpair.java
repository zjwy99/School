import java.io.Serializable;

/**
 * Class to store a key and a element assocated with it 
 * @author Zachary Yee
 * @version 2.0
 * @param <Key>
 * @param <E>
 */
public class KVpair<Key, E> implements Serializable {
    private Key k;
    private E e;

    /**
     * Constructor
     * @param key key inserted into this kv pair
     * @param value element to be stored into this pair
     */
    KVpair(Key key, E value) {
        k = key;
        e = value;
    }

    /**
     * @return key associated with this kv pair
     */
    public Key key() {
        return k;
    }

    /**
     * @return value associated with this kv pair
     */
    public E value() {
        return e;
    }
}