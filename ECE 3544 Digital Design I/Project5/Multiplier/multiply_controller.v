module multiply_controller(clock, reset,load_m_out, load_q_out);
	input clock, reset;
	output load_m_out, load_q_out;
	reg load_m_out, load_q_out;
	
	reg [1:0] state, next_state;
	parameter [1:0] start = 2'b00, load_multiplicand = 2'b01, load_multiplier = 2'b10; 
			
	always @ (posedge clock or negedge reset) begin
		if (~reset)
			state <= start;
		else
			state <= next_state;
	end
	

	always @ (state) begin
		next_state = 2'bxx;
		load_m_out = 1'b0;
		load_q_out = 1'b0;
		case (state)
			start:
				begin
					load_m_out = 1'b0;
					load_q_out = 1'b0;
					next_state = load_multiplicand;
				end
			load_multiplicand:
				begin
					load_m_out = 1'b1;
					load_q_out = 1'b0;
					next_state = load_multiplier;
				end
			load_multiplier:
				begin
					load_m_out = 1'b0;
					load_q_out = 1'b1;
				end
		endcase
	end

endmodule
