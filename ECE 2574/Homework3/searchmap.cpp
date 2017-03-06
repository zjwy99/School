///////////////////////////////////////////
// ECE 2574, Homework 3, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	searchmap.cpp
// Description: Client code to test ADT Map module (ECE2574 HW3)
// Date: March 26, 2015
//
#include "mapclass.h"
#include <iomanip>
using std::setw;


int main(int argc, char* argv[])
{


	try
	{
		//Exception thrown if no input or output file provided
		if (argc != 3)
			throw 1;

		
		int numOfCities = 0; //int to hold number of destination cities
		string* cities = new string[1];//Array to hold possible destination cities

		//input and output stream
		ifstream in(argv[1]);
		ofstream out(argv[2], ios::app);

		//Temporary strings to hold input of inputfile
		string temp;
		string temp2;
		int temp3;

		//Checks if origin city is provided and creates a Map object if so
		getline(in, temp);
		if (temp.size() > 1)
			throw 2;
		Map a = Map(temp);
		
		//Gets input from file and checks if input is valid
		while (!in.fail())
		{
			in >> temp;
			if (temp.length() < 1)
				throw 3;
			in >> temp2;
			if (temp2.length() < 1)
				throw 3;
			in >> temp3;
			
			if (temp3 <= 0)
				throw 4;
			if (!in.fail())
			{
				
					a.addRoute(temp, temp2, temp3);
			}

			//Puts potential destination cities into array
			if (numOfCities == 0)
			{

				cities[0] = temp2;
				numOfCities++;

			}
			
			else 
			{
				//Checks if destination city is already in array 
				bool check = false;
				for (int i = 0; i < numOfCities; i++)
				{
					if (cities[i] == temp2) 

						check = true;

				}

				//Expands array and puts new city into array if destination city is not in the array
				if ((check == false) && temp2 != a.getStartCity())
				{
					string* newArr = new string[numOfCities + 1];
					for (int i = 0; i < numOfCities; i++)
					{
						newArr[i] = cities[i];
					}

					newArr[numOfCities] = temp2;

					numOfCities++;
					delete[] cities;
					cities = newArr;
				}
			}


		}


		//Outputs information to a file with proper formating 
		out << "Destination" << "\t" << "Flight Route from " << a.getStartCity() << "\t" << "Total Cost" << endl;
		for (int i = 0; i < numOfCities; i++)
		{

			string route = a.flightRoute(cities[i]);

			//Only outputs routes that have a route to the destination city
			if (route != "Fail")
			{
				int cost = a.totalCost(route);
				int size = route.size()-1;
				
				out << cities[i] << "\t" << "\t"; //Outputs the destination city

				//Modifies the route string to proper format
				for (int x = 1; x <= size;x++)
				{
					route.insert((x+x)-1, ",");
				}
				
				out << left << setw(24) << route << "$" << cost << endl; //Outputs the route and the cost of associated route
			}
			
		}
		
		
	
	}

	//Exception handling
	catch (bad_alloc& err)
	{
		throw 5;
	}

	
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Input or output file not provided" << endl;
		if (err == 2)
			cerr << "Bad input file, no origin city" << endl;
		if (err == 3)
			cerr << "Bad input file, a city is represented with more than one letter" << endl;
		if (err == 4)
			cerr << "Bad input file, no associated flight cost, malformed pair of cities, or non-positive flight cost" << endl;
		if (err == 5)
			cerr << "Memory allocation failed" << endl;
		
	}
	
	
	
	return 0;
}