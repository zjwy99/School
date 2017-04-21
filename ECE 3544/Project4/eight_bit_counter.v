////////////////////////////////////////////////////////////////////////////////
// Filename:    eight_bit_counter.v
// Author:      Zachary Yee
// Date:        04/21/2017
// Version:     1
// Description: This is an 8 bit counter based on the 74163 counter 
//
/////////////////////////////////////////////////////////////////////////////////

module eight_bit_counter(clock, clr_l, ld_l, enp, ent, ins, state);
	input clock, clr_l, ld_l, enp, ent;
	input  [7:0] ins;  
	output [7:0] state;
	
	reg [7:0] state;
	
	always @ (posedge clock) begin
		if (~clr_l)
			state <= 8'b00000000;
		else if (~ld_l)
			state <= ins;
		else if (ent && enp)
			state <= state + 1'b1;
	end

endmodule