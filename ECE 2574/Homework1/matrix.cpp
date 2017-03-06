///////////////////////////////////////////
// ECE 2574, Homework 1, Zachary Yee
// ID #: 905796206
// File name:	matrix.cpp
// Description: Implementaion file for the ADT Matrix
// Date: February 11, 2015	
//

#include "matrix.h"

Matrix::Matrix(int row_size, int column_size)
{
	try
	{
		//checks to see if inputed matrix dimensions are valid
		if (row_size <= 0)
			throw 1;
		if (column_size <= 0)
			throw 2;

		//stores the dimensions of the matrix
		rows = row_size;
		columns = column_size;

		//allocate data array & verify memory has been allocated
		data = new MatrixItem[row_size*column_size];
		if (data == NULL)
			throw 3;

		// initialize Matrix elements
		int i, j;
		for (i = 0; i < row_size; i++)
			for (j = 0; j < column_size; j++)
				data[i*column_size + j] = (MatrixItem)(i - j);
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err == 1)
			cerr << "Illegal number of rows" << endl;
		if (err == 2)
			cerr << "Illegal number of columns" << endl;
		if (err == 3)
			cerr << "No Information in Array" << endl;
		exit(-1);
	}
	
	

}

//Copy constructor
Matrix::Matrix(const Matrix& orig)
{
	//sets matrix dimensions to orig's dimensions
	rows = orig.rows;
	columns = orig.columns;

	try
	{
		
		//allocate data array & verify memory has been allocated
		data = new MatrixItem[rows*columns];
		if (data == NULL)
			throw 1;

		//fills matrix values with orig's values
		for (int i = 0; i<rows; i++)
			{
				for (int j = 0; j<columns; j++)
				{
					data[i*columns + j] = orig.data[i*columns + j];
				}
			}
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err==1)
			cerr << "No Information in Array" << endl;
		exit(-1);
	}
	

}

//Destructor 
Matrix::~Matrix()
{
	if (data!=NULL)
		delete data;
}


void Matrix::setElement(int row, int column, MatrixItem value)
{
	data[(row)*columns + (column)] = value; 
}

MatrixItem Matrix::getElement(int row, int column) const
{

	return data[(row)*columns + (column)];
}

//Implementation of overloaded << operator
ostream& operator<<(ostream& out, Matrix& orig)
{

	//displays entire matrix
	cout << "\n";
	for (int i = 0; i < orig.rows; i++)
	{
		for (int j = 0; j < orig.columns; j++)
		{
			if (j < 5 && i < 5)
			{
				//
				cout << "  " << setprecision(1) << fixed << orig.data[i*orig.columns + j];
			}
			if (j == orig.columns - 1)
			{
				//creates new line when finished outputing row
				cout << "\n";
			}

		}
	}
	return out;
}

//Implementation of overloaded + operator
Matrix Matrix::operator+(const Matrix& orig) const
{
	try
	{
		//Checks to see if matrices are valid for addition
		if (rows != orig.rows)
			throw 1;
		if (columns != orig.columns)
			throw 2;

		//Creates new matrix to return as a result for addition
		Matrix temp(orig.rows, orig.columns);
		MatrixItem value;

		//adds the values of the matrix and places values in temp
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				value = getElement(i, j) + orig.getElement(i, j);
				temp.setElement(i, j, value);
			}
		}

		return temp;
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err == 1)
			cerr << "Matrices do not have equal number of rows " << endl;
		if (err == 2)
			cerr << "Matrices do not have equal number of columns" << endl;
		exit(-1);
	}
}

//Implementation of overloaded += operator
Matrix& Matrix::operator+=(const Matrix& orig)
{
	try
	{
		//Checks to see if matrices are valid for addition
		if (rows != orig.rows)
			throw 1;
		if (columns != orig.columns)
			throw 2;

		//Adds the matrices values and replaces the left-hand matrix with the result
		MatrixItem value;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				value = getElement(i, j) + orig.getElement(i, j);
				setElement(i, j, value);
			}
		}
		return *this;
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err == 1)
			cerr << "Matrices do not have equal number of rows " << endl;
		if (err == 2)
			cerr << "Matrices do not have equal number of columns" << endl;
		exit(-1);
	}
	
}

//Implementation of overloaded * operator
Matrix Matrix::operator*(const Matrix& orig) const
{
	try
	{
		//checks to see if matrices are valid for multiplication 
		if (columns != orig.rows)
			throw 1;

		//creates new matrix with the results of mulitplied matrices 
		Matrix temp  (rows, orig.columns) ;
		MatrixItem value = 0;
		for (int i = 0; i < temp.rows; i++)
		{
			
			for (int j = 0; j < temp.columns; j++)
			{
				for (int x = 0; x < columns; x++)
				{
					
					value = value + (getElement(i, x)*orig.getElement(x, j));
				}
				temp.setElement(i, j, value);
				value = 0;
			}
		}

		return temp;
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err == 1)
			cerr << "Invalid dimensions for matrix multiplication" << endl;
		exit(-1);
	}
}

//Implementation of overloaded *= operator
Matrix& Matrix::operator*=(const Matrix& orig)
{
	try
	{
		//checks to see if matrices are valid for multiplication 
		if (columns != orig.rows)
			throw 1;

		//Creates temporary matrix to store multiplied values
		Matrix temp(rows, orig.columns);
		MatrixItem value = 0;
		for (int i = 0; i < temp.rows; i++)
		{
			for (int j = 0; j < temp.columns; j++)
			{
				for (int x = 0; x < columns; x++)
				{
					
					value = value + (getElement(i, x)*orig.getElement(x, j));
				}
				temp.setElement(i, j, value);
				value = 0;
			}
		}
		delete data; //deallocates array from memory
		data = temp.data; //data points to temp's matrix
		temp.data = NULL; //temp's data pointer points to NULL
		if (data == NULL)
			throw 2;

		//sets matrix dimensions to the temp's matrix dimensions
		rows = temp.rows;
		columns = temp.columns;
		return *this;
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err == 1)
			cerr << "Invalid dimensions for matrix multiplication" << endl;
		if (err == 2)
			cerr << "No Information in Array" << endl;
		exit(-1);
	}
}





//Implementation of overloaded = operator
Matrix& Matrix::operator=(const Matrix& orig)
{
	try
	{
		//checks for self assignments
		if (orig.data == data)
			return *this;

		delete data; //deallocates array from memory
		//allocate data array & verify memory has been allocated
		data = new MatrixItem[rows*columns];
		if (data == NULL)
			throw 1;

		//sets matrix dimensions to the orig matrix dimensions
		rows = orig.rows;
		columns = orig.columns;

		//replaces matrix values with orig's matrix values
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{

				setElement(i, j, orig.getElement(i, j));
			}
		}
		return *this;
	}
	//Error messages
	catch (int err)
	{
		cerr << "Error " << endl;
		if (err == 1)
			cerr << "No Information in Array" << endl;
		exit(-1);
	}
}

//Implementation of overloaded == operator
bool Matrix::operator==(const Matrix& orig)
{
	//Matrices are not equal if dimensions do not match
	if (rows != orig.rows || columns != orig.columns)
		return false;

	//Checks to see if any values in the matrices differ
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (getElement(i, j) != orig.getElement(i, j))
				return false;
		}
	}

	return true;

}

//Implementation of overloaded != operator
bool Matrix::operator!=(const Matrix& orig)
{
	//Matrices are not equal if dimensions do not match
	if (rows != orig.rows || columns != orig.columns)
		return true;

	//Checks to see if any values in the matrices differ
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (getElement(i, j) != orig.getElement(i, j))
				return true;
		}
	}

	return false;
} 

// End of implementation file
