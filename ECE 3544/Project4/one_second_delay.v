module one_second_delay(clock, enable, out);
	input clock, enable;
	output reg out;
	reg[26:0] counter;
	
	always @ (posedge clock or posedge enable)
	begin
		if (enable) begin
			out <= 0;
			counter <= 0;
		end
		else begin
			if (counter >= 26'd49999999) begin
				out <= 0;
			end
			else begin
				out <= 1;
				counter <= counter + 1;
			end
		end
	end
	
endmodule 