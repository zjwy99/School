//ECE 2500, Project 3, Zachary Yee
//ID #: 905796206
//File Name: cache.h
//Description: Header file for cache class.

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <vector> 
#include <map>
#include <bitset>
#include <cmath>

//Class to hold info for individual cache line
class Line
{
public:
	std::string tag;
	bool valid;
	bool dirty;
	int LRU;
	Line();

};

//class to info for a set in the cache
class Set
{
public:
	std::vector<Line> cacheLines;
	Set(int linesPerSet);
};

//class to hold info and functions for creating 
class Cache
{
public:
	//Various variables to hold info about cache simulation, names are self explanatory
	double hits;
	double misses;
	double reads;
	double writes;
	int cache_size;
	int tagSize;
	int linesPerSet;
	int offsetSize;
	int indexSize;
	std::string writePolicy;
	std::string cacheType;
	std::vector<Set> sets;

	//Constructor
	Cache(int numOfSets, int linesPerSet, std::string policy, std::string type, int blockSize, int cacheSize);  

	//Fuctions to help in cache simulation
	bool isHit(std::string address, int setIndex); // determines if the address is a hit within a given set
	void direct_map_read(std::string address); //handles reads when cache is direct map
	void direct_map_write(std::string address); //handles writes when cache is direct map
	void full_associative_read(std::string address); //handles reads when cache is fully associative
	void full_associative_write(std::string address);  //handles writes when cache is fully associative
	void set_associative_read(std::string address); //handles reads when cache is set associative
	void set_associative_write(std::string address); //handles writes when cache is set associative
	bool tagCompare(std::string binary1, std::string binary2); //compares two tags determines if they are equal
	int getSetIndex(std::string address); //gets the set iondex number for the given address
	bool isLineAvailable(int setIndex); //determines if there is a empty space in the set
	void LRU_update(int setIndex, int hitIndex); // updates the LRU
	void updateDirty(std::string address, int setIndex); // Updates dirty bit in set
};