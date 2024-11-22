/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA8
* WordFrequency.cpp
* Create WordFrequency here, counts frequency of words, reads user in as well
***/

#include "Dictionary.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return 1;
    }

    ifstream input(argv[1]);
    ofstream output(argv[2]);
    if (!input.is_open() || !output.is_open()) {
        cerr << "Could not open file(s)." << endl;
        return 1;
    }

    Dictionary dict;
    string line, word;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";

    while (getline(input, line)) {
        size_t pos = 0, end;
        transform(line.begin(), line.end(), line.begin(), ::tolower); // Convert line to lower case
        while ((end = line.find_first_of(delim, pos)) != string::npos) {
            if (end != pos) {
                word = line.substr(pos, end - pos);
                if (dict.contains(word)) {
                    dict.setValue(word, dict.getValue(word) + 1); // Increment word frequency
                } else {
                    dict.setValue(word, 1); // Insert new word with frequency 1
                }
            }
            pos = line.find_first_not_of(delim, end);
        }
        if (pos < line.length()) { // Handle last word
            word = line.substr(pos);
            if (dict.contains(word)) {
                dict.setValue(word, dict.getValue(word) + 1);
            } else {
                dict.setValue(word, 1);
            }
        }
    }

    output << dict.to_string(); // Output the dictionary contents

    input.close();
    output.close();

    return 0;
}