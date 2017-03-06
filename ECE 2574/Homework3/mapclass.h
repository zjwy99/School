///////////////////////////////////////////
// ECE 2574, Homework 3, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	mapclass.h
// Description: Header file for ADT Map
// Date: March 26, 2015
//
#include "stackP.h"


class Map
{
public:
	Map(); //Default Constructor
	Map(string startCity); //Copy Constructor
	~Map(); //Destructor
	void addRoute(string city1,string city2, int cost); //Adds a flight route to a Stack object
	string flightRoute(string destination); //Returns a flight route given a destination, returns Fail if no route is found
	int totalCost(string route); //Returns the cost of a given flight route, assumes the route is a proper input
	void print(); //Prints the flightmap Stack
	string getStartCity() const; //Returns the origin city of the flight map
private:
	bool hasVisited(string city1, string city2); //Checks whether 
	void emptyVisited(); //Empties the visited Stack
	Stack flightmap; //Stack representing the flightmap
	Stack visited; //Stack used to aid in searching for a flight route
	string startCity; 
	
	

};