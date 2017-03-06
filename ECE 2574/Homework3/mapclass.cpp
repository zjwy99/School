///////////////////////////////////////////
// ECE 2574, Homework 3, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	mapclass.cpp
// Description: Implementation file for the ADT Map
// Date: March 26, 2015
//
#include "mapclass.h"

//Default Constructor
Map::Map()
{
	flightmap = Stack();
	visited = Stack();
}

//Copy Constructor
Map::Map(string startCity)
{
	flightmap = Stack();
	visited = Stack();
	this->startCity = startCity;
}

//Destructor
Map::~Map()
{
	flightmap.~Stack();
	visited.~Stack();
}


void Map::addRoute(string city1, string city2, int cost)
{
	flightmap.addToStack(city1,city2,cost);

}

string Map::flightRoute(string destination)
{
	try
	{

		Stack *temp = new Stack(flightmap); //Temporary stack to aid in search of route to destination city
		string path = startCity; //String that contains the current route to the destination city
		string city = startCity; //City that needs to be searched for
		bool check = temp->isEmpty(); //Boolean to check whether the temp stack is empty
		string from = temp->getTopLetter1(); //Start city of the route
		string to = temp->getTopLetter2(); //End city of the route

		//Tries to find a flight route to the destination city, if there is a path the path is returned and if not Fail is returned
		while (check == false)
		{
			//Loop termination, if path to destination city is found
			if (city == destination)
			{
				emptyVisited();//Empties visited stack so that a future search could be performed
				return path;
			}

			//Gets the start and end city of the element on top of the stack 
			from = temp->getTopLetter1();
			to = temp->getTopLetter2();


			//Looks for route from city to unvisited adjacent city 
			if ((city == from) && (hasVisited(from, to) == false))
			{
				
				path = path + to; //Adds end city to the end of the path
				city = to; //Search city is now the top element's end city
				visited.addToStack(from, to, temp->getTopNumber()); //Adds the route to the visited stack

				//Resets temp stack
				delete temp;
				temp = new Stack(flightmap);
			}
			else
				temp->removeFromTop();


			check = temp->isEmpty();

			//Backtracking, if the no route from the search city to another city is found and if the visted stack is not empty
			if ((check == true) && (visited.isEmpty() == false))
			{


				check = false;

				//Resets temp stack
				delete temp;
				temp = new Stack(flightmap);

				//Backtracks to a different end city and the search city would be that end city
				if (city == visited.getTopLetter1())
				{
					//No path to destination city if 
					if (startCity == visited.getTopLetter2())
					{
						emptyVisited();//Empties visited stack so that a future search could be performed
						return "Fail";
					}

					//Removes cities in the visited stack with the same start city
					string tempCity = visited.getTopLetter1();
					while (tempCity == city)
					{
						visited.removeFromTop();

						//No path to destination if loop removes all elements in visited stack
						if (visited.isEmpty() == true)
						{
							return "Fail";
						}
						tempCity = visited.getTopLetter1();
					}

					tempCity = visited.getTopLetter1();
					city = visited.getTopLetter1();

					//If the backtracking removes cities in the path until no more can be removed, then there is no path to destination city
					if (path.size() < 1)
					{
						emptyVisited();//Empties visited stack so that a future search could be performed
						return "Fail";
					}
					path.pop_back(); //Removes city from the end of the path

				}
				else
				{
					city = visited.getTopLetter1(); //Search city is now the visited top element's start city
					path.pop_back(); //Removes city from the end of the path

				}


			}

		}
		emptyVisited(); //Empties visited stack so that a future search could be performed
		return "Fail";
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
			cerr << "Memory allocation failed" << endl;
	}
}


int Map::totalCost(string route)
{
	try
	{
		Stack *temp = new Stack(flightmap);
		string path = route;
		int cost = 0;

		//Searches for each part in the path and adds the price of each route to the total cost
		for (int i = 0; i < path.length() - 1; i++)
		{

			string from = string(1, path[i]);
			string to = string(1, path[i + 1]);
			bool check = temp->isEmpty();

			
			while (check == false)
			{
				if (temp->getTopLetter1() == from && temp->getTopLetter2() == to)
				{
					cost = cost + temp->getTopNumber();
					check = true;
				}
				else
					temp->removeFromTop();


			}

			//Resets the stack
			delete temp;
			temp = new Stack(flightmap);



		}

		return cost;
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
			cerr << "Memory allocation failed" << endl;
	}
}



void Map::print()
{
	flightmap.print();
}

bool Map::hasVisited(string city1,string city2)
{
	
	Stack temp = Stack(visited); //Creates temporary stack from the visited stack

	//Goes through each element of the stack to see if the given city is in the stack
	while (temp.isEmpty()==false)
	{
		if (city1 == temp.getTopLetter1() && city2 == temp.getTopLetter2())
			return true;
		else
			temp.removeFromTop();
	}
	return false;
}

void Map::emptyVisited()
{
	while (visited.isEmpty()==false)
	{
		visited.removeFromTop();
	}


}

string Map::getStartCity() const
{
	return startCity;
}