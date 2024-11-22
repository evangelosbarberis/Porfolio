/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA8
* Dictionary.cpp
* creates Dictionary adt, implementation used functions from Dict.h
***/

#include "Dictionary.h"
#include <stdexcept> 
#include <sstream>   
using namespace std;


// helper functiions for pa7
void Dictionary::inOrderString(std::string& s, Node* R) const {
    // Generate an in-order traversal string representation of the tree
    if (R != nil) {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}
void Dictionary::preOrderString(string& s, Node* R) const {
    // Generate a pre-order traversal string representation of the tree
    if (R != nil) {
        s += R->key;
        if (R->color == 0) { // If the node's color is red
            s += " (RED)";
        }
        s += "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}
void Dictionary::preOrderCopy(Node* R, Node* N) {
    // Copy each node from R to this dictionary excluding N
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}
void Dictionary::postOrderDelete(Node* R) {
    // Deallocate all nodes from R downwards
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}
Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    // Locate a node with key k
    if (R == nil || k == R->key) {
        return R;
    } else if (k < R->key) {
        return search(R->left, k);
    } else {
        return search(R->right, k);
    }
}
Dictionary::Node* Dictionary::findMin(Node* R) {
    // Find the node with the smallest key
    while (R->left != nil) {
        R = R->left;
    }
    return R;
}
Dictionary::Node* Dictionary::findMax(Node* R) {
    // Find the node with the largest key
    while (R->right != nil) {
        R = R->right;
    }
    return R;
}
Dictionary::Node* Dictionary::findNext(Node* N) {
    // Find the successor of N
    if (N->right != nil) return findMin(N->right);
    Node* P = N->parent;
    while (P != nil && N == P->right) {
        N = P;
        P = P->parent;
    }
    return P;
}
Dictionary::Node* Dictionary::findPrev(Node* N) {
    // Find the predecessor of N
    if (N->left != nil) return findMax(N->left);
    Node* P = N->parent;
    while (P != nil && N == P->left) {
        N = P;
        P = P->parent;
    }
    return P;
}
   // RBT Helper Functions (Optional) -----------------------------------------
   // LeftRotate()
void Dictionary::LeftRotate(Node* x) {
    Node* y = x->right; // set y
    x->right = y->left; // turn y's left subtree into x's right subtree
    if (y->left != nil) {
        y->left->parent = x;
    }
    y->parent = x->parent; // link x's parent to y
    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x; // put x on y's left
    x->parent = y;
}
   // RightRotate()
void Dictionary::RightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}
   // RB_InsertFixUP()
void Dictionary::RB_InsertFixUp(Node* z) {
    while (z->parent->color == 0) { // While parent is red
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right; // Uncle
            if (y->color == 0) { // Case 1: Uncle is red
                z->parent->color = 1; // Make parent black
                y->color = 1; // Make uncle black
                z->parent->parent->color = 0; // Make grandparent red
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) { // Case 2: z is right child
                    z = z->parent; // Move z up
                    LeftRotate(z);
                }
                // Case 3: z is left child
                z->parent->color = 1;
                z->parent->parent->color = 0;
                RightRotate(z->parent->parent);
            }
        } else {
            // Same as then clause with "right" and "left" exchanged
            Node* y = z->parent->parent->left; // Uncle
            if (y->color == 0) { // Case 1: Uncle is red
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                LeftRotate(z->parent->parent);
            }
        }
    }
    root->color = 1; // Ensure root is always black
}
   // RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}
// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* N) {
    Node* w;
    while (N != root && N->color == 1) {
        if (N == N->parent->left) {
            w = N->parent->right;
            if (w->color == 0) {
                w->color = 1;
                N->parent->color = 0;
                LeftRotate(N->parent);
                w = N->parent->right;
            }
            if (w->left->color == 1 && w->right->color == 1) {
                w->color = 0;
                N = N->parent;
            } else {
                if (w->right->color == 1) {
                    w->left->color = 1;
                    w->color = 0;
                    RightRotate(w);
                    w = N->parent->right;
                }
                w->color = N->parent->color;
                N->parent->color = 1;
                w->right->color = 1;
                LeftRotate(N->parent);
                N = root;
            }
        } else {
            w = N->parent->left;
            if (w->color == 0) {
                w->color = 1;
                N->parent->color = 0;
                RightRotate(N->parent);
                w = N->parent->left;
            }
            if (w->right->color == 1 && w->left->color == 1) {
                w->color = 0;
                N = N->parent;
            } else {
                if (w->left->color == 1) {
                    w->right->color = 1;
                    w->color = 0;
                    LeftRotate(w);
                    w = N->parent->left;
                }
                w->color = N->parent->color;
                N->parent->color = 1;
                w->left->color = 1;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = 1;
}
// RB_Delete()
void Dictionary::RB_Delete(Node* N) {
    Node* x;
    Node* y = N;
    int y_original_color = y->color;
    if (N->left == nil) {
        x = N->right;
        RB_Transplant(N, N->right);
    } else if (N->right == nil) {
        x = N->left;
        RB_Transplant(N, N->left);
    } else {
        y = findMin(N->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == N) {
            x->parent = y;
        } else {
            RB_Transplant(y, y->right);
            y->right = N->right;
            y->right->parent = y;
        }
        RB_Transplant(N, y);
        y->left = N->left;
        y->left->parent = y;
        y->color = N->color;
    }
    if (y_original_color == 1) {
        RB_DeleteFixUp(x);
    }
}
// Node Constructors
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = 1; // Initialize nodes as black
}
// Class Constructors & Destructors
Dictionary::Dictionary() {
    // Initialize nil node with empty key and value
    nil = new Node("", 0);
    nil->color = 1; // nil nodes are black
    // Set root and current to nil
    root = nil;
    current = nil;
    // Initialize number of pairs to 0
    num_pairs = 0;
}
Dictionary::Dictionary(const Dictionary& D) {
    // Create nil node with empty key and value
    nil = new Node("", 0);
    nil->color = 1; // nil nodes are black
    // Set root and current to nil
    root = nil;
    current = nil;
    // Initialize number of pairs to 0
    num_pairs = 0;
    // Copy elements from D to this dictionary
    preOrderCopy(D.root, D.nil);
}
Dictionary::~Dictionary() {
    // Delete nodes in the dictionary
    postOrderDelete(root);
    // Delete nil node
    delete nil;
}
// Access Functions 
int Dictionary::size() const {
    // Return the number of key-value pairs in the dictionary
    return num_pairs;
}
bool Dictionary::contains(keyType k) const {
    // Check if the dictionary contains a given key
    return search(root, k) != nil;
}
valType& Dictionary::getValue(keyType k) const {
    // Get the value associated with a given key
    Node* N = search(root, k);
    if (N == nil) throw std::logic_error("key not found");
    return N->val;
}
bool Dictionary::hasCurrent() const {
    // Check if there is a current node set
    return current != nil;
}
keyType Dictionary::currentKey() const {
    // Get the key of the current node
    if (!hasCurrent()) {
        throw std::logic_error("undefined");
    }
    return current->key;
}
valType& Dictionary::currentVal() const {
    // Get the value of the current node
    if (!hasCurrent()) {
        throw std::logic_error("undefined");
    }
    return current->val;
}
// Manipulation Procedures
void Dictionary::clear() {
    // Clear the dictionary by deleting all nodes and resetting attributes
    postOrderDelete(root); // Delete nodes using postorder traversal
    root = nil; // Reset root to nil
    current = nil; // Reset current to nil
    num_pairs = 0; // Reset number of pairs to 0
}
void Dictionary::setValue(keyType k, valType v) {
    Node* z = new Node(k, v); // Create new node with key and value
    Node* y = nil; // Parent of added node
    Node* x = root;
    while (x != nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else if (z->key > x->key) {
            x = x->right;
        } else { // If key already exists, update the value and exit
            x->val = v;
            delete z;
            return;
        }
    }
    z->parent = y;
    if (y == nil) { // The tree was empty
        root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = 0; // New nodes are always red
    RB_InsertFixUp(z); // Fix up the modified tree
    num_pairs++;
}
void Dictionary::remove(keyType key) {
    // Remove the node from the dictionary
    Node* Z = search(root, key); // Find the node to remove
    if (Z == nil) return; // Key not found, return
    if (current == Z) { // Invalidate current if it points to node being removed
        current = nil;
    }
    Node* Y = Z;   // Node to be removed
    Node* X = nil; // Node that will replace Y
    int Y_original_color = Y->color; // Original color of Y
    if (Z->left == nil) {
        // Z has no left child
        X = Z->right;
        RB_Transplant(Z, Z->right);
    } else if (Z->right == nil) {
        // Z has no right child
        X = Z->left;
        RB_Transplant(Z, Z->left);
    } else {
        // Z has two children
        Y = findMin(Z->right); // Successor of Z
        Y_original_color = Y->color;
        X = Y->right;
        if (Y->parent == Z) {
            X->parent = Y;
        } else {
            RB_Transplant(Y, Y->right);
            Y->right = Z->right;
            Y->right->parent = Y;
        }
        RB_Transplant(Z, Y);
        Y->left = Z->left;
        Y->left->parent = Y;
        Y->color = Z->color;
    }
    
    delete Z; // Free the node
    num_pairs--; // Decrement number of key-value pairs
    
    if (Y_original_color == 1) {
        // Fix up the tree
        RB_DeleteFixUp(X);
    }
}





// Iterator Functions


void Dictionary::begin() {
    // Set the iterator to the smallest key
    if (root != nil) {
        current = findMin(root);
    }
}

void Dictionary::end() {
    // Set the iterator to the largest key
    if (root != nil) {
        current = findMax(root);
    }
}

void Dictionary::next() {
    // Move the iterator to the next key
    if (!hasCurrent()) {
    }
    Node* N = findNext(current);
    current = (N != nil) ? N : nil;
}

void Dictionary::prev() {
    // Move the iterator to the previous key
    if (!hasCurrent()) {
    }
    Node* N = findPrev(current);
    current = (N != nil) ? N : nil;
}


// Other Functions


string Dictionary::to_string() const {
    // Convert the dictionary to a string using inorder traversal
    string str;
    inOrderString(str, root); // Traverse the tree and append keys and values to the string
    return str; // Return the resulting string
}

string Dictionary::pre_string() const {
    // Convert the dictionary to a string using preorder traversal
    string str;
    preOrderString(str, root); // Traverse the tree and append keys and values to the string
    return str; // Return the resulting string
}

bool Dictionary::equals(const Dictionary& D) const {
    // Check if two dictionaries are equal
    if (size() != D.size()) return false; // If sizes are different, dictionaries are not equal
    return to_string() == D.to_string(); // Compare the string representations of dictionaries
}


// Overloaded Operators

ostream& operator<<(ostream& stream, Dictionary& D) {
    // Overloaded stream insertion operator to output dictionary as string
    stream << D.to_string(); // Output the dictionary as string
    return stream; // Return the output stream
}

bool operator==(const Dictionary& A, const Dictionary& B) {
    // Overloaded equality operator to compare dictionaries
    return A.equals(B); // Check if dictionaries are equal
}

Dictionary& Dictionary::operator=(const Dictionary& D) {
    // Overloaded assignment operator to copy dictionary
    if (this != &D) { // Check for self-assignment
        clear(); // Clear current dictionary
        preOrderCopy(D.root, D.nil); // Copy elements from D to this dictionary
    }
    return *this; // Return the updated dictionary
}
