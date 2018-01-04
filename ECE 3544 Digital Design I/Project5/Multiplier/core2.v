`timescale 1ns/100ps
module core2(clock, reset, multiplicand, multiplier, product);
	input clock, reset;
	input [3:0] multiplicand, multiplier;
	output reg[7:0] product;
	
	reg [3:0] A, M, Q;
	reg [2:0] n;
	reg C;
	wire Cout;
	wire [3:0] sum_out;
	wire [8:0] shift_out;
	
	reg [1:0] state, next_state;
	parameter [1:0] init = 2'b00, start = 2'b01, add = 2'b10, shift = 2'b11;
	
	adder ADD(A, multiplicand, C, Cout, sum_out);
	right_shift_ninebits SHIFT({C, A, M}, shift_out);
	
	always @ (posedge clock or negedge reset) begin
		if (~reset)
			state <= init;
		else
			state <= next_state;
	end 
	
	always @ (state) begin
		next_state = 2'bxx;
		case (state)
			init:
				begin
					C = 1'b0;
					A = 4'b0000;
					M = multiplier;
					Q = multiplicand;
					n = 3'd0;
					next_state = start;
				end
			start:
				begin
					n = n + 1;
					if (M[0] == 1) begin
						next_state = add;
					end 
					else 
						next_state = shift;
				end
			add:
				begin
					A = sum_out;
					C = Cout;
					next_state = shift;	
				end 
			shift:
				begin
					C = shift_out[8];
					A = shift_out[7:4];
					M = shift_out[3:0];
					product = {A, M};
					if (n == 4) begin
						#20;
						$stop;
					end
					next_state = start;
				end
		endcase
	end 

endmodule 