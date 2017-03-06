///////////////////////////////////////////
// ECE 2574, Homework 5, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	mergesort.h
// Description: Header file for the mergesort class.
//				
// Date:	May 6, 2015
//

#include <iomanip>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;


struct node
{
	char letter;
	int frequency;

};




class Mergesort
{

	private:
		int size_complete_array; //size of the array

		//Methods to merge sort the array
		void merge(node *a, int low, int high, int mid);
		void sort(node *a, int start_index, int end_index);
	public:

		node* arrayC; //Array which stores nodes
	
		Mergesort(); //Default Constructor
		~Mergesort(); //Destructor
		void addnode(char let, int freq); //Adds an element to the array
		void mergesort(); //Method to initialize the sorting of the array
		void print(); //Prints content of array, for debugging 
		int size(); //Returns the size of the array

};