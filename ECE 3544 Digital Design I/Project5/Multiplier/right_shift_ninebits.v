module right_shift_ninebits(in, out);
	input [8:0] in;
	output [8:0] out;
	
	assign out = in >> 1;

endmodule
