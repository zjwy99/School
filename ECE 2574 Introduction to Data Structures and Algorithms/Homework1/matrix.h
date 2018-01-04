///////////////////////////////////////////
// ECE 2574, Homework 1, Zachary Yee
// ID #: 905796206
// File name:	matrix.h
// Description: Header file for the ADT Matrix
// Date: February 11, 2015		
//



#include <iostream>
#include <iomanip>
#include <stdlib.h>
using namespace std;
using std::setprecision;
using std::fixed;

typedef double MatrixItem;


class Matrix
{
	// overload operator<< to print a Matrix
	friend ostream& operator<<(ostream& out, Matrix& orig);
public:
	// Constructor: create a matrix of size (row_size x column_size)
	Matrix(int row_size, int column_size);
	// Copy constructor: create a deep copy of matrix orig
	Matrix(const Matrix& orig);

	// Destructor: deallocate memory that was allocated dynamically
	~Matrix();
	// Set/Get the value of an element at (row, column), where
	// erowf ranges from 0 to (row_size-1), and ecolumnf ranges
	// from 0 to (column_size-1)
	void setElement(int row, int column, MatrixItem value);
	MatrixItem getElement(int row, int column) const;
	// Overloaded assignment operator
	Matrix& operator=(const Matrix& orig);
	//// Overloaded matrix math operators
	Matrix operator+(const Matrix& orig) const;
	Matrix& operator+=(const Matrix& orig);
	Matrix operator*(const Matrix& orig) const;
	Matrix& operator*=(const Matrix& orig);
	// Overloaded comparison operators
	bool operator==(const Matrix& orig);
	bool operator!=(const Matrix& orig);

private:
	int rows; // the number of rows in the matrix
	int columns; // the number of columns in the matrix
	MatrixItem *data; // dynamically allocated array storage
}; // End Matrix class
// End header file