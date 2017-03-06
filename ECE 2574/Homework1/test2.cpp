////////////////////////////////////////////////////////
// File name:   test2.cpp
// Description: Client #2 to test ADT Matrix module (ECE2574 HW1)
// Date:        1/26/2015
//

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "matrix.h"

using namespace std;

int main(void)
{
   Matrix aa(4, 4);
   Matrix bb(4, 4); 
   Matrix cc(4, 4); 
   Matrix dd(4, 4);
   Matrix ee(1, 2);
   Matrix ff(5, 5);

   bb += aa;					// test "+="
   cout << "aa = " << aa << endl;
   cout << "bb += aa results in " << bb << endl;

   cc *= bb;					// test "*="
   cout << "cc *= bb results in " << cc << endl;

   if (aa == bb)
   {
	   cout << "aa and bb are equal:" << endl;
	   cout << "aa = " << aa << endl;
	   cout << "bb = " << bb << endl;
   }
   if (aa != bb)
   {
	   cout << "aa and bb are NOT equal:" << endl;
	   cout << "aa = " << aa << endl;
	   cout << "bb = " << bb << endl;
   }

   cc = bb;
   if (bb == cc)
   {
	   cout << "bb and cc are equal:" << endl;
	   cout << "bb = " << bb << endl;
	   cout << "cc = " << cc << endl;
   }
   if (bb != cc)
   {
	   cout << "bb and cc are NOT equal:" << endl;
	   cout << "bb = " << bb << endl;
	   cout << "cc = " << cc << endl;
   }

   bb = cc = dd;				// test repeated "="
   aa = bb + cc + dd;			// test repeated "+"
   cout << "aa = " << aa << endl;
   cout << "bb = " << bb << endl;
   cout << "cc = " << cc << endl;
   cout << "dd = " << dd << endl;
  
   // small and large sizes
   cout << "ee = " << ee << endl;
   cout << "ff = " << ff << endl;
   
   cin.get();	// pause display
   return 0;
} // End main


