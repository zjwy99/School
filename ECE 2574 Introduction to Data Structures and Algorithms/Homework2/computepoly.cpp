////////////////////////////////////////////////////////
// File name:   computepoly.cpp
// Description: Client #1 to test ADT Poly module (ECE2574 HW2)
// Date:    March 2, 2015    
//



#include <string>
#include "PolytermsP.h"
using std::string;
using namespace std;


//Method to print polynomial data into text file
void printToFile(string filename, Node *head)
{
	ofstream out(filename, ios::app);
	for (Node *temp = head; temp != NULL; temp = temp->next)
	{

		out << temp->coefficient << " " << temp->exponent << endl;

	}
}


int main(int argc, char* argv[]) {
	try{
		//Checks to see if input and output files are provided
		if (argc != 3)
			throw 1;

		//Gets the input and output file names
		string input = argv[1];
		string output = argv[2];
		ifstream in(input); //For reading in data from input file
		ofstream out(output, ios::app); //For appending data to output file

		//Initializes two polynomials
		Poly a = Poly();
		Poly b = Poly();

		//Stores data as a string
		string number;
		in >> number;

		//Places data into Poly as long as the data is valid
		while (number != "XXX"&&!in.fail())
		{

			int coeff, expt;
			coeff = stoi(number);
			in >> number;
			expt = stoi(number);
			in >> number;
			a.addTerm(coeff, expt);
		}
		//Checks if text file provided proper input data
		if (number != "XXX"||a.getSize()==0)
			throw 2;

		//Places data into Poly as long as the data is valid
		in >> number;
		while (!in.fail())
		{
			int coeff, expt;
			coeff = stoi(number);
			in >> number;
			expt = stoi(number);
			in >> number;
			b.addTerm(coeff, expt);
		}
		in.close(); //closes input file stream

		//Checks if text file provided proper input data
		if (b.getSize() == 0)
			throw 2;

		Poly temp = a + b; //Adds two polynomials together and stores result in temp
		//Prints results of addition to file
		printToFile(output,temp.getHead()); 
		out << "YYY" << endl;

		Poly temp2 = a*b; //Multiplies two polynomials together and stores result in temp2
		printToFile(output,temp2.getHead()); //Prints result of multiplication to file
	}

	//Error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Input or output file not provided" << endl;
		if (err == 2)
			cerr << "Bad input file" << endl;
		
	}
	
	return 0;
}

