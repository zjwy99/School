//ECE 2500, Project 3, Zachary Yee
//ID #: 905796206
//File Name: cache.cpp
//Description: Implementation file for cache class.
//			   IMPORTANT: For a reason I cannot determine the total number of times
//			   transferred from cache to memory is incorrect

#include "cache.h"

//Default Constructor, initializes data
Line::Line()
{
	tag = "";
	valid = false;
	dirty = false;
	LRU = 0;
}

//Constructor initializes data
Set::Set(int linesPerSet)
{
	Line temp = Line();
	
	for (int i = 0; i < linesPerSet; i++)
	{
		temp.LRU = i;
		cacheLines.push_back(temp);
	}

}

//Constructor initializes data and sets up cache for simulation 
Cache::Cache(int numOfSets, int linesperset, std::string policy, std::string type, int blockSize, int cacheSize)
{
	
	
	hits = 0;
	reads = 0;
	writes = 0;
	misses = 0;
	writePolicy = policy;
	cacheType = type;
	linesPerSet = linesperset;
	Set temp = Set(linesPerSet);
	for (int i = 0; i < numOfSets; i++)
	{
		sets.push_back(temp);
	}
	offsetSize = log(blockSize) / log(2);
	indexSize = log(cacheSize / (blockSize*linesPerSet)) / log(2);
	cache_size = cacheSize;
	tagSize = 32 - (log(blockSize) / log(2)) - (log(cacheSize/(blockSize*linesPerSet))/log(2));
}

bool Cache::tagCompare(std::string binary1, std::string binary2)
{
	if (binary1 == binary2)
		return true;
	else
		return false;
}

//For this function to get set index, it gets the index bits in address and converts it to a decimal number 
int Cache::getSetIndex(std::string binary)
{
	
	std::string indexBits = binary.substr(32-(offsetSize+indexSize),indexSize);
	std::bitset<16> indexInBinary (indexBits);
	return indexInBinary.to_ulong();
}

//Functions goes through each line in the set and sees if the valid bit is false
bool Cache::isLineAvailable(int setIndex)
{
	for (int i = 0; i < linesPerSet; i++)
	{
		if (sets[setIndex].cacheLines[i].valid == false)
			return true;
	}
	return false;
}

//Function goes through a set to see if the tag bits in the address match with any of the tag bits in the set
bool Cache::isHit(std::string address, int setIndex)
{
	
	for (int i = 0; i < linesPerSet; i++)
	{
		std::string binary = address.substr(0,tagSize);
		if (tagCompare(sets[setIndex].cacheLines[i].tag, binary) && sets[setIndex].cacheLines[i].valid)
		{
			return true;
		}
	}
	
	return false;
}

//updates the LRU variable 
void Cache::LRU_update(int setIndex, int hitIndex)
{

	
	for (int i = 0; i < linesPerSet; i++)
	{
		if (i != hitIndex)
		{
			if (sets[setIndex].cacheLines[i].LRU>0 && sets[setIndex].cacheLines[i].LRU < sets[setIndex].cacheLines[hitIndex].LRU)
				sets[setIndex].cacheLines[i].LRU -= 1;
		}
		
	}
	sets[setIndex].cacheLines[hitIndex].LRU = linesPerSet - 1;

}

//updates dirty bit by checking tag bits
void Cache::updateDirty(std::string address, int setIndex)
{
	std::string tag = address.substr(0, tagSize);
	for (int i = 0; i < linesPerSet; i++)
	{
		if (tagCompare(tag, sets[setIndex].cacheLines[i].tag))
		{
			sets[setIndex].cacheLines[i].dirty = true;
		}
	}
}

void Cache::direct_map_read(std::string address)
{
	std::string tag = address.substr(0,tagSize);
	int setIndex = getSetIndex(address);

	
	if (isHit(address, setIndex))
		hits++;
	else // if a miss, increments appropiate variables and replaces line in cache
	{
		misses++;
		reads++;
		sets[setIndex].cacheLines[0].valid = true;
		if (writePolicy == "WB" && sets[setIndex].cacheLines[0].dirty)
		{
			writes++;
			sets[setIndex].cacheLines[0].dirty = false;
		}
		sets[setIndex].cacheLines[0].tag = tag;
	}

}

void Cache::full_associative_read(std::string address)
{
	std::string tag = address.substr(0, tagSize);
	if (isHit(address, 0))
		hits++;
	else //if a miss, increments appropiate variables and replaces line in cache
	{
		misses++;
		reads++;

		//if statements determine how the line in the cache is going to be replaced
		int replaceIndex = 0;
		if (isLineAvailable(0)) //checks if there is line available in given set
		{
			
			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[0].cacheLines[i].valid == false)
				{
					replaceIndex = i;
					break;
				}
			}
			
		}
		else
		{

			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[0].cacheLines[i].LRU == 0)
				{
					replaceIndex = i;
					break;
				}
			}
			
			
			
		}
		if (writePolicy == "WB" && sets[0].cacheLines[replaceIndex].dirty)
		{
			writes++;
			sets[0].cacheLines[replaceIndex].dirty = false;
		}
		sets[0].cacheLines[replaceIndex].valid = true;
		sets[0].cacheLines[replaceIndex].tag = tag;
		LRU_update(0, replaceIndex);
	}

}

void Cache::set_associative_read(std::string address)
{
	std::string tag = address.substr(0, tagSize);
	int setIndex = getSetIndex(address);
	if (isHit(address, setIndex))
		hits++;
	else //if a miss, increments appropiate variables and replaces line in cache
	{
		misses++;
		reads++;

		//if statements determine how the line in the cache is going to be replaced
		int replaceIndex = 0;
		if (isLineAvailable(setIndex)) //checks if there is line available in given set
		{

			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[setIndex].cacheLines[i].valid == false)
				{
					replaceIndex = i;
					break;
				}
			}
			
		}
		else
		{

			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[setIndex].cacheLines[i].LRU == 0)
				{
					replaceIndex = i;
					break;
				}
			}
		

			

		}
		if (writePolicy == "WB" && sets[setIndex].cacheLines[replaceIndex].dirty)
		{
			writes++;
			sets[setIndex].cacheLines[replaceIndex].dirty = false;
		}
		sets[setIndex].cacheLines[replaceIndex].valid = true;
		sets[setIndex].cacheLines[replaceIndex].tag = tag;
	}
}

void Cache::direct_map_write(std::string address)
{
	std::string tag = address.substr(0, tagSize);
	int setIndex = getSetIndex(address);
	if (isHit(address, setIndex))
	{
		if (writePolicy == "WT")
			writes++;
		hits++;
		sets[setIndex].cacheLines[0].dirty = true;
	}
	else //if a miss, increments appropiate variables and replaces line in cache
	{
		misses++;
		reads++;
		if (writePolicy == "WT")
			writes++;
		if (writePolicy == "WB" && sets[setIndex].cacheLines[0].dirty)
			writes++;
		sets[setIndex].cacheLines[0].dirty = true;
		sets[setIndex].cacheLines[0].valid = true;
		sets[setIndex].cacheLines[0].tag = tag;
	}
}

void Cache::full_associative_write(std::string address)
{
	std::string tag = address.substr(0, tagSize);
	if (isHit(address, 0))
	{
		hits++;
		if (writePolicy == "WT")
			writes++;
		updateDirty(address, 0);
	}
	else //if a miss, increments appropiate variables and replaces line in cache
	{
		misses++;
		reads++;
		if (writePolicy == "WT")
			writes++;
		int replaceIndex = 0;
		//if statements determine how the line in the cache is going to be replaced
		if (isLineAvailable(0)) //checks if there is line available in given set
		{
			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[0].cacheLines[i].valid == false)
				{
					replaceIndex = i;
					break;
				}
			}
			
			
		}
		else
		{
			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[0].cacheLines[i].LRU == 0)
				{
					replaceIndex = i;
					break;
				}
			}

		}
		if (writePolicy == "WB" && sets[0].cacheLines[replaceIndex].dirty)
		{

			writes++;

		}
		sets[0].cacheLines[replaceIndex].valid = true;
		sets[0].cacheLines[replaceIndex].dirty = true;
		sets[0].cacheLines[replaceIndex].tag = tag;
		LRU_update(0, replaceIndex);
	}
}

void Cache::set_associative_write(std::string address)
{
	std::string tag = address.substr(0, tagSize);
	int setIndex = getSetIndex(address);
	if (isHit(address, setIndex))
	{
		hits++;
		if (writePolicy == "WT")
			writes++;
		updateDirty(address, setIndex);
	}
	else //if a miss, increments appropiate variables and replaces line in cache
	{
		misses++;
		reads++;
		if (writePolicy == "WT")
			writes++;
		int replaceIndex = 0;
		//if statements determine how the line in the cache is going to be replaced
		if (isLineAvailable(setIndex)) //checks if there is line available in given set
		{
			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[setIndex].cacheLines[i].valid == false)
				{
					replaceIndex = i;
					break;
				}
			}
			

		}
		else
		{
			for (int i = 0; i < linesPerSet; i++)
			{
				if (sets[setIndex].cacheLines[i].LRU == 0)
				{
					replaceIndex = i;
					break;
				}
			}
			
			

		}
		if (writePolicy == "WB" && sets[setIndex].cacheLines[replaceIndex].dirty)
			writes++;
		sets[setIndex].cacheLines[replaceIndex].valid = true;
		sets[setIndex].cacheLines[replaceIndex].dirty = true;
		sets[setIndex].cacheLines[replaceIndex].tag = tag;
		LRU_update(setIndex, replaceIndex);
	}
}