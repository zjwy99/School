module multiply_register(clock, reset, enable, ins, outs);
	input clock, reset, enable;
    input [3:0] ins;
    output [3:0] outs;
	reg [3:0] outs;
	
	always @ (posedge clock or negedge reset) begin
		if (~reset)
			outs <= 4'b0000;
	end
	
	always @ (posedge enable) begin
		if (enable) begin
			outs = ins;
		end
	end
	
	
endmodule
