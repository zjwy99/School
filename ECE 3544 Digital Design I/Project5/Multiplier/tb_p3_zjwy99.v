`timescale 1ns/100ps
module tb_p3_zjwy99();
	reg clock, reset;
	reg [3:0] mult_in;
	wire m, q;
	wire [7:0] result;
	// My clock module 
	reg clk_en;
	always #20 
		if(clk_en) begin 
		clock = ~clock;
	end 
	
	
	//core_mult CORE(clock, reset, 4'd14, 4'd13, result, q);
	core2 CORE(clock, reset, 4'd6, 4'd14, result);
	
	initial begin
	reset = 1'b1;
	clk_en = 1'b1;
	clock = 1'b0;
	#5;
	reset = 1'b0;
	#5;
	reset = 1'b1;
	#200;
	//$stop;
	end
	


endmodule
