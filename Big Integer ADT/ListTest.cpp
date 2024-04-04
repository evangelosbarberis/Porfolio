/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA6
* ListTest.cpp
* acuratly testst functions from List.cpp file
* comes almost directly from handout
***/

#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

using namespace std;

int main() {
    int i, n = 10;
    List A, B, C, D;
    
    // Populate Lists A, B, C, and D with different patterns
    for(i = 1; i <= n; i++) {
        A.insertAfter(i);
        B.insertAfter(11-i);
        C.insertBefore(i);
        D.insertBefore(11-i);
    }

    // Display contents and positions of Lists A, B, C, and D
    cout << endl;
    cout << "A = " << A << endl << "A.position() = " << A.position() << endl;
    cout << "B = " << B << endl << "B.position() = " << B.position() << endl;
    cout << "C = " << C << endl << "C.position() = " << C.position() << endl;
    cout << "D = " << D << endl << "D.position() = " << D.position() << endl;
    cout << endl;

    // Move cursor positions
    A.moveBack();
    B.moveBack();
    C.moveFront();
    D.moveFront();

    // Display Lists and positions after moving cursor positions
    cout << "A = " << A << endl << "A.position() = " << A.position() << endl;
    cout << "B = " << B << endl << "B.position() = " << B.position() << endl;
    cout << "C = " << C << endl << "C.position() = " << C.position() << endl;
    cout << "D = " << D << endl << "D.position() = " << D.position() << endl;

    cout << endl << "B: ";
    for(i = 1; i <= 7; i++) {
        cout << B.movePrev() << " ";
    }
    cout << endl << "B.position() = " << B.position() << endl;

    cout << endl << "C: ";
    for(i = 1; i <= 7; i++) {
        cout << C.moveNext() << " ";
    }
    cout << endl << "C.position() = " << C.position() << endl;

    cout << endl;
    cout << "A==B is " << (A==B ? "true" : "false") << endl;
    cout << "B==C is " << (B==C ? "true" : "false") << endl;
    cout << "C==D is " << (C==D ? "true" : "false") << endl;
    cout << "D==A is " << (D==A ? "true" : "false") << endl;
    cout << endl;

    // Testing findNext, findPrev, eraseBefore, and eraseAfter operations
    cout << B.findNext(5) << endl;
    cout << B << endl;
    B.eraseBefore();
    B.eraseAfter();
    cout << B << endl;
    cout << B.position() << endl; 
    cout << B.findPrev(2) << endl;
    B.eraseBefore();
    B.eraseAfter();
    cout << B << endl;
    cout << B.position() << endl; 
    cout << B.findNext(20) << endl; 
    cout << B.position() << endl;
    cout << B.findPrev(20) << endl;
    cout << B.position() << endl;
    cout << endl;

    // Testing insertAfter along with other operations
    for(i = 10; i >= 1; i--) {
        A.insertAfter(i);
        A.movePrev();
    }
    cout << "A = " << A << endl;
    for(i = 1; i <= 15; i++) {
        A.moveNext();
    }
    cout << A.position() << endl;
    A.cleanup();
    cout << "A = " << A << endl;
    cout << A.position() << endl;
    cout << endl;

    // Testing copy constructor and equals function
    List E = A;
    cout << "E==A is " << (E==A ? "true" : "false") << endl;

    cout << endl;

    // Testing concat function
    List F = A.concat(B);
    cout << "F = " << F << endl;
    cout << "length: " << F.length() << endl;
    cout << "front: " << F.front() << endl;
    cout << "back: " << F.back() << endl;
    cout << "position: " << F.position() << endl;
    for(i = 1; i <= 7; i++) {
        cout << "peekNext: " << F.peekNext() << endl;
        F.insertBefore(-i);
        cout << F.moveNext() << endl; 
    }
    cout << "F = " << F << endl;

    cout << endl;

    // Testing exceptions
    cout << "test exceptions: " << endl;
    cout << "A = " << A << endl;
    A.moveFront();
    try { A.peekPrev(); } 
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.movePrev(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.setBefore(5); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.eraseBefore(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    A.moveBack();
    try { A.peekNext(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.moveNext(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.setAfter(5); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.eraseAfter(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    A.clear();
    try { A.front(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }
    try { A.back(); }
    catch(std::runtime_error& e) { cout << e.what() << endl; cout << "   continuing without interruption" << endl; }

    cout << endl;
    return 0;
}
