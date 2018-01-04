////////////////////////////////////////////////////////////////////
//  Filename     : register-file.v
//  Module       : REGFILE
//  Author       : L. Nazhand-Ali
//  Description  : MIPS register file
//   
//

module REG_FILE
  (
   data1_out,
   data2_out,
   
   clk,    
   readAddr1_in,
   readAddr2_in,
   writeAddr_in,
   writeData_in,
   writeCntrl_in
   );
	
   // Delay to do a register read or write
   parameter reg_delay = 5;	

   input 	 clk;
   input [4:0] 	 readAddr1_in, readAddr2_in, writeAddr_in;
   input [31:0]  writeData_in;	
   input 	 writeCntrl_in;

   output [31:0] data1_out, data2_out;
   reg [31:0] 	 data1_out, data2_out;			
   
   reg [31:0] 	 reg_array[0:31];
   
   initial 
     begin
	reg_array[0] = 32'b0;
     end 
  
   always@(readAddr1_in, readAddr2_in, reg_array[readAddr1_in], reg_array[readAddr2_in])
     begin
	#reg_delay;
	data1_out = reg_array[readAddr1_in];
	data2_out = reg_array[readAddr2_in];
     end
   
   always@(posedge clk)
     begin
	#reg_delay;
	if(writeCntrl_in)
	  begin
	     if(|writeAddr_in)
	       reg_array[writeAddr_in] = writeData_in;
	  end
     end // always@ (posedge clk)
   
endmodule
























