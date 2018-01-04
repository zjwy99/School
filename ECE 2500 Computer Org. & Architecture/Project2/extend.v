////////////////////////////////////////////////////////////////////
//  Filename     : extend.v
//  Module       : EXTEND
//  Author       : L. Nazhand-Ali
//  Description  : immediate extender
//   
//     This module is slightly different from a simple sign-extender.
//  It has an input control line, extendCntrl_in. When the control
//  is set (i.e. 1), the module sign extends, otherwise it zero extends
//  the half word to 32 bits.
// 


module EXTEND
  (
   word_out,
   halfWord_in, 
   extendCntrl_in
   );
   
   parameter     ext_delay = 3;	

   output [31:0] word_out;				
   reg [31:0] 	 word_out, temp;
   
   input [15:0]  halfWord_in ;	
   input 	 extendCntrl_in;
   
   always @*
     begin
	#ext_delay;
	if (extendCntrl_in & (halfWord_in[15])) 
	  word_out = {16'hffff, halfWord_in};
	else
	  word_out = {16'b0, halfWord_in};
     end
   
endmodule














