////////////////////////////////////////////////////////////////////
//  Filename     : data-mem.v
//  Module       : DMEM
//  Author       : L. Nazhand-Ali
//  Description  : data memory
//   
//
//      This module models the MIPS data segement starting at 
//  address 0x10000000 (SPIM default). Please note that we only
//  model the first 32 words of memory. Thus, valid addresses to
//  load and store data words are 0x10000000 to 0x1000007C. This
//  module ignores the lower 2 bits of the input address. Therfore,
//  does not detect wrong alignments for addresses.
//
//     At the begining of simulaiton, this module reads a text file
//  called data.txt which contains the initial values stored in data
//  segment. If the file does not exist, it will simply not initialize
//  the data memory. Therefore, create this file only if your test case
//  needs it. The format should be the same as program.txt
//      

module DMEM 
  (
   data_out,
   
   clk,
   writeCntrl_in,
   readCntrl_in,
   address_in,
   writeData_in   
   );

   // Delay to do a memory read or write
   parameter	 memory_delay = 20;

   output [31:0] data_out;
   reg [31:0] 	 data_out;
   
   input 	 clk;
   input 	 writeCntrl_in;
   input 	 readCntrl_in;
   input [31:0]  address_in;
   input [31:0]  writeData_in;
   
   reg [4:0] 	 word_address;
	 
   // We allocate space for 32 items
   reg [31:0] 	 mem_array[0:31];
   
   // Reading the data words from the file into the array
   initial 
     begin
	$readmemh("data.txt", mem_array, 0, 31);
     end  

   
   always @(address_in, readCntrl_in)
     begin
	#memory_delay;
	
	// word_address = (address_in - 0x00400000) / 4
	word_address= (address_in - 32'h10000000) >> 2;
	
	if ((address_in[31]) | (address_in > 32'h1000007C) | !readCntrl_in)
	  data_out = 32'bxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	else
	  data_out = mem_array[word_address];
	
     end // always @ (address_in, readCntrl_in)
      
   
   always @(posedge clk)
     begin
	#memory_delay;
	
	if (writeCntrl_in)
	  mem_array[word_address] = writeData_in;
	
     end // always @ (posedge clk)
   
endmodule // DMEM


















