module test(clock, reset, machine_hex, customer_hex, load_machine, load_value_machine, load_customer, load_value_customer,
	product_out, quarter_out, dime_out, nickel_out);

	input clock, reset;
	input [23:0] machine_hex, customer_hex;
	
	output load_machine, load_customer, product_out, quarter_out, dime_out, nickel_out;
	output [23:0] load_value_machine, load_value_customer;
	
	reg state, next_state;
	parameter [1:0]	INIT = 2'b00,
							COIN = 2'b01,
							DISPENSE = 2'b10;
								
								
	always @ (posedge clock or negedge reset)
		if (~reset)
			state <= INIT;
		else	
			state <= next_state;
	
	
endmodule