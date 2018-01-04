/**
 * Class used to handle the nodes of the skip list
 * @author Zach
 * @version 0.1
 * @param <K>
 * @param <V>
 */
public class SkipNode<K, V> {
    /**
     * Element in skip node
     */
    V element;
    /**
     * Key in the skip node
     */
    K key;

    /**
     * Array of skip nodes, contains pointers to other nodes in skip list
     */
    public SkipNode<K, V>[] forward;

    /**
     * 
     * @return key of the skip node
     */
    public K key() {
        return key;
    }

    /**
     * 
     * @return element of the skip node
     */
    public V element() {
        return element;
    }

    /**
     * Constructor for skip node
     * @param key
     *            the key for the skip node
     * @param element
     *            the element for the skip node
     * @param level
     *            the amount of skip nodes this skip node point to
     */
    @SuppressWarnings("unchecked")
    public SkipNode(K key, V element, int level) {
        this.key = key;
        this.element = element;
        forward = (SkipNode<K, V>[]) new SkipNode[level + 1];
        for (int i = 0; i < level; i++)
            forward[i] = null;
    }

}