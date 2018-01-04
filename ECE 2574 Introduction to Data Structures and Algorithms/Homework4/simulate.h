///////////////////////////////////////////
// ECE 2574, Homework 4, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	simulate.h
// Description: Header file for the simulate class.
//				simulate has variables and methods to simulate customers in a line at a bank being processed
// Date:	April 14, 2015
//

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <queue> //STL queue header file
#include <list> //STL list header file

using namespace std;

struct Customer
{

	double arrival_time; //time customer arrived in line
	double trans_time; //time it takes for customer to be processed

};


struct Event
{
	double arrival_time; //time customer arrived in line
	double trans_time; //time it takes for customer to be processed
	double depart_time; //time customer departed line
	bool arrival; //Checks whether the event is an arrival or departure

};

class Simulate
{
public:
	queue <Customer> line; //the queue of customers, represents a line in a bank
	list <Event> event_list; //list of events that need to be processed
	list <Event> final_list; //list of events that have already been processed
	
	int total_customers;
	int max_line_length; //Largest number of people that were in line
	double max_time_in_bank; //Largest amount of time a single customer spent in the bank
	double bank_time_total; //Total time all customers spent in the bank
	double wait_time_total; //Total time all customers spent waiting, not being served
	double max_wait_time; //Largest amount of time a customer waited not being served
	double currenttime; //the current time in the simulation
	Simulate(); //default constructor
	void add_arrival(int arrival_time, int trans_time); //Processes arrival events
	void update(); //Updates the line,event_list, and final_list in response to arrival or departure events

	/*	Calculates average time in bank and average amount of wait time. 
		Also, determines the most spent time in the bank and the most spent time waiting.*/
	void calculate_statistics(); 

	void print(); //Prints to command prompt, for debugging

private:
	Event current; //the current event being processed can either be arrival or departure event
	Customer cur_customer; //the customer being processed
	
	void place(Event e); //Puts event where it is suppposed to be chronologically 

};