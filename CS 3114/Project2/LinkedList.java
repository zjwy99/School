/**
 * Linked List
 * @author Zach
 * @version 1.0
 * @param <T>
 */
public class LinkedList<T> {
    private int size;
    private Node head;
    
    /**
     * Default constructor to initialize list
     */
    public LinkedList() {
        size = 0;
        head = null;
    }
    
    /**
     * @return the size of the list
     */
    public int getSize() {
        return size;
    }
       
    /**
     * Inserts a element at the end of the list
     * @param element to be inserted
     */
    public void insert(T element) {
        
        Node tempNode = new Node(element);
        if (head == null) {
            head = tempNode;
        }
        else {
            Node current = head;
            
            
            while (current.getNext() != null) 
                current = current.getNext();
            
            current.setNext(tempNode);
            
        }
        size++;
    }

    /**
     * Deletes a element of the list given a index
     * @param index of the element to be removed
     * @return result of the removal
     */
    public boolean delete(int index) {
        if (index == 0) {
            head = head.getNext();
            size--;
            return true;
        }
        Node current = head;
       
        for (int i = 0; i < index; i++)
            current = current.getNext();            
        if (current.getNext() == null)
            current.setNext(null);
        else {
            current.setElement(current.getNext().getElement());
            current.setNext(current.getNext().getNext());
        }
        size--;
        return true;
       
    }
    
    /**
     * Checks to see if element exists in the list
     * @param element to check for
     * @return result 
     */
    public boolean exists(T element) {
        
        if (size == 0)
            return false;
        
        else {
            Node current = head;
            
            while (current != null) {
                if (current.getElement().equals(element))
                    return true;
                
                current = current.getNext();
            }
        }
        return false;
    }
    
    /**
     * Gets a element in the list
     * @param index of element to get
     * @return element in the list
     */
    public T get(int index) {
        Node current = head;
        T temp = current.element;
        if (index == 0)
            return temp;
        else {
            for (int i = 0; i < index; i++)
                current = current.getNext();
            temp = current.getElement();
        }        
        return temp;
    }
          
    /**
     * Private class to handle nodes of the list 
     * @author Zach
     * @version 1.0
     */
    private class Node {
        
        Node next; // next node in the list
        T element; // current element of the point
        
        /**
         * Constructor to create a Node
         * @param element to be inserted in the list
         */
        public Node(T element) {
            this.element = element;
            next = null;
        }
        
        /**
         * @return element of the node
         */
        public T getElement() {
            return element;
        }
        
        /**
         * Sets an element for the node 
         * @param element to be set
         */
        public void setElement(T element) {
            this.element = element;
        }
        
        /**
         * @return next node in the list
         */
        public Node getNext() {
            return next;
        }
        
        /**
         * Sets the next node in the list
         * @param nextNode the node to be set
         */
        public void setNext(Node nextNode) {
            next = nextNode;
        }
    }

}
