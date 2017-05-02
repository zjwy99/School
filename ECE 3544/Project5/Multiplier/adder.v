
module adder(A, B, Cin, C, sum);
	input Cin;
	input [3:0] A, B;
	output C;
	output [3:0] sum;
	
	assign {C, sum} = A + B + Cin; 

endmodule