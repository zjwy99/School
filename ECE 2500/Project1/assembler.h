///////////////////////////////////////////
//ECE 2500, Project 1, Zachary Yee
//ID #: 905796206
//File Name: assembler.h
//Description: Header file for assembler class.
//			   assembler contains maps and functions to convert instructions to machine code represented as hex values
//			  
//
// Date: 10/2/2015

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <vector> 
#include <map>
#include <bitset>



// A struct for storing various information about a given instruction
struct Instruction
{
	std::string label; //an istruction's label if it has any
	std::string inst; //the instruction mnemonic
	std::string type; //what kind of instruction the instruction is
	std::vector<std::string> tokens; //a vector of the values of the instruction
	int lineNumber; 

};


class Assembler
{
public:
	void addInstruction(Instruction inst); //adds an instruction to the toConvert vector
	void createMachineCode(); //converts the instructions in the toConvert vector to machine code and stores it in the machineCode vector
	Assembler(); //defualt constructor
	void debug(); //for debugging purposes 
	std::vector<std::string>machineCode; //vector to store the converted machine code, machine code is represented in hexadecimal format
private:
	std::vector<Instruction>toConvert; //vector that contains the Instructions that need to be converted
	void assignInstructionTypes(); //sets each Instruction in toConvert to either R or I deppending on what kind of instruction it is
	std::string toHex(std::string binary); //converts a 32-bit binary number in a string format to a hexadecimal value also in string format 
	bool needSecondPass; //bool to see if the instructions contain a beq or bne instruction so that the machine code can be completed
	void secondPass(); //goes through the toConvert vector to complete the beq and bne instructions
	bool registerCheck(std::string reg); //Checks if given string is a register 
	std::string rtypeAssemble(Instruction inst); //converts given R-type instruction to a binary representation 
	std::string itypeAssemble(Instruction inst); //converts given I-type instruction to a binary representation 
	std::map<std::string,std::string>rtypeInsts; //map that contains all the R-type instructions and corresponding function code in binary
	std::map<std::string,std::string>itypeInsts; //map that contains all the I-type instructions and corresponding opcode in binary
	std::map<std::string, std::string> numbers; //map that contains all the register names and their corresponding number in binary
	std::map<std::string, std::string> hex; //map that contains the first 15 binary numbers and their corresponding value in hexadecimal
};