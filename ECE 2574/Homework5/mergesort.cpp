///////////////////////////////////////////
// ECE 2574, Homework 5, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	mergesort.cpp
// Description: Implementation file for the mergesort class
// Date: May 6, 2015
//

#include "mergesort.h"

//Default Constructor, initializes array and size
Mergesort::Mergesort()
{

	size_complete_array = 0;
	arrayC = new node[0];
	
}

//Destructor
Mergesort::~Mergesort()
{
	
	delete[] arrayC;

}


void Mergesort::addnode(char let, int freq)
{
	try
	{
		node *temp = new node[size_complete_array + 1];
		node newNode;
		newNode.letter = let;
		newNode.frequency = freq;
		for (int right_index = 0; right_index < size_complete_array; right_index++)
		{

			temp[right_index] = arrayC[right_index];
		}

		temp[size_complete_array] = newNode;

		size_complete_array++;
		delete arrayC;

		arrayC = temp;
	}

	//Exception handling
	catch (bad_alloc& err)
	{
		throw 1;
	}

	//Exception messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Memory Allocation Failed" << endl;
	}
}

//Method for the client file to start the merge sort
void Mergesort::mergesort()
{
	sort(arrayC, 0, (size_complete_array - 1));

}

void Mergesort::merge(node *a, int start_index, int end_index, int mid)
{
	try
	{
		
		
		//Temporary array for storing the sorted array, the size of the temporary array will never exceed the size of the array that needs to be sorted
		node *temp = new node[size_complete_array];

		//Initializes values 
		int right_index = start_index; //Index that starts the right section of the array
		int temp_index = start_index; //Helps with determining how many values are in the temporary array
		int left_index = mid + 1; //Index that starts the left section of the array

		/*Goes through each section of the array and compares a value on the right with a value on the left.
		Smaller values go before larger values for ascending order.
		*/
		while (right_index <= mid && left_index <= end_index)
		{

			
			if (a[right_index].frequency < a[left_index].frequency)
			{
				temp[temp_index] = a[right_index]; //places value into temporary array
				temp_index++;
				right_index++; //Since value is placed in temporary array, index increases to compare next value
			}
			else
			{
				temp[temp_index] = a[left_index]; //places value into temporary array
				temp_index++;
				left_index++; //Since value is placed in temporary array, index increases to compare next value
			}
		}

		//Places the remaining values on the right section of the array into the temporary array, if there are any remaining values
		while (right_index <= mid)
		{
			temp[temp_index] = a[right_index];
			temp_index++;
			right_index++;
		}

		//Places the remaining values on the left section of the array into the temporary array, if there are any remaining values
		while (left_index <= end_index)
		{
			temp[temp_index] = a[left_index];
			temp_index++;
			left_index++;
		}

		//Copies the values from the temporary array into the array that needs to be sorted
		for (int x = start_index; x < temp_index; x++)
		{
			a[x] = temp[x];
		}

		delete[] temp; //Returns the memory that was allocated back to the memory
	}

	//Exception handling 
	catch (bad_alloc& err)
	{
		throw 1;
	}

	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Memory Allocation Failed" << endl;
	}
}

//Method to start the mergesort in ascending order
void Mergesort::sort(node *a, int start_index, int end_index)
{
	/*Takes an array, divides it into two sections, then sorts the individual sections. Does this until the array can longer be subdivided
	Afterwords it combines each section and merges them in the process.
	*/
	if (start_index < end_index)
	{
	
		sort(a, start_index, (start_index + end_index) / 2);
		sort(a, ((start_index + end_index) / 2) + 1, end_index);
		merge(a, start_index, end_index, (start_index + end_index) / 2);
	}
	
}

//Prints values of array, for debugging 
void Mergesort::print()
{
	
	for (int x = 0; x < size_complete_array; x++)
	{
		cout << arrayC[x].letter <<" " <<arrayC[x].frequency << endl;
	}
	
}


int Mergesort::size()
{

	return size_complete_array;
}