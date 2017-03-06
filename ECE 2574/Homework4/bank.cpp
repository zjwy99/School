///////////////////////////////////////////
// ECE 2574, Homework 4, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	bank.cpp
// Description: Client code to test simulate
// Date: April 14, 2015
//

#include "simulate.h"
using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		//Throws exception if input or output file not provided
		if (argc != 3)
			throw 1;
		ifstream in(argv[1]); //Input file stream
		
		Simulate bank = Simulate(); //Initialize the simulation of the bank

		//Adds customer arrival and transaction times to simulation
		int arrival, trans;
		in >> arrival;
		in >> trans;
		while (!in.fail())
		{
			if (trans == 0||arrival == 0)
				throw 3;
			if (arrival < bank.currenttime)
				throw 2;
			bank.add_arrival(arrival, trans);
			
			bank.update();
			in >> arrival;
			in >> trans;
			
		}

		

		//Continues to process events until there are no more customers or the event list is empty
		while (!bank.line.empty() || !bank.event_list.empty())
			bank.update();
		


		bank.calculate_statistics(); //Calculates statistics for output file
		bank.print();

		//Outputs data to output file
		ofstream out(argv[2]);
		int size = bank.final_list.size();
		out << "t = 0: Simulation begins" << endl;
		for (list<Event>::iterator it = bank.final_list.begin(); it != bank.final_list.end(); it++)
		{
			out << "t = ";
			if (it->arrival == true)
				out << it->arrival_time << ": Arrival event" << endl;
			else
				out << it->depart_time << ": Departure event" << endl;
			
		}
		out << "" << endl;
		out << setprecision(1) << fixed;
		out << "Total number of customers = " << bank.total_customers << endl;
		out << "Average amount of time in the bank = " << bank.bank_time_total / bank.total_customers << " minutes" << endl;
		out << "Maximum amount of time in the bank = " << bank.max_time_in_bank << " minutes" << endl;
		out << "Average amount of time waiting = " << bank.wait_time_total / bank.total_customers << " minutes" << endl;
		out << "Maximum amount of time waiting = " << bank.max_wait_time << " minutes" << endl;
		out << "Maixmum length of line = " << bank.max_line_length << " customers" << endl;
		out.close();
		cin.get();
		return 0;
	}

	//Error Messages
	catch (int err)
	{
		cerr << "Error" << endl;
		if (err == 1)
			cerr << "Input or output file not provided" << endl;
		if (err == 2)
			cerr << "Input file not in expected format" << endl;
		if (err == 3)
			cerr << "Arrival or transaction time cannot be zero" << endl;
		
		cin.get();
		return 0;
	}
}