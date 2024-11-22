/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA6
* List.cpp
* creates the List ADT in C++
* Main ADT to create the relevant fuctions for Shuffle.cpp
***/

#include "List.h"
using namespace std;

//-----------------------------------------------------------------------------
// Node Constructor
//-----------------------------------------------------------------------------
// intialze node elements
List::Node::Node(ListElement x){
   data = x;
   next = nullptr;
   prev = nullptr;
}

//-----------------------------------------------------------------------------
// Class Constructors & Destructors
//-----------------------------------------------------------------------------
// intalize list elements
List::List() {
   frontDummy = new Node(0);
   backDummy = new Node(0);
   frontDummy->next = backDummy;
   backDummy->prev = frontDummy;
   beforeCursor = frontDummy;
   afterCursor = backDummy;
   num_elements = 0;
   pos_cursor = 0;
}

// copy list contsructor
List::List(const List& L) { 
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    Node *N = L.frontDummy->next;
    while (N != L.backDummy) {
        this->insertAfter(N->data);
        moveNext();
        N = N->next;
    }
    moveFront();
}
// destructor
List::~List() {
   clear();
   delete frontDummy;
   delete backDummy;
}

//-----------------------------------------------------------------------------
// Access functions
//-----------------------------------------------------------------------------
// return length of list
int List::length() const{
   return num_elements;
}

// get front of list
ListElement List::front() const{
   if(length() <= 0){
   }
   return frontDummy->next->data;
}

// Return back of list
ListElement List::back() const{
   if(length() <= 0){
   }
   return backDummy->prev->data;
}

// return the curr position 
int List::position() const{
   return pos_cursor;
}

ListElement List::peekNext() const{
   if(position() >= length()){
       throw std::out_of_range("peekNext position >= length");
   }
   return afterCursor->data;
}
ListElement List::peekPrev() const{
   if(position() <= 0){
       throw std::out_of_range("peekPrev position <= 0");
   }
   return beforeCursor->data;
}


//-----------------------------------------------------------------------------
// Manipulation procedures
//-----------------------------------------------------------------------------
// clear list
void List::clear(){
   moveFront();
   // iterate through while clearing eleent after cursor
   while(length() > 0){
      eraseAfter();
   }
}

// set cursors to front of lust
void List::moveFront() {
   beforeCursor = frontDummy;
   afterCursor = frontDummy->next;
   pos_cursor = 0;
}

// set cursors for moveback
void List::moveBack(){
   pos_cursor = num_elements;
   beforeCursor = backDummy->prev;
   afterCursor = backDummy;
}

// move cursor to next element
ListElement List::moveNext(){
   if(position() >= length()){
   }
   // move before to next, and add position of cursor
   beforeCursor = afterCursor;
   afterCursor = afterCursor->next;
   pos_cursor++;
   return beforeCursor->data;
}

// move cursor to previous position
ListElement List::movePrev(){
   if(pos_cursor <= 0){
   }
   // set after to prev and vice cera, and move cursor position down 1
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;
    pos_cursor--;
    return afterCursor->data; // Return the data element that was passed over
}

// insert element after 
void List::insertAfter(ListElement x){
   // inster new node after the cursor
    Node* newNext = new List::Node(x);
    newNext->next = afterCursor;
    newNext->prev = beforeCursor;
    afterCursor->prev = newNext;
    beforeCursor->next = newNext;
    afterCursor = newNext;
    num_elements++; // update elements

}

// insert before 
void List::insertBefore(ListElement x){
    Node* newPrev = new List::Node(x);
    // insert new node before the cursor
    newPrev->next = afterCursor;
    newPrev->prev = beforeCursor;
    afterCursor->prev = newPrev;
    beforeCursor->next = newPrev;
    beforeCursor = newPrev;
    num_elements++;
    pos_cursor++; // update the pos

}

//set data element after the cursor
void List::setAfter(ListElement x){
   if(position() >= length()){
   }
   afterCursor->data = x;
}

// set data element before the cursor
void List::setBefore(ListElement x){
   if(position() <= 0){
   }
   beforeCursor->data = x;
}

// eraseAfter the cursor
void List::eraseAfter() {
   if (pos_cursor >= num_elements) { 
   }

   // Proceed with node deletion
    beforeCursor->next = afterCursor->next;
    afterCursor->next->prev = beforeCursor;
    Node *temp = afterCursor;
    delete(temp);
    afterCursor = beforeCursor->next;
    num_elements--;

}

// erase before cursor
void List::eraseBefore(){
   if(position() <= 0){
   }
   Node* N = beforeCursor;
   //same logic as eraseAfter just inversed
   N->prev->next = afterCursor;
   afterCursor->prev = N->prev;
   delete N;
   beforeCursor = afterCursor->prev;
   num_elements--;
   pos_cursor--;
}

//-----------------------------------------------------------------------------
// Other Functions
//-----------------------------------------------------------------------------

// find the next elemenr
int List::findNext(ListElement x){
   Node* N = afterCursor; // Start searching from the element after cursor
   int currentPosition = pos_cursor + 1; // Adjust for the starting position
   while(N != backDummy){
      if(N->data == x){
         beforeCursor = N;
         afterCursor = N->next;
         pos_cursor = currentPosition; // Correctly update pos_cursor
         return pos_cursor;
      }
      N = N->next;
      currentPosition++;
   }
   moveBack();
   return -1;
}

// find the prev element
int List::findPrev(ListElement x) {
    Node* N = beforeCursor; // Start searching from the element before cursor
    int currentPosition = pos_cursor - 1; // Adjust for the starting position
    while (N != frontDummy) { // Iterate until the front dummy node is reached
        if (N->data == x) { // If the element is found
            afterCursor = N; // Update afterCursor to the found element
            beforeCursor = N->prev; // Update beforeCursor to the previous element
            pos_cursor = currentPosition; // Correctly update pos_cursor
            return pos_cursor; // Return the position of the found element
        }
        N = N->prev; // Move to the previous node
        currentPosition--; // Decrement the position
    }
    moveFront(); // Move cursor to the front
    return -1; // Element not found
}


// removes duplicates 
void List::cleanup() {
    Node* current = frontDummy->next; // Start from the first element
    while (current != backDummy) { // Proceed till the end
        Node* runner = current->next; // Runner to check duplicates ahead
        while (runner != backDummy) { // Proceed till the end
            if (runner->data == current->data) {
                // Duplicate found. Remove the runner node.
                Node* toDelete = runner;
                runner = runner->next; // Move runner ahead before deleting the current node
                
                bool adjustPosCursor = false;
                if (beforeCursor == toDelete || afterCursor == toDelete) {
                    adjustPosCursor = true;
                }

                toDelete->prev->next = toDelete->next;
                toDelete->next->prev = toDelete->prev;

                if (adjustPosCursor) {
                    if (beforeCursor == toDelete) {
                        beforeCursor = toDelete->prev;
                    } else if (afterCursor == toDelete) {
                        afterCursor = toDelete->next;
                    }
                    pos_cursor = 0; // Reset cursor position for correct relocation after cleanup
                    Node* relocate = frontDummy->next;
                    while (relocate != afterCursor && relocate != backDummy) {
                        pos_cursor++;
                        relocate = relocate->next;
                    }
                }

                delete toDelete;
                num_elements--;
            } else {
                runner = runner->next; // Move runner to the next node if no deletion happens
            }
        }
        current = current->next; // Move to the next node for the next round of checks
    }
}


// concatenates two lists and returns a new list
List List::concat(const List& L) const {
    List result; // Use a new List object
    Node* temp = this->frontDummy->next;
    // iterate all elements in a list
    while (temp != this->backDummy) {
        result.insertAfter(temp->data); // insert into result list
        temp = temp->next;
    }
    temp = L.frontDummy->next;
    while (temp != L.backDummy) { // iterate through elements in List L
        result.insertAfter(temp->data); // Insert the current into result list
        temp = temp->next;
    }
    return result; // Return by value
}


// comvert list to string
std::string List::to_string() const{
   Node* N = nullptr;
   std::string s = "(";
   // iterate through list
   for(N = frontDummy->next; N != backDummy; N = N->next){
       if (N != frontDummy->next){
           s += ", ";
       }
       s += std::to_string(N->data); // append each element to the string
   }
   s += ")";
   return s; // return the string rep
}

// sees if lists are equal
bool List::equals(const List& R) const{
   bool eq = false;
   Node* N = nullptr;
   Node* M = nullptr;

   eq = ( this->num_elements == R.num_elements ); // check if elements equal
   N = this->frontDummy->next;
   M = R.frontDummy->next;
   while( eq && N != this->backDummy){ // iterate through both
       eq = (N->data == M->data); // check if elements are equal
       N = N->next;
       M = M->next;
   }
   return eq; // return fale otherwise
}

//-----------------------------------------------------------------------------
// Overridden Operators
//-----------------------------------------------------------------------------
// insertion opererator
std::ostream& operator<<( std::ostream& stream, const List& L ){
   return stream << L.to_string();
}
// equality operator
bool operator==( const List& A, const List& B ){
   return A.equals(B); // check if lists are equal
}

// assignment operator
List& List::operator=(const List& L) {
    if (this != &L) { // Avoid self-assignment
        clear(); // Clear existing elements
        
        Node* temp = L.frontDummy->next;
        while(temp != L.backDummy) {
            this->insertBefore(temp->data); // Duplicate the elements of L
            temp = temp->next;
        }
        
        // cursor to mimic L's cursor
        this->pos_cursor = L.pos_cursor;
        this->moveFront();
        for(int i = 0; i < L.pos_cursor; i++) {
            this->moveNext();
        }
    }
    return *this; // return the modified list
}