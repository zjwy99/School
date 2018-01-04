/////////////////////////////////////////////////////////////////////
//  Filename     : instruction-mem.v
//  Module       : IMEM
//  Author       : L. Nazhand-Ali
//  Description  : instruction memory
//   
//
//      This module models the MIPS text segement starting at 
//  address 0x00400000 (SPIM default). Please note that we only
//  model the first 32 words of instruction memory. Thus, valid 
//  addresses to load instructions are 0x00400000 to 0x0040007C.
//
//     At the begining of simulaiton, this module reads a text file
//  called program.txt which contains the instructions to be executed
//  in the hex format. Note that 0x should be removed from the 
//  instructions, otherwise an error will occure when reading the file.
//  Copy your test cases to program.txt to simulate them.

module IMEM
  (
   address_in, 
   instruction_out
   );

   // Delay to do a memory read or write
   parameter memory_delay = 20;	
   
   input [31:0]  address_in;
   
   output [31:0] instruction_out;
   reg [31:0] 	 instruction_out;			
   
   reg [4:0] 	 word_address;
   
   // We allocate space for 32 instructions in our model
   reg [31:0] 	 mem_array[0:31];

   // Reading the instructions from the file into the array
   initial begin
      $readmemh("program.txt", mem_array, 0, 31);
   end
   
   always @(address_in)
     begin
	#memory_delay;
	
	// word_address = (address_in - 0x00400000) / 4
	word_address= (address_in - 32'h00400000) >> 2;
	
	if ((address_in[31]) | (address_in > 32'h40007C))
	  instruction_out = 32'bxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	else
	  instruction_out = mem_array[word_address];
	
     end // always @(address_in)
   
endmodule // IMEM















