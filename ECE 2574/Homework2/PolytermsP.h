///////////////////////////////////////////
// ECE 2574, Homework 2, Zachary Yee
// ID #: 905796206
// File name:	PolytermsP.h
// Description: Header file for the ADT Poly
// Date:	March 2, 2015
//


#include <iostream>
#include <fstream>
using namespace std;
using std::istream;
using std::ostream;
using std::ofstream;
using std::ifstream;
typedef int  CoefficientItem;

//A struct that stores each element of the linked list
struct Node
{
public:
	CoefficientItem coefficient; //Holds coefficient term for item in linked list
	int exponent; //Holds the exponent term for item in linked list
	Node* next;
public:
	//Constructor 
	Node(CoefficientItem coefficient, int exponent);
	//Default Constructor
	Node();
};


class Poly
{
private:
	Node* head; //start of the linked list
	int length; //length of linked list
public:
	//Constructor
	Poly(); 
	//Copy Constructor
	Poly(const Poly& other);
	//Destructor
	~Poly();
	
	
public:
	//Overloaded Arithmetic Operators
	Poly operator+(const Poly& other)const;
	Poly operator*(const Poly& other)const;

	int getSize(); //Returns the length of the list
	Node* getHead(); //Returns the start Node of the list

	void addTerm(int coefficient, int exponent); //Adds a term to the polynomial at the end of the list
	
};