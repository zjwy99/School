///////////////////////////////////////////
//ECE 2500, Project 1, Zachary Yee
//ID #: 905796206
//File Name: myAssembler.cpp
//Description: Main file used to get the instructions from a .asm and converts it into machine code and places result into a .txt file
//			   
//			  
//
// Date: 10/2/2015
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include "assembler.h"
#include "exceptions.h"
#include "lexer.h"
#include "util.h"

std::string read_file(const std::string& name) {
	std::ifstream file(name);
	if (!file.is_open()) {
		std::string error = "Could not open file: ";
		error += name;
		throw std::runtime_error(error);
	}
	std::stringstream stream;
	stream << file.rdbuf();
	return std::move(stream.str());
}

int main(int argc, char** argv) {
	// Adjusting -- argv[0] is always filename.
	--argc;
	++argv;
	Assembler assemble = Assembler();
	if (argc == 0) {
		std::cerr << "Need a file" << std::endl;
		return 1;
	}

	for (int i = 0; i < argc; ++i) {
		std::string asmName(argv[i]);

		if (!util::ends_with_subseq(asmName, std::string(".asm"))) {
			std::cerr << "Need a valid file name (that ends in .asm)" << std::endl;
			std::cerr << "(Bad name: " << asmName << ")" << std::endl;
			return 1;
		}

		// 4 is len(".asm")
		auto length = asmName.size() - string_length(".asm");
		std::string baseName(asmName.begin(), asmName.begin() + length);
		std::string objName = baseName + ".obj";
		try {
			auto text = read_file(asmName);
			try {
				auto lexed = lexer::analyze(text);		// Parses the entire file and returns a vector of instructions
				int num = 0;							//variable used to determine line number
				for (int i = 0; i < (int)lexed.size(); i++){
					Instruction temp = Instruction(); //temporary instruction struct 
					num++;
					if (lexed[i].labels.size() > 0)		// Checking if there is a label in the current instruction
					{
						std::cout << "label = " << lexed[i].labels[0] << "\n";		// Prints the label
						temp.label = lexed[i].labels[0];							//puts label name in temp, if there is a label available
					}
						

					std::cout << "instruction name = " << lexed[i].name << "\n";		// Prints the name of instruction
					temp.inst = lexed[i].name;											//places instruction name in temp
					std::cout << "tokens = ";
					std::vector<lexer::token> tokens = lexed[i].args;
					for (int j = 0; j < (int)tokens.size(); j++){       // Prints all the tokens of this instruction like $t1, $t2, $t3 and puts tokens into temp
						if (tokens[j].type == lexer::token::Integer)
						{
							std::cout << tokens[j].integer() << " ";
							int a = tokens[j].integer();
							temp.tokens.push_back(std::to_string(a));
						}
						else
						{
							std::cout << tokens[j].string() << " ";
							std::string str = tokens[j].string();
							temp.tokens.push_back(str);
						}
					}
					std::cout << num << std::endl;
					temp.lineNumber = num;			//puts line number into temp
					assemble.addInstruction(temp);	//adds temp to a vector for Assembler to convert into machine code
				}

				
				
				assemble.createMachineCode();	//converts instruction vector to machine code and places result into a vector called machineCode
				assemble.debug(); //prints out some debugging info


				std::ofstream out(baseName+".txt");	//Creates output file of the machine code result
				for (int i = 0; i < assemble.machineCode.size(); i++)
				{
					out << assemble.machineCode[i] << std::endl;
					
				}

			}

			catch (const bad_asm& e) {
				std::stringstream error;
				error << "Cannot assemble the assembly code at line " << e.line;
				throw std::runtime_error(error.str());
			}
			catch (const bad_label& e) {
				std::stringstream error;
				error << "Undefined label " << e.what() << " at line " << e.line;
				throw std::runtime_error(error.str());
			}

		}
		catch (const std::runtime_error& err) {
			std::cout << err.what() << std::endl;
			return 1;
		}
	}
	getchar();

	return 0;
}


	
