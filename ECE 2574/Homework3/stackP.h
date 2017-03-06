///////////////////////////////////////////
// ECE 2574, Homework 3, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	stackP.h
// Description: Header file for the ADT Stack
// Date:	March 26, 2015
//
#include <iostream>
#include <fstream>
#include <string>
using std::istream;
using std::ostream;
using std::ofstream;
using std::ifstream;
using namespace std;
typedef string NodeLetter;
typedef int NodeNumber;

class Stack
{
	public:
		Stack(); //Default Constructor
		Stack(const Stack& otherStack); //Copy Constructor
		~Stack(); //Destructor
		void addToStack(NodeLetter first,NodeLetter second,NodeNumber number ); //Adds a new Node struct to the top of the stack
		void removeFromTop(); //removes the top Node from the stack
		void remove(NodeLetter first, NodeLetter second, NodeNumber number); //Removes specific node from the stack
		string getTopLetter1(); //Returns the first top letter of the stack 
		string getTopLetter2(); //Returns the second top letter of the stack 
		int getTopNumber(); //Returns the top number of the stack
		void print(); //Prints the contents of the stack
		bool isEmpty()const; //Determines if stack is empty and returns result
		int getSize() const; //Returns the size of the stack
	private:
		//Node declaration and implementation
		struct Node
		{
	
			NodeLetter first;
			NodeLetter second;
			NodeNumber number;
			Node *next;
			Node(NodeLetter first, NodeLetter second, NodeNumber number)
			{
				this->first = first;
				this->second = second;
				this->number = number;
				this->next = NULL;
			}
			Node()
			{
				this->next = NULL;
			}
		
		};
		Node *top; //Pointer of the top Node of the stack
		int size; //Variable where the size of the stack is stored

};