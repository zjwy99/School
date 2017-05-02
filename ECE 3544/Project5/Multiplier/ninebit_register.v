module ninebit_register(clock, reset, enable, ins, outs);
	input clock, reset, enable;
	input [8:0] ins;
	output [8:0] outs;
	reg [8:0] outs;
	
	always @ (posedge clock or negedge reset) begin
		if (~reset)
			outs <= 9'd0;
	end
	
	always @ (posedge enable) begin
		if (enable) begin
			outs = ins;
		end
	end
	
	
endmodule