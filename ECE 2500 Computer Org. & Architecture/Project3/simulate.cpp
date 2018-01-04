//ECE 2500, Project 3, Zachary Yee
//ID #: 905796206
//File Name: simulate.cpp
//Description: client file for cache simulation.


#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include "cache.h"

std::map< char, std::string> hex; // map to aid in conversion from hex to binary

//function used to turn hex to binary format
std::string hexToBinary(std::string toConvert);

//Various configurations for a cache
int cacheSizes[4] = {1024, 4096, 65536, 131072};
int blockSizes[4] = { 8, 16, 32, 128 };
std::string mappingType[4] = {"DM", "2W", "4W", "FA"};
std::string writePolicy[2] = {"WB", "WT"};

//struct used to store instruction obtained from file
struct instruction
{
	std::string inst;
	std::string binary;
};



int main(int argc, const char* argv[])
{
	//initializes map for hexToBinary function
	hex['0'] = "0000";
	hex['1'] = "0001";
	hex['2'] = "0010";
	hex['3'] = "0011";
	hex['4'] = "0100";
	hex['5'] = "0101";
	hex['6'] = "0110";
	hex['7'] = "0111";
	hex['8'] = "1000";
	hex['9'] = "1001";
	hex['a'] = "1010";
	hex['b'] = "1011";
	hex['c'] = "1100";
	hex['d'] = "1101";
	hex['e'] = "1110";
	hex['f'] = "1111";

	//get input and output file names
	std::string filename = argv[1];
	std::string outfile = filename + ".result";
	filename.append(".trace");
	std::ifstream inputFile(filename);
	std::ofstream outputFile(outfile);

	//stores each instruction into a vector for easier processing
	std::string word;
	instruction temp;
	std::vector<instruction> instList;
	if (inputFile.is_open())
	{
		while (inputFile >> word)
		{
			
			if (word == "read" || word == "write")
			{
				temp.inst = word;
				
			}
			else if (word.length() > 0)
			{
				temp.binary = hexToBinary(word);
				instList.push_back(temp);
			}
		}

		inputFile.close();
	}
	
	//Loops go through each cache configuration and within each configuration the code processes the instructions for the input file and outputs the results to the console and a output file
	for (int i = 0; i < 4; i++)
	{
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				for (int j = 0; j < 2; j++)
				{
					//These lines of code help with giving the cache appropiate data
					int numOfSets = 0;
					int linesPerSet = 0;
					if (mappingType[y] == "DM")
					{
						numOfSets = cacheSizes[i] / blockSizes[x];
						linesPerSet = 1;
					}
					else if (mappingType[y] == "FA")
					{
						numOfSets = 1;
						linesPerSet = cacheSizes[i] / blockSizes[x];
					}
					else if (mappingType[y] == "2W")
					{
						numOfSets = cacheSizes[i] / (blockSizes[x] * 2);
						linesPerSet = 2;
					}
					else if (mappingType[y] == "4W")
					{
						numOfSets = cacheSizes[i] / (blockSizes[x] * 4);
						linesPerSet = 4;
					}

					//creates the cache for the simulation
					Cache cache = Cache(numOfSets, linesPerSet, writePolicy[j], mappingType[y], blockSizes[x], cacheSizes[i]);

					//this loop goes through each instruction and simulates reading and writing to cache
					for (unsigned int l = 0; l < instList.size(); l++)
					{
						if (cache.cacheType == "DM")
						{

							if (instList[l].inst == "read")
							{
								cache.direct_map_read(instList[l].binary);
							}
							else
							{
								cache.direct_map_write(instList[l].binary);
							}
						}
						else if (cache.cacheType == "FA")
						{
							if (instList[l].inst == "read")
							{
								cache.full_associative_read(instList[l].binary);
							}
							else
							{
								cache.full_associative_write(instList[l].binary);
							}
						}
						else if (cache.cacheType == "2W" || cache.cacheType == "4W")
						{

							if (instList[l].inst == "read")
							{
								cache.set_associative_read(instList[l].binary);
							}
							else
							{
								cache.set_associative_write(instList[l].binary);
							}
						}
					}
					
					//output to both console and file, output to console was to aid in debugging 
					std::cout << cacheSizes[i] << "\t" << blockSizes[x] << "\t" << mappingType[y] << "\t" << writePolicy[j] << "\t" << std::fixed << std::setprecision(2) << cache.hits / instList.size() << "\t" << std::setprecision(0) <<cache.misses*blockSizes[x] << "\t" <<cache.writes * 4 << "\t" << cache.linesPerSet <<"\n";
					outputFile << cacheSizes[i] << "\t" << blockSizes[x] << "\t" << mappingType[y] << "\t" << writePolicy[j] << "\t" << std::fixed << std::setprecision(2) << cache.hits / instList.size() << "\t" << std::setprecision(0) << cache.misses*blockSizes[x] << "\t" << cache.writes * 4 << "\t" << cache.linesPerSet << "\n";
				}
			}
		}
	}

	
	
}

//Implementation of hexToBinary function
std::string hexToBinary(std::string toConvert)
{
	std::string binary;
	for (unsigned int i = 2; i < toConvert.length(); i++)
	{
		binary.append(hex[toConvert[i]]);
	}

	return binary;
}
