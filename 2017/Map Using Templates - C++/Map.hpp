/*Some references
Map::erase - takes in an iterator pointing to an object, erases it, returns ???
- percolate binary search tree
//http://www.geeksforgeeks.org/binary-search-tree-set-2-delete/
http://www.cplusplus.com/reference/map/map/

Class SBBSTNode
//https://www.sanfoundry.com/cpp-program-implement-self-balancing-binary-tree/
//https://www.geeksforgeeks.org/avl-tree-set-2-deletion/
// Java program for deletion in AVL Tree

Previous PATH variable:
/home/ubuntu/.nvm/versions/node/v6.11.2/bin:/usr/local/rvm/gems/ruby-2.4.0/bin:/usr/local/rvm/gems/ruby-2.4.0@global/bin:/usr/local/rvm/rubies/ruby-2.4.0/bin:/mnt/shared/bin:/home/ubuntu/workspace/node_modules/.bin:/home/ubuntu/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/mnt/shared/sbin:/opt/gitl:/opt/go/bin:/mnt/shared/c9/app.nw/bin:/usr/local/rvm/bin
*/
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <initializer_list>
using namespace std;
namespace cs540 {

//T = the pair
template <class Key_T, class Mapped_T> //
class Node
{
    public:
    using ValueType = std::pair<const Key_T, Mapped_T>;
    
    bool isSentinel;
    int height;
    ValueType * nodeVal;
    Node<Key_T, Mapped_T> *left;
    Node<Key_T, Mapped_T> *right;
    Node<Key_T, Mapped_T> * pred;
    Node<Key_T, Mapped_T> * suc;
    bool isNew;
    
    Node<Key_T, Mapped_T>():nodeVal{NULL}
    {
        isSentinel = false;
        left   = NULL;
        right  = NULL;
        pred = NULL;
        suc = NULL;
        height = 1;  // new node is initially added at leaf
        isNew = true;
    }
 
    Node<Key_T, Mapped_T>(std::pair<const Key_T, Mapped_T> value):nodeVal{new ValueType(value)}
    {
        isSentinel = false;
        left   = NULL;
        right  = NULL;
        pred = NULL;
        suc = NULL;
        height = 1;  // new node is initially added at leaf
        isNew = true;
    }
    
    //Copy Constructor
    Node<Key_T, Mapped_T>(Node<Key_T, Mapped_T> * toCopy)
    {
        nodeVal = toCopy->nodeVal;
        isSentinel = toCopy->isSentinel;
        left   = toCopy->left;
        right  = toCopy->right;
        pred = toCopy->pred;
        suc = toCopy->suc;
        height = toCopy->height;  // new node is initially added at leaf
        isNew = false;
    }
    
    ~Node<Key_T, Mapped_T>(){
      if (isSentinel == false) {
        delete(nodeVal);
      }
    }
    
};
template <class Key_T, class Mapped_T>
class Tree
{
    public:
    using ValueType = std::pair<const Key_T, Mapped_T>;
    Node<Key_T, Mapped_T> * root;
    Node<Key_T, Mapped_T> * sentinel;
    
    //Creates a new tree
    Tree<Key_T, Mapped_T>() {
        sentinel = new Node<Key_T, Mapped_T>();
        sentinel->isSentinel = true;
        sentinel->suc = sentinel->pred = sentinel;
        sentinel->isNew = false;
        root = NULL;
    }
    
    //Copy constructor of the tree.
    Tree<Key_T, Mapped_T>(const Tree<Key_T, Mapped_T> & toCopy) {
        root = NULL;
        sentinel = new Node<Key_T, Mapped_T>();
        sentinel->isSentinel = true;
        sentinel->suc = sentinel->pred = sentinel;
        sentinel->isNew = false;
        if(toCopy.root != NULL) {
            root = insert(root, *toCopy.root->nodeVal, root, 0);
        }
        Node<Key_T, Mapped_T> * current = toCopy.sentinel->suc;
        while (current->isSentinel == false) {
            insert(root, *current->nodeVal, root, 0);
            current = current->suc;
        }
    }
 
    // A utility function to get height of the tree
    int height(Node<Key_T, Mapped_T> * N)
    {
        if (N == NULL)
             return 0;
         return N->height;
    }
 
    // A utility function to get maximum of two integers
    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }
    
// A utility function to right rotate subtree rooted with y
// See the diagram in the link given above - self balancing binary search tree.
//The function is not exactly the same as the one in the link as it had to be adapted to the parameters of the program.
Node<Key_T, Mapped_T> *rightRotate(Node<Key_T, Mapped_T> *y)
{
    Node<Key_T, Mapped_T> *x = y->left;
    Node<Key_T, Mapped_T> *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
 
    // Return new root
    if (y == this->root) {
      this->root = x;
    }
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram in the link given above - self balancing binary search tree.
//The function is not exactly the same as the one in the link as it had to be adapted to the parameters of the program.
Node<Key_T, Mapped_T> *leftRotate(Node<Key_T, Mapped_T> *x)
{
    Node<Key_T, Mapped_T> *y = x->right;
    Node<Key_T, Mapped_T> *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 
    // Return new root
    if (x == this->root) {
      this->root = y;
    }
    return y;
}
 
// Get Balance factor of node N
int getBalance(Node<Key_T, Mapped_T> *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

//Inserts a node into the tree
Node<Key_T, Mapped_T>* insert(Node<Key_T, Mapped_T>* node, ValueType val, Node<Key_T, Mapped_T> * prevNode, int L_or_R)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL) {
        node = new Node<Key_T, Mapped_T>(val);
        if (root == NULL) {
            root = node;
        }
        
        if (L_or_R == -1) {
            insertList(node, prevNode);
        } else if (L_or_R == 0) {
            insertList(sentinel, node);
        } else {
            insertList(prevNode, node);
        }
        return node;
    }
 
    if (val.first < node->nodeVal->first) {
        node->left  = insert(node->left, val, node, -1);
    } else if (!(val.first < node->nodeVal->first) && !(val.first == node->nodeVal->first)) {
        node->right = insert(node->right, val, node, 1);
    } else {// Equal keys not allowed
        return node;
    }

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && val.first < node->left->nodeVal->first)
        return rightRotate(node);
 
    // Right Right Case
    if (node->right != NULL) {
        if (balance < -1 && (!(val.first < node->right->nodeVal->first) && !(val.first == node->right->nodeVal->first)))
            return leftRotate(node);
    } 
    // Left Right Case
    if (node->right != NULL) {
        if (balance > 1 && (!(val.first < node->right->nodeVal->first) && !(val.first == node->right->nodeVal->first)))
        {
            node->left =  leftRotate(node->left);
            return rightRotate(node);
        }
    }
 
    // Right Left Case
    if (balance < -1 && val.first < node->right->nodeVal->first)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    /* return the (unchanged) node pointer */
   return node;
}
 
/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
 Node<Key_T, Mapped_T> * minValueNode(Node<Key_T, Mapped_T>* node)
{
    Node<Key_T, Mapped_T>* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}
 
// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
Node<Key_T, Mapped_T>* deleteNode(Node<Key_T, Mapped_T>* root, Key_T val, bool wasDeleted)  {
    // STEP 1: PERFORM STANDARD BST DELETE
    Node<Key_T, Mapped_T> * toDelete;
    if (root == NULL) {
        return root;
    }
  
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if (wasDeleted == false) {
        if ( val < root->nodeVal->first ) {
            root->left = deleteNode(root->left, val, wasDeleted);
        }
     
        // If the key to be deleted is greater than the
        // root's key, then it lies in right subtree
        else if(!(val < root->nodeVal->first) && !(val == root->nodeVal->first)) {
            root->right = deleteNode(root->right, val, wasDeleted);
        }
     
        // if key is same as root's key, then this is
        // the node to be deleted
        else
        {
            // node with only one child or no child
            if( (root->left == NULL) || (root->right == NULL) )
    	  {
                Node<Key_T, Mapped_T> *temp = root->left ? root->left :
                                                 root->right;
    	    // No child case
                if (temp == NULL) {
                    toDelete = root;
                    root = temp;
                    if (this->root == root) {
                        this->root = NULL;
                    }
                    removeList(toDelete, wasDeleted);
                    wasDeleted = true;
                    root = temp;
                } else { // One child case
                    toDelete = root;
                    removeList(toDelete, wasDeleted);
                    wasDeleted = true;
                    *root = *temp; // Copy the contents of non-empty Child
                }
            } else {
                // node with two children: Get the inorder
                // successor (smallest in the right subtree)
                Node<Key_T, Mapped_T>* temp = minValueNode(root->right);
        	    toDelete = root;
        	    ValueType *tempVal = toDelete->nodeVal;
        	    root->nodeVal = temp->nodeVal;
        	    root->height = height(root);
        	    temp->nodeVal = tempVal;
        	    temp->height = height(temp);
        	    toDelete = temp;
        	    root->right = deleteNode(root->right, temp->nodeVal->first, wasDeleted);
            }
        }
    }
    
    // If the tree had only one node then return
    if (root == NULL)
      return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
 
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
 
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    
    return root;
}
 
// A utility function to print preorder traversal of
// the tree.
// The function also prints height of every node
void preOrder(Node<Key_T, Mapped_T> *root)
{
    if(root != NULL)
    {
        preOrder(root->left);
        preOrder(root->right);
    }
}

//Prints the contents of the tree in this format:
//Key - Mapped Value - Height of the Node
void inOrder(Node<Key_T, Mapped_T> *root)
{
    if(root != NULL)
    {
        inOrder(root->left);
        printf("%d - %c - %d\n", root->nodeVal->first, root->nodeVal->second, root->height);
    	inOrder(root->right);
    }
}

//Searches the tree by key value
Node<Key_T, Mapped_T> *search(Key_T val)
{
    return search(root, val);
}

//Utility function to search the tree by value
Node<Key_T, Mapped_T> * search(Node<Key_T, Mapped_T> *r, Key_T val)
{
    bool found = false;
    while ((r != NULL) && !found)
    {
        ValueType * rval = r->nodeVal;
        if (val < rval->first)
            r = r->left;
        else if (!(val < rval->first) && !(val == rval->first))
            r = r->right;
        else
        {
            found = true;
            break;
        }
        search(r, val);
    }
    if (found == true) {
        return r;
    } else {
        return NULL;
    }
}

//Linked List code
// Inserts node n after node pos.
void insertList(Node<Key_T, Mapped_T> *pos, Node<Key_T, Mapped_T> *n) {
    //pos = root, n = sentienel
    if (pos->suc == NULL && pos->pred == NULL) {
        pos->suc = n;
        pos->pred = n->pred;
        n->pred->suc = pos;
        n->pred = pos;
    } else {
        n->suc = pos->suc;
        pos->suc->pred = n;
        pos->suc = n;
        n->pred = pos;
    }
}
    
// Removes a node from the linked list, then deletes the value of the node.
void removeList(Node<Key_T, Mapped_T> *n, bool wasDeleted) {
    if (wasDeleted == false && (n->pred != NULL && n->suc != NULL)) {
        n->pred->suc = n->suc;
        n->suc->pred = n->pred;
        n->pred = NULL;
        n->suc = NULL;
	    delete(n);
    }
}
};


template <class Key_T, class Mapped_T>
class Iterator {
    public:
    using ValueType = std::pair<const Key_T, Mapped_T>;
    Node<Key_T, Mapped_T> * thisNode;

    Iterator<Key_T, Mapped_T>(Node<Key_T, Mapped_T> * node) {
        thisNode = node;
        thisNode->nodeVal = node->nodeVal;
        thisNode->left = node->left;
        thisNode->right = node->right;
        thisNode->pred = node->pred;
        thisNode->suc = node->suc;
    }
    
    ~Iterator<Key_T, Mapped_T>() {
        thisNode = NULL;
    }
    
    //++ operators make the iterator point to the next node in the linked list.
    Iterator<Key_T, Mapped_T> &operator++() {
        thisNode = thisNode->suc;
        return *this;
    }
    Iterator<Key_T, Mapped_T> operator++(int) {
        Iterator<Key_T, Mapped_T> temp = Iterator<Key_T, Mapped_T>(thisNode);
        thisNode = thisNode->suc;
        return temp;
    }
    
    //-- operators make the iterator point to the previous node in the linked list.
    Iterator<Key_T, Mapped_T> &operator--() {
        thisNode = thisNode->pred;
        return *this;
    }
    
    Iterator<Key_T, Mapped_T> operator--(int) {
        Iterator<Key_T, Mapped_T> temp = Iterator<Key_T, Mapped_T>(thisNode);
        thisNode = thisNode->pred;
        return temp;
    }
    
    //* operator
    ValueType &operator*() const {
        return *thisNode->nodeVal;
    }
    
    //-> operator
    ValueType *operator->() const {
        return thisNode->nodeVal;
    }
};

template <class Key_T, class Mapped_T>
class ConstIterator : public Iterator<Key_T, Mapped_T> {
    public:
    using ValueType = std::pair<const Key_T, Mapped_T>;
    ConstIterator<Key_T, Mapped_T>(const Iterator<Key_T, Mapped_T> & it) : Iterator<Key_T, Mapped_T>(it) {
    }
    
    //* operator
    const ValueType &operator*() const {
        return &const_cast<class Iterator<Key_T, Mapped_T>::ValueType>(Iterator<Key_T, Mapped_T>::thisNode->nodeVal);
    }
    
    //-> operator
    const ValueType *operator->() const {
        return const_cast<class Iterator<Key_T, Mapped_T>::ValueType>(Iterator<Key_T, Mapped_T>::thisNode->nodeVal);
    }
};

template <class Key_T, class Mapped_T>
class ReverseIterator : public Iterator<Key_T, Mapped_T> {
    public:
    ReverseIterator<Key_T, Mapped_T>(const Iterator<Key_T, Mapped_T> & it) : Iterator<Key_T, Mapped_T>(it) {
    }
    
    //++ operators make the reverse iterator point to the previous node in the linked list.
    ReverseIterator<Key_T, Mapped_T> &operator++() {
        Iterator<Key_T, Mapped_T>::thisNode = Iterator<Key_T, Mapped_T>::thisNode->pred;
        return *this;
    }
    ReverseIterator<Key_T, Mapped_T> operator++(int) {
        ReverseIterator<Key_T, Mapped_T> temp = ReverseIterator<Key_T, Mapped_T>(Iterator<Key_T, Mapped_T>::thisNode);
        Iterator<Key_T, Mapped_T>::thisNode = Iterator<Key_T, Mapped_T>::thisNode->pred;
        return temp;
    }
    
    //-- operators make the iterator point to the next node in the linked list.
    ReverseIterator<Key_T, Mapped_T> &operator--() {
        Iterator<Key_T, Mapped_T>::thisNode = Iterator<Key_T, Mapped_T>::thisNode->suc;
        return *this;
    }
    
    ReverseIterator<Key_T, Mapped_T> operator--(int) {
        ReverseIterator<Key_T, Mapped_T> temp = Iterator<Key_T, Mapped_T>(Iterator<Key_T, Mapped_T>::thisNode);
        Iterator<Key_T, Mapped_T>::thisNode = Iterator<Key_T, Mapped_T>::thisNode->suc;
        return temp;
    }
};

template <class Key_T, class Mapped_T>
class Map {
    public:
    using ValueType = std::pair<const Key_T, Mapped_T>;
    friend class Tree<Key_T, Mapped_T>;
    Tree<Key_T, Mapped_T> * mapTree;
    
    //Creates a new map
    Map<Key_T, Mapped_T>() {
        mapTree = new Tree<Key_T, Mapped_T>();   
    }
    
    //Copy constructor of the map.
    Map<Key_T, Mapped_T>(const Map<Key_T, Mapped_T> & toCopy) {
        mapTree = new Tree<Key_T, Mapped_T>(*(toCopy.mapTree));
    }
    
    //Assignment operator: calls the copy constructor, and deletes the original map.
    Map &operator=(const Map<Key_T, Mapped_T> & toCopy) {
        if (this != &toCopy) {
            (*this).~Map();
            mapTree = new Tree<Key_T, Mapped_T>(*(toCopy.mapTree));
        }
        return *this;
    }
    
    //Uses an initializer list to populate a map.
    Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> il) {
        mapTree = new Tree<Key_T, Mapped_T>(); 
        for (auto * it=il.begin(); it!=il.end(); ++it) {
            insert(*it);
        }
    }
     
    //Destructor
    ~Map<Key_T, Mapped_T>() {
        Node<Key_T, Mapped_T> * toDelete = NULL;
        Node<Key_T, Mapped_T> * current = mapTree->sentinel->suc;
        while(current->isSentinel == false) {
            toDelete = current;
            current = current->suc;
            delete(toDelete);
        }
        delete(mapTree->sentinel);
        delete(mapTree);
    }
    
    //Gets the number of the nodes in the map.
    size_t size() const {
        size_t count = 0;
        Node<Key_T, Mapped_T> * current = mapTree->sentinel->suc;
        while(current != mapTree->sentinel) {
            count++;
            current = current->suc;
        }
        return count;
    }
    
    //Returns true if the map is empty, false otherwise.
    bool empty() const {
        if (mapTree->root == NULL) {
            return true;
        } else {
            return false;
        }
    }
    
    //Returns an Iterator pointing to the beginning of the map.
    Iterator<Key_T, Mapped_T> begin(){
        Iterator<Key_T, Mapped_T> first = Iterator<Key_T, Mapped_T>(mapTree->sentinel->suc);
        return first;
    }
    
    //Returns an Iterator pointing to one past the end of the map.
    Iterator<Key_T, Mapped_T> end() {
        Iterator<Key_T, Mapped_T> last = Iterator<Key_T, Mapped_T>(mapTree->sentinel);
        return last;
    }
    
    //Returns a ConstIterator pointing to the beginning of the map.
    ConstIterator<Key_T, Mapped_T> begin() const {
        ConstIterator<Key_T, Mapped_T> first = ConstIterator<Key_T, Mapped_T>(mapTree->sentinel->suc);
        return first;
    }
    
    //Returns a ConstIterator pointing to one past the end of the map.
    ConstIterator<Key_T, Mapped_T> end() const {
        ConstIterator<Key_T, Mapped_T> last = ConstIterator<Key_T, Mapped_T>(mapTree->sentinel);
        return last;
    }
    
    //Returns a ReverseIterator pointing to one past the end of the map.
    ReverseIterator<Key_T, Mapped_T> rbegin() {
        ReverseIterator<Key_T, Mapped_T> first = ReverseIterator<Key_T, Mapped_T>(mapTree->sentinel->pred);
        return first;
    }
    
    //Returns a ReverseIterator pointing to one past the end of the map.
    ReverseIterator<Key_T, Mapped_T> rend() {
        ReverseIterator<Key_T, Mapped_T> last = ReverseIterator<Key_T, Mapped_T>(mapTree->sentinel);
        return last;
    }
    
    //Finds the value associated the given key and returns an Iterator pointing to it.
    //Returns an Iterator pointing to the end of the list if it's not found.
    Iterator<Key_T, Mapped_T> find(const Key_T & val) {
        Iterator<Key_T, Mapped_T> current = begin();
        while(current.thisNode->isSentinel == false) {
            if (current.thisNode->nodeVal->first == val) {
                return current;
            }
            current++;
        }
        return end();
    }
    
    //Finds the value associated the given key and returns a ConstIterator pointing to it.
    //Returns a ConstIterator pointing to the end of the list if it's not found.
    ConstIterator<Key_T, Mapped_T> find(const Key_T & val) const {
        ConstIterator<Key_T, Mapped_T> current = begin();
        while(current.thisNode->isSentinel == false) {
            if (current.thisNode->nodeVal->first == val) {
                return current;
            }
            current++;
        }
        return end();
    }
    
    //Finds a key-value pair and returns the node associated with it.
    //Returns NULL it's not found.
    Node<Key_T, Mapped_T> * find(ValueType val) {
        Node<Key_T, Mapped_T> * current = mapTree->sentinel->suc;
        while(current->isSentinel == false) {
            if ((current->nodeVal->first == val.first) && (current->nodeVal->second == val.second)) {
                return current;
            }
            current = current->suc;
        }
        return NULL;
    }
    
    //Returns the value associated with a given key.
    //If it's not found an exception is thrown.
    Mapped_T &at(const Key_T & val) {
        Node<Key_T, Mapped_T> * found = mapTree->search(val);
        if (found != NULL) {
            return found->nodeVal->second;
        } else {
            throw std::out_of_range("Key not in map\n");
        }
    }
    
    //Returns a const value associated with a given key.
    //If it's not found an exception is thrown.
    const Mapped_T &at(const Key_T & val) const {
        Node<Key_T, Mapped_T> * found = mapTree->search(val);
        if (found != NULL) {
            return found->nodeVal->second;
        } else {
            throw std::out_of_range("Key not in map\n");
            //To Do: Look up how to throw std::out of range
        }
    }
    
    //[] operator. Returns the value associated with the key val.
    //If it does not exist, inserts the value, then returns the value.
    Mapped_T &operator[](const Key_T & val) {
        Iterator<Key_T, Mapped_T> it = find(val);
        Mapped_T temp = Mapped_T();
        if (it == end()) {
            ValueType defaultVal = std::pair<Key_T, Mapped_T>(val, temp);
            std::pair<Iterator<Key_T, Mapped_T>, bool> tester = insert(defaultVal);
            return tester.first->second;
        } else {
            return it.thisNode->nodeVal->second;
        }
    }
    
    //Gets an iterator associated with a given node.
    Iterator<Key_T, Mapped_T> get(Node<Key_T, Mapped_T> * n) {
        Iterator<Key_T, Mapped_T> current = Iterator<Key_T, Mapped_T>(n);
        return current;
    }
    
    //Dereferences any given iterator.
    Node<Key_T, Mapped_T> deref(Iterator<Key_T, Mapped_T> it) {
        if (it == end()) {
            return NULL;
        } else {
            return it.thisNode;
        }
    }
    
    //Inserts a key-value pair into the map.
    //What is returned - a pair
    //First part: Iterator pointing to the node with the value that was attempted to be added
    //Second part: A boolean value indicating if the value was added to the map or not.
    //If the value already exists, nothing is added, and the second value of the pair is false.
    std::pair<Iterator<Key_T, Mapped_T>, bool> insert(const ValueType & val) {
        Node<Key_T, Mapped_T> * current = mapTree->insert(mapTree->root, val, mapTree->root, 0);
        mapTree->root = current;
        Node<Key_T, Mapped_T> * found = find(val);
        bool isNew;
        if(found->isNew == true) {
            found->isNew = false;
            isNew = true;
        } else {
            isNew = false;
        }
        return std::make_pair(get(found), isNew);
    }
    
    //Inserts a group of values into the map.
    template <typename IT_T>
    void insert(IT_T range_beg, IT_T range_end) {
        IT_T current = range_beg;
        while (current != range_end) {
            insert(*range_beg);
        }
    }
    
    //Given an iterator pointing to position pos, erases the node associated with that iterator.
    //Do not change the 'false' value that is the third parameter of deleteNode.
    void erase(Iterator<Key_T,Mapped_T> pos) {
        if (pos.thisNode != mapTree->sentinel) {
            Mapped_T found = at(pos.thisNode->nodeVal->first);
            mapTree->deleteNode(mapTree->root, pos.thisNode->nodeVal->first, false);
        } else {
            throw std::out_of_range("Tried to delete sentinel\n");
        }
    }
    
    //Given a key, traverse the map and delete the node associated with that key.
    //Do not change the 'false' value that is the third parameter of deleteNode.
    void erase(const Key_T & val) {
        Mapped_T found = at(val);
        mapTree->deleteNode(mapTree->root, val, false);
    }
    
    //Prints the contents of the map using in order traversal.
    void inOrder() {
        mapTree->inOrder(mapTree->root);
    }
};

//== operator checks to see if two maps are equal by iterating through the contents of both maps.
template <class Key_T, class Mapped_T>
bool operator==(const Map<Key_T, Mapped_T> & m1, const Map<Key_T, Mapped_T> & m2) {
    Node<Key_T, Mapped_T> * n1 = m1.mapTree->sentinel->suc;
    Node<Key_T, Mapped_T> * n2 = m2.mapTree->sentinel->suc;
    while (!(n1 == m1.mapTree->sentinel) && !(n2 == m2.mapTree->sentinel)) {
        if (!(n1->nodeVal->first == n2->nodeVal->first)) {
            return false;
        } else if (!(n1->nodeVal->second == n2->nodeVal->second)) {
            return false;
        }
        n1 = n1->suc;
        n2 = n2->suc;
    }
    if ((n1 == m1.mapTree->sentinel) && (n2 == m2.mapTree->sentinel)) {
        return true;
    } else {
        return false;
    }
}

//!= checks to see if two maps are unequal.
template <class Key_T, class Mapped_T>
bool operator!=(const Map<Key_T, Mapped_T> & m1, const Map<Key_T, Mapped_T> & m2) {
    return !(m1 == m2);
}

//< operator checks to see if one map is less than another.
//Checks to see if one map has a key less than another.
//Compares Keys only
template <class Key_T, class Mapped_T>
bool operator<(const Map<Key_T, Mapped_T> & m1, const Map<Key_T, Mapped_T> & m2) {
    Node<Key_T, Mapped_T> * n1 = m1.mapTree->sentinel->suc;
    Node<Key_T, Mapped_T> * n2 = m2.mapTree->sentinel->suc;
    while ((n1 != m1.mapTree->sentinel) && (n2 != m2.mapTree->sentinel)) {
        if (n1->nodeVal->first < n2->nodeVal->first) {
            return true;
        }
        n1 = n1->suc;
        n2 = n2->suc;
    }
    if ((n1 == m1.mapTree->sentinel) && !(n2 == m2.mapTree->sentinel)) {
        return true;
    } else {
        return false;
    }
}

//== operator compares two iterators to see if they are pointing to the same node.
template <class Key_T, class Mapped_T>
bool operator==(const Iterator<Key_T, Mapped_T> & it1, const Iterator<Key_T, Mapped_T> & it2) {
    if (it1.thisNode == it2.thisNode) {
        return true;
    } else {
        return false;
    }
}


//!= operator compares two iterators to see if they are pointing to different nodes.
template <class Key_T, class Mapped_T>
bool operator!=(const Iterator<Key_T, Mapped_T> & it1, const Iterator<Key_T, Mapped_T> & it2) {
    return !(it1 == it2);
}
}
