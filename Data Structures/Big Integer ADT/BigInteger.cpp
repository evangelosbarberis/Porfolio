/*
 * Evangelos Neo barberis
 * ebarberi
 * 2024 Winter CSE101 PA6
 * BigInteger.cpp
 * uses List ADT to create a BigInteger ADT
 */

#include "BigInteger.h"
#include <cctype>
#include <cmath>
using namespace std;
const ListElement base = 1000000000;
const int p = 9;
// Default constructor
BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}
// Constructor with long integer
BigInteger::BigInteger(long value) {
    // Handle zero case explicitly
    if (value == 0) {
        signum = 0;
        digits.insertAfter(0); // BigInteger representing 0
        return; // Early return
    }
    // Determine the sign of BigInteger and make value positive
    signum = (value < 0) ? -1 : 1;
    value = abs(value); // Ensure value is positive
    // Decompose value into its base components
    while (value > 0) {
        digits.insertBefore(value % base); // Insert least significant digit
        value /= base; // Reduce value by base
    }
}
// Constructor with string
BigInteger::BigInteger(std::string s) {
    if (s.empty()) {
        signum = 0;
        digits.insertAfter(0); // Handle empty string by assigning zero
        return;
    }
    int startIdx = 0;
    if (s[0] == '+' || s[0] == '-') {
        signum = (s[0] == '-') ? -1 : 1;
        startIdx = 1; // Skip the sign for further processing
        s = s.substr(startIdx);
    } else {
        signum = 1; // Default to positive if no sign provided
    }
    // Validate the string only contains digits after optional sign removal
    for (size_t i = 0; i < s.length(); ++i) {
        if (!isdigit(s[i])) {
        }
    }
    std::string segment = "";
    size_t startIndex = 0;
    size_t digitsCount = s.length();
    while (startIndex < s.length() / p) {
        segment = s.substr(digitsCount - p, p);
        digits.insertAfter(std::stol(segment, nullptr, 10));
        digitsCount -= p;
        startIndex++;
    }
    if (digitsCount > 0) {
        segment = s.substr(0, digitsCount);
        digits.insertAfter(std::stol(segment, nullptr, 10));
    }
    // Cleaning up leading zeros if any, after forming the BigInteger
    digits.moveFront();
    while (digits.peekNext() == 0 && digits.length() > 1) {
        digits.eraseAfter();
        digits.moveFront(); 
    }
}
// Copy constructor
BigInteger::BigInteger(const BigInteger& N) {
    this->signum = N.signum;
    this->digits = N.digits;
}
// Get the sign of the BigInteger
int BigInteger::sign() const{
    return this->signum;
}
// Compare two BigIntegers
int BigInteger::compare(const BigInteger& N) const {
    // Compare signs
    if (sign() > N.sign()) {
        // If this BigInteger has a greater sign, return 1
        return 1;
    }
    else if (sign() < N.sign()) {
        // If this BigInteger has a smaller sign, return -1
        return -1;
    }
    else if (sign() == 0 && N.sign() == 0) {
        // If both BigIntegers are zero, return 0
        return 0;
    }
    // Compare digit lists
    List A_digits = digits;  // Make a copy of digits List for manipulation
    List B_digits = N.digits; // Make a copy of N's digits List for manipulation
    // First, compare lengths.
    int A_length = A_digits.length();
    int B_length = B_digits.length();
    if (A_length > B_length) return signum;
    if (A_length < B_length) return -N.signum;
    // If lengths are equal, compare digit-by-digit from the highest
    A_digits.moveFront();
    B_digits.moveFront();
    while (A_digits.position() < A_length && B_digits.position() < B_length) {
        ListElement A_digit = A_digits.moveNext();
        ListElement B_digit = B_digits.moveNext();
        if (A_digit > B_digit) return signum;    // Current digit of 'this' is larger
        if (A_digit < B_digit) return -N.signum; // Current digit of 'N' is larger
    }
    // Digit lists are identical
    return 0;
}
// Make BigInteger zero
void BigInteger::makeZero() {
    digits.clear();
    signum = 0;
}
// Negate a BigInteger
void BigInteger::negate() {
    signum *= -1;
}

// Addition operation
BigInteger BigInteger::add(const BigInteger& operand) const {
    BigInteger self = *this; 
    BigInteger other = operand; 
    BigInteger resultSum; 

    // Handling sign differences
    if (self.sign() > 0 && other.sign() < 0) {
        other.negate();
        return self.sub(other);
    } else if (self.sign() < 0 && other.sign() > 0) {
        self.negate();
        return other.sub(self);
    } else if (self.sign() < 0 && other.sign() < 0) {
        self.negate();
        other.negate();
        resultSum = self.add(other);
        resultSum.negate();
        return resultSum;
    }

    if (self > other) {
        return other.add(self);
    }

    List selfDigits = self.digits;
    List otherDigits = other.digits;
    List sumDigits;

    long overflow = 0; 
    long digitSum = 0; 

    // Traverse from the least significant digit
    selfDigits.moveBack();
    otherDigits.moveBack();

    while (selfDigits.position() > 0 && otherDigits.position() > 0) {
        digitSum = overflow + selfDigits.peekPrev() + otherDigits.peekPrev();
        overflow = digitSum / base;
        digitSum %= base;
        sumDigits.insertAfter(digitSum);
        selfDigits.movePrev();
        otherDigits.movePrev();
    }

    while (otherDigits.position() > 0) {
        digitSum = overflow + otherDigits.peekPrev();
        overflow = digitSum / base;
        digitSum %= base;
        sumDigits.insertAfter(digitSum);
        otherDigits.movePrev();
    }

    if (overflow > 0) {
        sumDigits.insertAfter(overflow);
    }

    resultSum.signum = self.signum;
    resultSum.digits = sumDigits;
    return resultSum;
}

// Subtraction operation
BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger num2 = *this;  
    BigInteger num1 = N;      
    BigInteger resultNum;     
    List digits1 = num1.digits; 
    List digits2 = num2.digits; 
    List resultDigits; 
    if (num1 == num2) {
        return resultNum;
    }
    if (num1.sign() && !num2.sign()) {
        num1.negate();
        return num1;
    }
    if (!num1.sign() && num2.sign()) {
        return num2;
    }
    if (num1.sign() < 0 && num2.sign() > 0) {
        num2.negate();
        resultNum = num1.add(num2);
        resultNum.negate();
        return resultNum;
    }
    if (num1.sign() > 0 && num2.sign() < 0) {
        num1.negate();
        resultNum = num1.add(num2);
        return resultNum;
    }
    if (num1.sign() < 0 && num2.sign() < 0) {
        num1.negate();
        num2.negate();
        resultNum = num2.sub(num1);
        resultNum.negate();
        return resultNum;
    }
    if (num1 < num2) {
        resultNum = num1.sub(num2);
        resultNum.negate();
        return resultNum;
    }
    digits1.moveBack();
    digits2.moveBack();
    long dist = 0;
    long temp = 0;
    int i = digits2.position();
    while (!(i <= 0)) {
        if (digits1.peekPrev() - dist < digits2.peekPrev()) {
            temp = digits1.peekPrev() + base - digits2.peekPrev() - dist;
            dist = 1;
        } else {
            temp = digits1.peekPrev() - dist - digits2.peekPrev();
            if (!(digits1.peekPrev() <= 0)) {
                dist = 0;
            }
        }
        resultDigits.insertAfter(temp);
        digits1.movePrev();
        digits2.movePrev();
        i--;
    }
    while (digits1.position() > 0) {
        if (digits1.peekPrev() - dist < 0) {
            temp = digits1.peekPrev() + base - 0 - dist;
            dist = 1;
        } else {
            temp = digits1.peekPrev() - dist - 0;
            if (!(digits1.peekPrev() <= 0)) {
                dist = 0;
            }
        }
        resultDigits.insertAfter(temp);
        digits1.movePrev();
    }
    resultNum.digits = resultDigits;
    resultNum.digits.moveFront();
    while (resultNum.digits.front() == 0 && resultNum.digits.length() > 1) {
        resultNum.digits.eraseAfter();
    }
    resultNum.signum = -1;
    return resultNum;
}

// Multiplication operation
BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger operandOne = *this; 
    BigInteger operandTwo = N; 
    BigInteger result;  
    List digitsOne = operandOne.digits;     
    List digitsTwo = operandTwo.digits;     
    int shift = 0;  
    digitsOne.moveBack();
    digitsTwo.moveBack();
    int digitPosition = digitsOne.position(); 
    // Iterate through each digit of operandOne
    for (int i = digitPosition; i > 0; i--) {
        long carryOver = 0;        
        long multiplicationResult = 0; 
        List intermediate; 
        long digitFromOne = digitsOne.peekPrev();  // Current digit of operandOne
        // Multiply current digit of operandOne with each digit of operandTwo
        for (digitsTwo.moveBack(); digitsTwo.position() > 0; digitsTwo.movePrev()) {
            multiplicationResult = (digitsTwo.peekPrev() * digitFromOne) + carryOver;
            carryOver = multiplicationResult / base;
            multiplicationResult %= base;
            intermediate.insertAfter(multiplicationResult);
        }
        if (carryOver > 0) {
            intermediate.insertAfter(carryOver);
        }
        intermediate.moveBack();
        // Shift the result based on the current digit position we are handling
        for (int j = 0; j < shift; j++) {
            intermediate.insertAfter(0);
        }
        // Combine with the accumulated result so far
        BigInteger tempProduct;
        tempProduct.signum = 1;
        tempProduct.digits = intermediate;
        result = result.add(tempProduct);
        digitsOne.movePrev();
        shift++;
    }
    result.signum = operandOne.signum * operandTwo.signum;
    return result;
}




// Convert BigInteger to string
std::string BigInteger::to_string() {
    std::string out = "";

    if (signum == 0) {
        return "0";
    } else if (signum == -1) {
        out += "-";
    }

    digits.moveFront();

    for (digits.moveFront(); digits.position() < digits.length(); digits.moveNext()) {
        std::string A = std::to_string(digits.peekNext());
        std::string B = "";

        while ((B.length() + A.length()) < p && digits.position() != 0) {
            B += '0';
        }

        out += (B + A);
    }

    return out;
}

// Output operator
std::ostream& operator<<(std::ostream& stream, BigInteger N) {
    return stream << N.to_string();
}

// Comparison operators
bool operator==(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) == 0;
}

bool operator<(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) < 0;
}

bool operator<=(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) <= 0;
}

bool operator>(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) > 0;
}

bool operator>=(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) >= 0;
}

// Arithmetic operators
BigInteger operator+(const BigInteger& A, const BigInteger& B) {
    return A.add(B);
}

BigInteger operator-(const BigInteger& A, const BigInteger& B) {
    return A.sub(B);
}

BigInteger operator*(const BigInteger& A, const BigInteger& B) {
    return A.mult(B);
}

// Compound assignment operators
BigInteger operator+=(BigInteger& A, const BigInteger& B) {
    A = A.add(B);
    return A;
}

BigInteger operator-=(BigInteger& A, const BigInteger& B) {
    A = A.sub(B);
    return A;
}

BigInteger operator*=(BigInteger& A, const BigInteger& B) {
    A = A.mult(B);
    return A;
}

