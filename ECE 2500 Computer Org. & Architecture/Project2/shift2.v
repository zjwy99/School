////////////////////////////////////////////////////////////////////
//  Filename     : shift2.v
//  Module       : SHIFT2
//  Author       : L. Nazhand-Ali
//  Description  : 2-bit shifter to left
//   
//     This module shifts the 32-bit input to the left
//  twice and inserts 0's in the lower 2 bits. Naturally,
//  the value of output is 4 times the value of input.


module SHIFT2(word_out, word_in);
   parameter shift_delay = 5;

   output [31:0] word_out;				   
   reg [31:0] 	 word_out;			

   input [31:0]  word_in;	

   always@*
     begin
	#shift_delay;

	word_out[31:2] = word_in[29:0];
	word_out[1:0]  = 2'b00;
     end

endmodule // SHIFT2















