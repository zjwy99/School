///////////////////////////////////////////
//ECE 2500, Project 1, Zachary Yee
//ID #: 905796206
//File Name: assembler.cpp
//Description: implementation file for assembler.h
//			   
//			  
//
// Date: 10/2/2015
#include "assembler.h"

//implementation of deafult constructor
Assembler::Assembler()
{
	//implementation of the rtypeInsts map, map has all the r-type instructions and corresponding function values in binary
	rtypeInsts["add"] = "100000";
	rtypeInsts["addu"] = "100001";
	rtypeInsts["and"] = "100100";
	rtypeInsts["jr"] = "001000";
	rtypeInsts["nor"] = "100111";
	rtypeInsts["or"] = "100101";
	rtypeInsts["slt"] = "101010";
	rtypeInsts["sltu"] = "101011";
	rtypeInsts["sll"] = "000000";
	rtypeInsts["srl"] = "000010";
	rtypeInsts["sub"] = "100010";
	rtypeInsts["subu"] = "100011";

	//implementation of the itypeInsts map, map has all the i-type instructions and corresponding opcode values in binary
	itypeInsts["addi"] = "001000";
	itypeInsts["addiu"] = "001001";
	itypeInsts["andi"] = "001100";
	itypeInsts["beq"] = "000100";
	itypeInsts["bne"] = "000101";
	itypeInsts["lbu"] = "100100";
	itypeInsts["lhu"] = "100101";
	itypeInsts["ll"] = "110000";
	itypeInsts["lui"] = "001111";
	itypeInsts["lw"] = "100011";
	itypeInsts["ori"] = "001101";
	itypeInsts["slti"] = "001010";
	itypeInsts["sltiu"] = "001011";
	itypeInsts["sb"] = "101000";
	itypeInsts["sh"] = "101001";
	itypeInsts["sc"] = "111000";
	itypeInsts["sw"] = "101011";

	//implementation of the numbers map, map has all the register names with corresponding values in binary
	numbers["$zero"] = "00000";
	numbers["$at"] = "00001";
	numbers["$v0"] = "00010";
	numbers["$v1"] = "00011";
	numbers["$a0"] = "00100";
	numbers["$a1"] = "00101";
	numbers["$a2"] = "00110";
	numbers["$a3"] = "00111";
	numbers["$t0"] = "01000";
	numbers["$t1"] = "01001";
	numbers["$t2"] = "01010";
	numbers["$t3"] = "01011";
	numbers["$t4"] = "01100";
	numbers["$t5"] = "01101";
	numbers["$t6"] = "01110";
	numbers["$t7"] = "01111";
	numbers["$s0"] = "10000";
	numbers["$s1"] = "10001";
	numbers["$s2"] = "10010";
	numbers["$s3"] = "10011";
	numbers["$s4"] = "10100";
	numbers["$s5"] = "10101";
	numbers["$s6"] = "10110";
	numbers["$s7"] = "10111";
	numbers["$t8"] = "11000";
	numbers["$t9"] = "11001";
	numbers["$k0"] = "11010";
	numbers["$k1"] = "11011";
	numbers["$gp"] = "11100";
	numbers["$sp"] = "11101";
	numbers["$fp"] = "11110";
	numbers["$ra"] = "11111";

	//implementation of hex map, has the binary values 
	hex["0000"] = "0";
	hex["0001"] = "1";
	hex["0010"] = "2";
	hex["0011"] = "3";
	hex["0100"] = "4";
	hex["0101"] = "5";
	hex["0110"] = "6";
	hex["0111"] = "7";
	hex["1000"] = "8";
	hex["1001"] = "9";
	hex["1010"] = "a";
	hex["1011"] = "b";
	hex["1100"] = "c";
	hex["1101"] = "d";
	hex["1110"] = "e";
	hex["1111"] = "f";

	needSecondPass = false;

}

//debuging method
void Assembler::debug()
{
	for (int i = 0; i < machineCode.size(); i++)
	{
		std::cout << machineCode[i] << std::endl;
	}
}

//Method to convert given r-type instruction into binary
std::string Assembler::rtypeAssemble(Instruction inst)
{
	
	try
	{
		std::bitset<5> temp; //tempoarary bitset used to help convert decimal and hexadecimal tokens into binary 

		if (inst.tokens.size() == 0 || inst.tokens.size()>3) //checks for correct amount of tokens and throws exception if necessary
			throw 0;
		if (registerCheck(inst.tokens[0])==false) //checks if the first token is a register
			throw 0;
		
		if (inst.inst == "jr")
		{
			return "000000" + numbers[inst.tokens[0]] + "000000000000000" + rtypeInsts[inst.inst];
		}

		if (inst.tokens.size() != 3) //checks for correct amount of tokens and throws exception if necessary
			throw 0;
		if (registerCheck(inst.tokens[1]) == false) //checks if the second token is a register
			throw 0;
		if (inst.inst == "sll" || inst.inst == "srl")
		{

			int temp2 = std::stoi(inst.tokens[2]);
			temp = std::bitset<5>(temp2);
			return "00000000000" + numbers[inst.tokens[1]] + numbers[inst.tokens[0]] + temp.to_string() + rtypeInsts[inst.inst];
		}

		if (registerCheck(inst.tokens[2]) == false) //checks if the third token is a register
			throw 0;

		return "000000" + numbers[inst.tokens[1]] + numbers[inst.tokens[2]] + numbers[inst.tokens[0]] + "00000" + rtypeInsts[inst.inst];

	}
	catch (int err)
	{

		std::cerr << "Cannot assemble the assembly code at line " << inst.lineNumber << std::endl;
		exit(EXIT_FAILURE);
	}

}

//Method to convert given i-type instructions into binary
std::string Assembler::itypeAssemble(Instruction inst)
{
	try
	{

		if (inst.tokens.size() == 0) //checks for correct amount of tokens and throws exception if necessary
			throw 0;

		if (inst.tokens.size() == 3)
		{
			if (inst.tokens[2][0] == '(')
			{
				inst.tokens[2].erase(4, 1);
				inst.tokens[2].erase(0, 1);
			}
		}

		

		if (registerCheck(inst.tokens[0]) == false) //checks if the first token is a register
			throw 0;

		if (inst.inst == "lui")
		{
			if (inst.tokens.size() != 2) //checks for correct amount of tokens and throws exception if necessary
				throw 0;
			std::bitset<16>temp(std::stoi(inst.tokens[1]));
			return itypeInsts[inst.inst] + "00000" + numbers[inst.tokens[0]] + temp.to_string();
		}

		if (inst.tokens.size() != 3) //checks for correct amount of tokens and throws exception if necessary
			throw 0;

		
		if (inst.inst == "addi" || inst.inst == "addiu" || inst.inst == "andi" || inst.inst == "ori" || inst.inst == "slti" || inst.inst == "sltiu")
		{
			if (registerCheck(inst.tokens[1]) == false) //checks if the second token is a register
				throw 0;
			std::bitset<16>temp(std::stoi(inst.tokens[2]));
			return itypeInsts[inst.inst] + numbers[inst.tokens[1]] + numbers[inst.tokens[0]] + temp.to_string();
		}

		if (inst.inst == "beq" || inst.inst == "bne") //since this for the first pass conversion, the code for beq and bne instruction is not complete
		{
			if (registerCheck(inst.tokens[1]) == false) //checks if the second token is a register
				throw 0;
			needSecondPass = true;
			return itypeInsts[inst.inst] + numbers[inst.tokens[0]] + numbers[inst.tokens[1]];
		}

		

		if (inst.inst == "lb" || inst.inst == "lbu" || inst.inst == "lh" || inst.inst == "lhu" || inst.inst == "lw" || inst.inst == "sb" || inst.inst == "sc" || inst.inst == "sh" || inst.inst == "sw" || inst.inst == "ll")
		{
			if (registerCheck(inst.tokens[2]) == false) //checks if the third token is a register
				throw 0; 
			std::bitset<16>temp(std::stoi(inst.tokens[1]));
			return itypeInsts[inst.inst] + numbers[inst.tokens[2]] + numbers[inst.tokens[0]] + temp.to_string();
		}

		


	}
	catch (int err)
	{
		std::cerr << "Cannot assemble the assembly code at line " << inst.lineNumber << std::endl;
		exit(EXIT_FAILURE);
	}
	
}

//places given instruction into toConvert vector
void Assembler::addInstruction(Instruction inst)
{
	toConvert.push_back(inst);
}

//Goes through each instruction in toConverts vector and assigns each Instruction as a r-type or i-type instruction. if it cannot assign a type then it throws an exception
void Assembler::assignInstructionTypes()
{
	for (int i = 0; i < toConvert.size(); i++)
	{
		if (rtypeInsts.find(toConvert[i].inst) != rtypeInsts.end())
		{
			toConvert[i].type = "R";
		}
	}

	for (int i = 0; i < toConvert.size(); i++)
	{
		if (itypeInsts.find(toConvert[i].inst) != itypeInsts.end())
		{
			toConvert[i].type = "I";
		}
	}
	try
	{
		for (int i = 0; i < toConvert.size(); i++)
		{
			if (toConvert[i].type == "")
				throw i;
		}
	}
	
	catch (int err)
	{
		std::cerr << "Cannot assemble the assembly code at line " << toConvert[err].lineNumber << std::endl;
		exit(EXIT_FAILURE);
	}

}

//Gets the offset for the branch instructions and finishes those instructions 
void Assembler::secondPass()
{

	
	try
	{
		bool check = false; // bool to aid in checking if the label in the given beq or bne exists
		if (needSecondPass == true)
		{
			for (int i = 0; i < toConvert.size(); i++)
			{
				if (toConvert[i].inst == "beq" || toConvert[i].inst == "bne")
				{
					int difference;
					for (int x = 0; x < toConvert.size(); x++)
					{
						if (toConvert[x].label == toConvert[i].tokens[2])
						{
							check = true; //since the label was found, check changes to true
							difference = toConvert[x].lineNumber - toConvert[i].lineNumber - 1;
						}
					}

					//throws an exception if the label given was not found
					if (check == false)
						throw i;

					std::bitset<16> temp(difference);
					machineCode[i] += temp.to_string();
				}
			}
		}
	}
	catch (int err)
	{
		
			std::cerr << "Undefined label " << toConvert[err].tokens[2] << " at line " << toConvert[err].lineNumber <<std::endl;
			exit(EXIT_FAILURE);
		
	}
}

//converts instructions to machine code
void Assembler::createMachineCode()
{
	assignInstructionTypes();
	for (int i = 0; i < toConvert.size(); i++)
	{
		Instruction temp = toConvert[i];
		if (toConvert[i].type=="R")
		{
			
			machineCode.push_back(rtypeAssemble(temp));
		}
		else
		{
			machineCode.push_back(itypeAssemble(temp));
		}
		
	}

	secondPass();

	for (int i = 0; i < machineCode.size(); i++)
	{
		machineCode[i] = toHex(machineCode[i]);
	}
}


//Converts a given 32-bit binary number into hexadecimal format
std::string Assembler::toHex(std::string binary)
{
	std::string one = binary.substr(0,4);
	std::string two = binary.substr(4, 4);
	std::string three = binary.substr(8, 4);
	std::string four = binary.substr(12, 4);
	std::string five = binary.substr(16, 4);
	std::string six = binary.substr(20, 4);
	std::string seven = binary.substr(24, 4);
	std::string eight = binary.substr(28, 4);
	
	return hex[one]+hex[two]+hex[three]+hex[four]+hex[five]+hex[six]+hex[seven]+hex[eight];
}


bool Assembler::registerCheck(std::string reg)
{
	if (numbers.find(reg) == numbers.end())
	{
		return false;
	}

	return true;
}