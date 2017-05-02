module toplevel_multiplier(clock, reset, mult_in);
	input clock, reset;
	input [3:0] mult_in;
	
	wire load_m, load_q;
	wire [3:0] multiplicand;
	
	multiply_controller CON(clock, reset, load_m, load_q);
	
	multiply_register MULTIPLICAND(clock, reset, load_m, mult_in, multiplicand);
	multiply_register MULTIPLIER(clock, reset, load_q, mult_in, multiplicand);
	

endmodule
