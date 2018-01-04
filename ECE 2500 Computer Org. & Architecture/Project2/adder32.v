////////////////////////////////////////////////////////////////////
//  Filename     : adder32.v
//  Module       : ADDER32
//  Date         : 10/09/06
//  Author       : L. Nazhand-Ali
//  Description  : A 32-bit adder
//

module ADDER32
  (
   result_out, 
   a_in, 
   b_in
   );
   
   parameter adder_delay  = 22;
   
   output [31:0] result_out;				
   reg [31:0] 	 result_out;
   
   input [31:0]  a_in;
   input [31:0]  b_in;

   always@*
     begin
	#adder_delay;

	result_out = a_in + b_in;
     end
   
endmodule
