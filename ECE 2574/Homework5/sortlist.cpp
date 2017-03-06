///////////////////////////////////////////
// ECE 2574, Homework 5, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	sortlist.cpp
// Description: Client code to test merge sort
// Date: May 6, 2015
//
#include "mergesort.h"
using namespace std;



int main(int argc, char* argv[])
{
	try
	{
		//Throws exception if input or output file not provided
		if (argc != 3)
			throw 1;

		Mergesort test = Mergesort(); //Creates an instance of the mergesort class
		ifstream in(argv[1]); //Input file stream

		//the letter and frequency for the node that needs to be stored in the array
		char temp;
		int temp2;

		//Checks if the character is a letter
		in >> temp;
		if (isalpha(temp) == false)
			throw 3;

		//Checks if there is a number
		in >> temp2;
		if (in.fail())
			throw 4;
			
		//Throws exception if there are no values in input file or input file does not exist
		if (in.fail())
			throw 2;

		//Loop to add values into array
		while (!in.eof())

		{
			test.addnode(temp, temp2);
			in >> temp;
			//Checks if the character is a letter
			if (isalpha(temp) == false)
				throw 3;
			//Checks if there is a number
			in >> temp2;
			if (in.fail())
				throw 4;
				
			
		}
		
		test.addnode(temp, temp2); // To place last values into array

		

		test.mergesort(); //Sorts the array using merge sort
		test.print(); //Prints values of array to console, for debuggine


		//Places result of sorted array into output file
		ofstream out(argv[2]);
		
		for (int i = 0; i< test.size(); i++)
		{
			out << test.arrayC[i].letter << " " << test.arrayC[i].frequency << endl;

		}
		cin.get();
		return 0;

	}

	//Exception Messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Input or output file not provided" << endl;
		if (err == 2)
			cerr << "Input file is empty or no input file provided" << endl;
		if (err == 3)
			cerr << "Missing letter in input file" << endl;
		if (err == 4)
			cerr << "Missing frequency in input file" << endl;
		cin.get();
		return 0;
	}
}