/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA8
* Order.cpp
* reads inputed strings and inputs strings into dictionary keys = alphabetical order and pre-ordered tree walk
***/


#include "Dictionary.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    ifstream input(argv[1]);
    if (!input.is_open()) {
        return 1;
    }

    ofstream output(argv[2]);
    if (!output.is_open()) {
        return 1;
    }

    Dictionary dict;
    string line;
    int lineNumber = 1;

    // Read each line and insert into the dictionary
    while (getline(input, line)) {
        dict.setValue(line, lineNumber++);
    }

    // Write the key-value pairs in alphabetical order
    output << dict.to_string();

    // Write a blank line to separate the two representations
    output << "\n";

    // Write the keys in the order obtained from a pre-order tree walk
    output << dict.pre_string();

    // Close the input and output files
    input.close();
    output.close();

    return 0;
}