////////////////////////////////////////////////////////////////
//  Filename     : mips-decode.v
//  Module       : MIPS_DECODE
//  Author       : L. Nazhand-Ali
//  Description  : MIPS decoder
//   
//      This module decodes the instruction into its fields. You 
//  can conisder this as a mdule to help us easier understand the
//  instruction. No combinatinal logic exists in this module. The
//  instruction is wired directly to different fields


module MIPS_DECODE
  (
   instruction_in, 
   op_out, 
   func_out, 
   rs_out, 
   rt_out, 
   rd_out, 
   shft_out, 
   imm16_out, 
   target_out
   );
   
   parameter decode_delay = 1;	
   
   input [31:0]  instruction_in;	

   output [5:0]  op_out, func_out;				
   reg [5:0] 	 op_out, func_out;
   
   output [4:0]  rs_out, rt_out, rd_out, shft_out;
   reg [4:0] 	 rs_out, rt_out, rd_out, shft_out;
   
   output [15:0] imm16_out;
   reg [15:0] 	 imm16_out;
   
   output [25:0] target_out;
   reg [25:0] 	 target_out;		
   
   always@*
     begin
	#decode_delay;	
	op_out     = instruction_in[31:26];
	func_out   = instruction_in[5:0];
	rs_out     = instruction_in[25:21];
	rt_out     = instruction_in[20:16];
	rd_out     = instruction_in[15:11];
	shft_out   = instruction_in[10:6];
	imm16_out  = instruction_in[15:0];
	target_out = instruction_in[25:0];
     end

endmodule // MIPS_DECODE




















