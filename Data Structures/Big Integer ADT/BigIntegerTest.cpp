/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA6
* BigIntegerTest.cpp
* tests all functions in BigInteger.cpp
***/

#include<iostream>
#include<string>
#include<stdexcept>
#include"BigInteger.h"
using namespace std;
int main(){
   string s1 = "91287346670000043892345634563400005619187236478";
   string s3 = "9876545439000000345634560000000002000034565430000000006543654365346534";
   string s5 = "98765454390000034563456191872363456345619187236000456456345756780000065436543";
   BigInteger A = BigInteger(s1);
   BigInteger B = BigInteger(s3);
   BigInteger C = BigInteger(s5);
   cout << endl;
   cout << "A = " << A << endl;
   cout << "B = " << B << endl;
   cout << "C = " << C << endl;
   cout << endl;
   // test the arithmetic opperators
   BigInteger D = A+B;
   BigInteger E = B+A;
   BigInteger F = D-E;
   BigInteger G = 53234*A + 29384747*B + 928374*C;
   BigInteger H = A*B;
   BigInteger I = B*A;
   BigInteger J = G*H*I;
   cout << "(A==B) = " << ((A==B)?"True":"False") << endl;
   cout << "(A<B)  = " << ((A<B)? "True":"False") << endl;
   cout << "(A<=B) = " << ((A<=B)?"True":"False") << endl;
   cout << "(A>B)  = " << ((A>B)? "True":"False") << endl;
   cout << "(A>=B) = " << ((A>=B)?"True":"False") << endl << endl;
   cout << "D = " << D << endl;
   cout << "E = " << E << endl;
   cout << "(D==E) = " << ((D==E)?"True":"False") << endl;
   cout << "F = " << F << endl;
   cout << "G = " << G << endl;
   cout << "H = " << H << endl;
   cout << "I = " << I << endl;
   cout << "(H==I) = " << ((H==I)?"True":"False") << endl;
   cout << "J = " << J << endl << endl;
   cout << endl;
   A += B;
   B -= C;
   C *= J;
   cout << "A = " << A << endl;
   cout << "B = " << B << endl;
   cout << "C = " << C << endl;
   cout << endl;
   cout << A*B*C*D*E*G*H*I*J << endl << endl;
   cout << endl;
   // test exceptions
   try{
      BigInteger K = BigInteger("");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      BigInteger L = BigInteger("+");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      BigInteger M = BigInteger("12329837492387492837492$4982379487293847");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      BigInteger N = BigInteger("5298374902837409+82734098729287349827398");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   cout << endl;
   return EXIT_SUCCESS;
}