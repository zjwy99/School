module test(clock, enable, customer_hex, out);
	input clock, enable;
	input [23:0] customer_hex;
	output reg out;
	
	reg[7:0] quarters, dimes, nickels;
	
	always @ (customer_hex) begin
		quarters <= customer_hex[23:16];
		dimes <= customer_hex[15:8];
		nickels <= customer_hex[7:0];
	end 
	
endmodule