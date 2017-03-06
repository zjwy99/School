///////////////////////////////////////////
// ECE 2574, Homework 3, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	stackP.cpp
// Description: Implementation file for the ADT Stack
// Date: March 26, 2015
//
#include "stackP.h"

//Default Constructor
Stack::Stack()
{
	size = 0; 
	top = NULL;
}

//Copy Constructor, creates Deep Copy
Stack::Stack(const Stack& otherStack)
{
	size = 0;
	Node *temp = top;

	//Deletes contents of current stack so that content of otherStack could be replaced
	while (temp!=NULL)
	{
		temp = top->next;
		delete top;
		top = temp;
	}
	this->top = NULL;

	//Places contents of otherStack in a temporary stack, called t. Note: order of stack t is reverse of otherStack
	Stack t = Stack();   
	for (Node *temp = otherStack.top; temp != NULL; temp = temp->next)
	{
		t.addToStack(temp->first,temp->second,temp->number);
	}

	//Places the contents of stack t in the current stack, order of the current stack matches otherStack
	for (Node *temp = t.top; temp != NULL; temp = temp->next)
	{
		addToStack(temp->first, temp->second, temp->number);
	}

	
}

//Destructor
Stack::~Stack()
{
	Node *temp = top;

	//Simply sets each Node in the stack to NULL
	while (temp != NULL)
	{
		Node *next = temp->next;
		delete temp;
		temp = next;
	}
	top = NULL;
}

void Stack::addToStack(NodeLetter first, NodeLetter second, NodeNumber number)
{
	try
	{
		Node *newElement = new Node(first, second, number); //Creates new Node to add to Stack

		//If the stack is empty the Node is now the top Node
		if (top == NULL)
		{
			newElement->next = NULL;
			top = newElement;
			size++;
		}

		//Puts the newElement on top of the stack
		else
		{
			newElement->next = top;
			top = newElement;
			size++;
		}
	}
	
	//Exception handling error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Memory allocation failed" << endl;
	}
}

void Stack::removeFromTop()
{
	try
	{
		//Checks to see if stack is empty
		if (top == NULL)
			throw 1;

		Node *temp = top;
		top = top->next; // sets top node to the next in the stack
		size--;
		delete(temp); //delete top node
	}

	//Exception handling error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Stack is empty." << endl;
	}
}

void Stack::remove(NodeLetter first, NodeLetter second, NodeNumber number)
{
	
	Stack temp = Stack();
	for (Node* tempN = top; tempN != NULL;tempN=tempN->next)
	{
		if (tempN->first != first || tempN->second != second || tempN->number != number)
		{
			temp.addToStack(tempN->first,tempN->second,tempN->number);
		}

	}
	Node *t = top;
	while (t != NULL)
	{
		Node *next = t->next;
		delete t;
		t = next;
	}
	top = NULL;
	for (Node* tempN = temp.top; tempN != NULL; tempN = tempN->next)
	{
		addToStack(tempN->first, tempN->second, tempN->number);
	}

}


string Stack::getTopLetter1()
{

	try
	{
		if (top == NULL)
			throw 1;
		string letter = top->first;
		return letter;
	}

	//Exception handling error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Stack is empty." << endl;
	}
}

string Stack::getTopLetter2()
{
	try
	{
		if (top == NULL)
			throw 1;
		string letter = top->second;
		return letter;
	}

	//Exception handling error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Stack is empty." << endl;
	}
}

int Stack::getTopNumber()
{
	try
	{
		if (top == NULL) 
			throw 1;
		int num = top->number;
		return num;
	}

	//Exception handling error messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Stack is empty." << endl;
	}
}


void Stack::print() 
{
	Node *temp = top;
	while (temp != NULL)
	{
		cout << temp->first << " " << temp->second << " " << temp->number << endl;
		temp = temp->next;
	}
}

bool Stack::isEmpty() const 
{
	//Checks size
	if (size <= 0)
		return true;
	else
		return false;
}

int Stack::getSize() const

{
	return size;
}