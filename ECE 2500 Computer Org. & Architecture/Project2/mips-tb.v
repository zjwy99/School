////////////////////////////////////////////////////////////////////
//  Filename     : mips-tb.v
//  Module       : MIPS_TB
//  Date         : 10/09/06
//  Author       : L. Nazhand-Ali
//  Description  : MIPS testbench 
//   
//     This is a simple testbench for the designed MIPS.
//  To increase the simulation time, either increase the
//  10000 below or remove the $finish line and use breakpoints
//  in your simulation.

module MIPS_TB ();

   reg clk;
   reg reset;

   MIPS	myMIPS(clk, reset);

   always	#100	clk = ~clk;

   initial 
     begin
	clk = 0;
	reset = 1;
	#400;
	reset = 0;

	// change this value to increase or decrease your simulation time
	// pressing the play button will simulate the testbench for 10000ns
	// and you do not need to set up a breakpoint anymore.
	
	#10000;
	$finish;
     end

endmodule