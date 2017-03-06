///////////////////////////////////////////
// ECE 2574, Homework 2, Zachary Yee
// ID #: 905796206
// File name:	PolytermsP.cpp
// Description: Implementaion file for the ADT Poly
// Date: March 2, 2015
//

#include "PolytermsP.h"
using namespace std;

//struct Node constructor
Node::Node(CoefficientItem coefficient, int exponent)
{
	this->coefficient = coefficient;
	this->exponent = exponent;
	this->next = NULL;
}

//Default Node constructor
Node::Node()
{
	this->next = NULL;
}

//Default Poly constructor
Poly::Poly()
{
	length = 0;
	head = NULL;
}

//Copy Constructor
Poly::Poly(const Poly& other)
{
	length = 0;
	Node* temp = head;
	while (temp != NULL)
	{
		temp = head->next;
		delete head;
		head = temp;
	}
	this->head = NULL;
	CoefficientItem cof;
	int expt;
	for (Node *temp = other.head; temp != NULL; temp = temp->next)
	{
		cof = temp->coefficient;
		expt = temp->exponent;
		addTerm(cof, expt);
	}

}

//Destructor
Poly::~Poly()
{

	Node* temp = head;
	while (temp != NULL)
	{
		temp = head->next;
		delete head;
		head = temp;
	}

}

//Returns the number of terms in the polynomial
int Poly::getSize()
{
	return length;
}

//Returns the start Node of the linked list
Node* Poly::getHead()
{
	return head;
}





//Adds a term to the polynomial at the end of the list
void Poly::addTerm(CoefficientItem coefficient, int exponent) {
	try{

		//Exception checking, sees if input is valid
		if (coefficient == 0)
			throw 1;
		if (exponent < 0)
			throw 2;

		//Checks to see if there is any terms in the polynomial
		if (head == NULL)
		{
			//Allocates memory for Node and check if memory was successfully allocated
			Node *temp;
			try
			{
				temp = new Node(coefficient, exponent);  //Creates a new Node to add to the list
			}
			catch (bad_alloc& err)
			{
				throw 3;
			}
			head = temp; //Makes the temporary Node the start of the list
		}
		else
		{
			//Finds the end of the list 
			Node *temp = NULL;
			for (Node *current = head; current != NULL; current = current->next)
			{
				temp = current; //sets temp to the last
			}

			//Allocates memory for Node and check if memory was successfully allocated
			Node *temp2;
			try
			{
				temp2 = new Node(coefficient, exponent);
			}
			catch (bad_alloc& err)
			{
				throw 3;
			}
			temp->next = temp2; //Sets the next block of memory to temp2
		}
		length++; //Increments length 
	}

	//Error statements
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Input coefficient is zero" << endl;
		if (err == 2)
			cerr << "Input exponent is negative" << endl;
		if (err == 3)
			cerr << "Allocation Failed" << endl;
	}
}

//Overloaded addition operator
Poly Poly::operator+(const Poly& other)const
{
	try{

		//Allocates memory for tempPoly 
		Poly* tempPoly;
		try
		{
			tempPoly = new Poly();
		}

		//Checks is allocation is succcessful
		catch (bad_alloc& err) 
		{
			throw 1;
		}

		//Adds like polynomial terms together
		CoefficientItem cofTemp;
		int exptTemp;
		for (Node *temp = head; temp != NULL; temp = temp->next)
		{
			cofTemp = temp->coefficient;
			exptTemp = temp->exponent;
			bool check = false;

			for (Node *temp2 = other.head; temp2 != NULL; temp2 = temp2->next)
			{
				if (exptTemp == temp2->exponent)
				{
					cofTemp = cofTemp + temp2->coefficient;
					tempPoly->addTerm(cofTemp, exptTemp);
					check = true;
				}


			}
			if (check == false)
				tempPoly->addTerm(cofTemp, exptTemp);


		}


		Node* temp = tempPoly->head;

		//Allocates memory for tempPoly2 and checks if allocation is successful
		Poly* tempPoly2;
		try
		{
			tempPoly2 = new Poly();
		}
		catch (bad_alloc& err)
		{
			throw 1;
		}

		//Finds largest exponent in the polynomial
		int largestExponent = 0;
		for (int i = 0; i < tempPoly->length; i++)
		{
			if (temp->exponent > largestExponent)
				largestExponent = temp->exponent;

			temp = temp->next;
		}

		//Adds like terms together and sorts list in decreasing exponential order
		for (int i = largestExponent; i >= 0; i--)
		{
			CoefficientItem cof = 0;
			for (Node* temp = tempPoly->head; temp != NULL; temp = temp->next)
			{

				if (temp->exponent == i)
					cof = cof + temp->coefficient;
			}
			if (cof > 0)
				tempPoly2->addTerm(cof, i);
		}



		return *tempPoly2; //Returns the final sorted polynomial
	}

	//Error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Allocation Failed" << endl;
	}
}


//Overloaded muliplication operator
Poly Poly::operator*(const Poly& other)const
{
	try
	{
		//Allocates memory for tempPoly and check if memory was successfully allocated
		Poly* tempPoly;
		try
		{
			tempPoly = new Poly();
		}

		catch (bad_alloc& err) 
		{
			throw 1;
		}

		//Multiplies the polynomials
		CoefficientItem cofTemp;
		int exptTemp;
		for (Node *temp = head; temp != NULL; temp = temp->next)
		{
			cofTemp = temp->coefficient;
			exptTemp = temp->exponent;

			for (Node *temp2 = other.head; temp2 != NULL; temp2 = temp2->next)
			{
				tempPoly->addTerm(cofTemp*temp2->coefficient, exptTemp + temp2->exponent);
			}
		}

		
		Node* temp = tempPoly->head;

		//Allocates memory for tempPloy2 and checks if allocation was successful 
		Poly* tempPoly2;
		try
		{
			tempPoly2 = new Poly();
		}
		catch (bad_alloc& err) 
		{
			throw 1;
		}

		//Finds the largest exponent
		int largestExponent = 0;
		for (int i = 0; i < tempPoly->length; i++) //finds the largest exponent 
		{
			if (temp->exponent > largestExponent)
				largestExponent = temp->exponent;

			temp = temp->next;
		}

		//Adds like terms together and sorts polynomial by decreasing exponential order
		for (int i = largestExponent; i >= 0; i--)
		{
			CoefficientItem cof = 0;
			for (Node* temp = tempPoly->head; temp != NULL; temp = temp->next)
			{

				if (temp->exponent == i)
					cof = cof + temp->coefficient;
			}
			if (cof > 0)
				tempPoly2->addTerm(cof, i);
		}


		return *tempPoly2;
	}

	//Error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Allocation Failed" << endl;
	}
}


