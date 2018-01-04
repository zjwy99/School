module LED_handler(dime_count, nickel_count, product, dime, nickel, out);
	input [7:0] dime_count, nickel_count;
	input product, dime, nickel;
	output reg [7:0] out;
	
	always @(*) begin
		if (dime_count == 8'd0 && nickel_count == 8'd0) begin
			out[3] <= 1'b1;
		end
		else begin
			out[3] <= 1'b0;
		end
		if (product) begin
			out[2] <= 1'b1;
		end
		else begin
			out[2] <= 1'b0;
		end
		if (dime) begin
			out[1] <= 1'b1;
		end
		else begin
			out[1] <= 1'b0;
		end
		if (nickel) begin
			out[0] <= 1'b1;
		end
		else begin
			out[0] <= 1'b0;
		end
	end
	
endmodule