///////////////////////////////////////////
// ECE 2574, Homework 4, Zachary Yee
// ID #: 905796206
// CRN: 12433
// File name:	simulate.cpp
// Description: Implementation file for the simulate class
// Date: April 14, 2015
//

#include "simulate.h"


Simulate::Simulate()
{
	total_customers = 0;
	max_line_length = 0;
	bank_time_total = 0;
	max_wait_time = 0;
	wait_time_total = 0;
}


void Simulate::update()
{
	current = event_list.front(); 
	
	//Processes arrival events
	if (current.arrival == true) 
	{
		currenttime = current.arrival_time; //the current time in simulation is now the arrival time of event

		//Creates new Customer to put in the line
		Customer p;
		p.arrival_time = current.arrival_time;
		p.trans_time = current.trans_time;
		line.push(p);

		event_list.pop_front(); //Arrival event is done being processed, so it is removed from the list

		//Puts processed event into final list, if final list is empty place event at the begining else put at the end of the list
		if (final_list.empty() == true)
		{
			final_list.push_front(current);
		}

		else
		{
			final_list.push_back(current);
		}

		//Creates depature event for customer if there is only one customer in the line
		if (line.size() == 1)
		{
			cur_customer = line.front();
			Event temp;
			temp.arrival_time = cur_customer.arrival_time;
			temp.trans_time = cur_customer.trans_time;
			temp.depart_time = currenttime + current.trans_time;
			temp.arrival = false;
			place(temp);
		}
		

	}

	//Processes departure events
	else
	{
		currenttime = current.depart_time; //Changes the current time in the simulation to the departure time of the event
		line.pop(); //Since the event being processed is a departure event, that customer will be removed from the line
		event_list.pop_front(); //Event is done being processed, so it is removed from the list
		final_list.push_back(current); //Departure event is now placed in the final list

		//Checks to see if more customers need to be processed
		if (line.size() > 0)
		{
			cur_customer = line.front(); //A new customer is now being processed

			//Creates new departure event and places event into event list
			Event temp;
			temp.arrival_time = cur_customer.arrival_time;
			temp.trans_time = cur_customer.trans_time;
			temp.depart_time = currenttime + cur_customer.trans_time;
			temp.arrival = false;
			place(temp);
		}
		

	}

	//Determines if the maximum number of people in the line has increased
	if (max_line_length < line.size())
		max_line_length = line.size();

}

void Simulate::add_arrival(int arrival_time,int trans_time)
{
	total_customers++;

	//Creates a new event represents arrivals
	Event e;
	e.arrival_time = arrival_time;
	e.trans_time = trans_time;
	e.depart_time = 0;
	e.arrival = true;

	//Updates the simulation so that the only unprocessed event is the new arrival
	while (event_list.size() > 1)
		update();
	
	//Places the event at start of event list, if the list is empty
	if (event_list.empty() == true)
	{
		event_list.push_front(e);
	}

	else
	{

		//Puts the event in the proper place in the list
		Event e2;
		e2 = event_list.front();
		if (e.arrival_time < e2.depart_time)
			event_list.push_front(e);
		else if (e.arrival_time > e2.depart_time)
			event_list.push_back(e);
		else if (e.arrival_time == e2.depart_time)
		{
			update();
			event_list.push_front(e);
		}

	}

	

}

void Simulate::calculate_statistics()
{
	//Iterates through final_list to determine various statistics
	for (list<Event>::iterator it = final_list.begin(); it != final_list.end(); it++)
	{
		if (it->arrival == false)
		{
			double bank_time = it->depart_time - it->arrival_time; //Total time a customer spent in the bank 
			bank_time_total = bank_time_total + bank_time;  
			if (bank_time > max_time_in_bank)
				max_time_in_bank = bank_time;
			double wait_time = bank_time - it->trans_time;
			wait_time_total = wait_time_total + wait_time;
			if (wait_time > max_wait_time)
				max_wait_time = wait_time;

		}

	}


}

void Simulate::place(Event e)
{
	double d;
	if (e.arrival)
		d = e.arrival_time;
	else
		d = e.depart_time;
	bool c = false;
	for (list<Event>::iterator it = event_list.begin(); it != event_list.end(); it++)
	{
		double check;

		if (it->arrival)
			check = it->arrival_time;
		else
			check = it->depart_time;
		if (check > d)
		{
			event_list.insert(it, e);
			c = true;
		}

	}
	if (c == false)
		event_list.push_back(e);


}

//outputs to command prompt, for debugging 
void Simulate::print()
{
	int size = final_list.size();
	cout << "t = 0: Simulation begins" << endl;
	for (list<Event>::iterator it = final_list.begin(); it != final_list.end(); it++)
	{
		cout << "t = ";
		if (it->arrival == true)
			cout << it->arrival_time << ": Arrival event" << endl;
		else
			cout << it->depart_time << ": Departure event" << endl;

	}

	cout << "" << endl;
	cout << setprecision(1) << fixed;
	cout << "Total number of customers = " << total_customers << endl;
	cout << "Average amount of time in the bank = " << bank_time_total / total_customers << " minutes" << endl;
	cout << "Maximum amount of time in the bank = " << max_time_in_bank << " minutes" << endl;
	cout << "Average amount of time waiting = " << wait_time_total / total_customers << " minutes" << endl;
	cout << "Maximum amount of time waiting = " << max_wait_time << " minutes" << endl;
	cout << "Maixmum length of line = " << max_line_length << " customers" << endl;


}