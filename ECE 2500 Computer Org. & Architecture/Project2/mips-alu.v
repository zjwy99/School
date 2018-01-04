////////////////////////////////////////////////////////////////
//  Filename     : mips-alu.v
//  Module       : MIPS_ALU
//  Author       : L. Nazhand-Ali
//  Description  : MIPS ALU 
//   
//     This module implements a simplified version of MIPS ALU.
//  The ALUCntrl_in functions are specified on page 261 of P&H book.
//  In addition to the operatins mentioned in the book, this ALU 
//  implements lui instruction with the 1111 control input.

module MIPS_ALU
  (
   ALUCntrl_in, 
   A_in, 
   B_in, 
   ALU_out, 
   zero_out
   );
   
   parameter ALU_delay = 22;
   parameter zero_delay = 3;
   
   output [31:0] ALU_out;
   reg [31:0] 	 ALU_out;
   
   output 	 zero_out;
   reg 		 zero_out;
   
   input [3:0] 	 ALUCntrl_in;
   input [31:0]  A_in, B_in;	

   reg [31:0] 	 temp;
   
   always@*
     begin
	#ALU_delay;
		
	case(ALUCntrl_in)
	  4'b0000:  // and
	    ALU_out = A_in & B_in;
	  
	  4'b0001:  // or
	    ALU_out = A_in | B_in;
	  
	  4'b0010:  // add
	    ALU_out = A_in + B_in;
	  
	  4'b0110:  // sub
	    ALU_out = A_in - B_in;
	  
	  4'b0111:  // slt
	    begin
	       temp = A_in - B_in;
	       if(temp[31]) 
		 ALU_out = 32'd1;
	       else
		 ALU_out = 32'd0;  		
	    end
	  
	  4'b1100:  // nor
	    ALU_out = ~(A_in | B_in);
	  
	  4'b1111:  // lui
	    ALU_out = {B_in[15:0], 16'b0};
	  
	  default:  // all other control values
	    ALU_out = 32'bx;
	  
	endcase // case(ALUCntrl_in)
	
     end // always@ *
   
   always@*
     begin
	#zero_delay;
	
	if (~(|ALU_out)) 
	  zero_out = 1;
	else	
	  zero_out = 0;
     end
   
endmodule




