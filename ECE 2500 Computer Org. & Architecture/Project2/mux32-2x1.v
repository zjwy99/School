////////////////////////////////////////////////////////////////
//  Filename     : mux32-2x1.v
//  Module       : MUX32_2X1
//  Author       : L. Nazhand-Ali
//  Description  : 32-bit wide 2x1 MUX
//

module MUX32_2X1
  (
   value_out,
   value0_in, 
   value1_in, 
   select_in
   );
   
   parameter MUX_delay = 4;	

   output [31:0] value_out;
   reg [31:0] 	 value_out;		
	
   input [31:0]  value0_in, value1_in;
   input 	 select_in;	

   always@*
     begin
	#MUX_delay;
	
	if (~select_in)
	  value_out = value0_in;
	else
	  value_out = value1_in;
     end

endmodule // MUX32_2X1
