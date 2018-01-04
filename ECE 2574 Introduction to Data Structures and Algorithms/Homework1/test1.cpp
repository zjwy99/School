////////////////////////////////////////////////////////
// File name:   test1.cpp
// Description: Client #1 to test ADT Matrix module (ECE2574 HW1)
// Date:        1/26/2015
//

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "matrix.h"

using namespace std;

int main(void)
{
   Matrix a(3, 4);
   Matrix b(3, 4); 
   Matrix c(3, 4); 
   Matrix d(4, 3);
   Matrix e(3, 3);

   cout << "a = " << a << endl;	// verify initialization

   c = a + b;					// test "+" and "="
   cout << "a = " << a << endl;
   cout << "b = " << b << endl;
   cout << "c = a + b  = " << c << endl;

   e = a * d;					// test "*"
   cout << "a = " << a << endl;
   cout << "d = " << d << endl;
   cout << "e = a * d  = " << e << endl;

   Matrix f = d;				// test copy constructor
   cout << "d = " << d << endl;
   cout << "f = " << f << endl;

   // test error handling
   cout << "trying to allocate a matrix of invalid size: " << endl;
   cout << "Matrix g(5, 0); " << endl;
   Matrix g(5, 0); 

   cout << "trying to add matrices of different sizes: " << endl;
   cout << "c = a + d " << endl;
   c = a + d; 

   cout << endl;
   cout << "trying to multiply matrices of incompatible sizes: " << endl;
   cout << "c = a * b " << endl;
   c = a * b; 
   
   cin.get();	// pause display
   return 0;
} // End main