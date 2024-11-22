/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA6
* Arithmetic.cpp
* main file that creates the client view and produces in and out files
***/

#include "BigInteger.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error: Cannot open file.\n";
        return EXIT_FAILURE;
    }

    std::string lineA, lineB;

    // Read BigInteger A
    std::getline(inputFile, lineA);
    BigInteger A(lineA);

    // Skip blank line
    std::getline(inputFile, lineB);

    // Read BigInteger B
    std::getline(inputFile, lineB);
    BigInteger B(lineB);

    // Perform arithmetic operations
    BigInteger sum = A + B;
    BigInteger diff = A - B;
    BigInteger AminusA = A - A;
    BigInteger threeAminusTwoB = 3 * A - 2 * B;
    BigInteger product = A * B;
    BigInteger Asquared = A * A;
    BigInteger Bsquared = B * B;
    BigInteger complexOp = 9 * (A * A * A * A) + 16 * (B * B * B * B * B);

    // Write results to output file with blank lines in between
    outputFile << A << "\n\n" << B << "\n\n" << sum << "\n\n" << diff << "\n\n";
    outputFile << AminusA << "\n\n" << threeAminusTwoB << "\n\n" << product << "\n\n";
    outputFile << Asquared << "\n\n" << Bsquared << "\n\n" << complexOp << "\n\n";

    inputFile.close();
    outputFile.close();

    return 0;
}