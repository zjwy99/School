// Active High Decoder

module decoder_2_to_4(enable, in, out);
	input [1:0] in;
	input enable;
	
	output reg [3:0] out;
	
	always @ (in or enable) begin
		if (enable) begin
			case (in)
				2'b00: out = 4'b0001;
				2'b01: out = 4'b0010;
				2'b10: out = 4'b0100;
				2'b11: out = 4'b1000;
				default: out = 4'bxxxx;
			endcase
		end
		else begin
			out = 3'b0000;
		end
	
	end
	
endmodule