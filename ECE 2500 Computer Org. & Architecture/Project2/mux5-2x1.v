////////////////////////////////////////////////////////////////
//  Filename     : mux5-2x1.v
//  Module       : MUX5_2X1
//  Author       : L. Nazhand-Ali
//  Description  : 5-bit wide 2x1 MUX
//

module MUX5_2X1
  (
   value_out,
   value0_in, 
   value1_in, 
   select_in
   );
   
   parameter MUX_delay = 4;	

   output [4:0] value_out;					
   reg [4:0] 	value_out;
   
   input [4:0]  value0_in, value1_in;
   input 	select_in;	
      
   always@*
     begin
	#MUX_delay;
	
	if (~select_in)
	  value_out = value0_in;
	else
	  value_out = value1_in;
     end

endmodule // MUX5_2X1
