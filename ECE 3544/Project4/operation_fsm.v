// This module represents the normal mode of the vending machine

module operation_fsm(clock, reset, enable, quarter_count, dime_count, nickel_count, delay, product_out, dime_out, nickel_out, quarter_out);
	input clock, enable, reset, delay;
	input [7:0] quarter_count, dime_count, nickel_count;
	output reg product_out, dime_out, nickel_out, quarter_out;
	reg [7:0] total;
	reg [2:0] state, next_state;
	parameter [2:0]	INIT = 3'b000, 
							COIN_ACCEPT = 3'b001, 
							DISPENSE = 3'b010, 
							CHANGE = 3'b011, 
							DISPENSE_CHANGE = 3'b100;
	
	always @ (posedge clock or negedge reset)
		if (!reset)
			state <= INIT;
		else 
			state <= next_state;
	
	always @ (state or enable or quarter_count or dime_count or nickel_count or delay) begin
		next_state = 3'bxxx;
		total = (quarter_count * 5'd25) + (dime_count * 4'd10) + (nickel_count * 3'd5);
		product_out = 1'b0;
		case (state)
			INIT: 
				begin
					next_state = COIN_ACCEPT;
				end
					
			COIN_ACCEPT:
				begin
					if (total >= 60) begin 
						next_state = DISPENSE;
						product_out = 1'b1;
					end
					else
						next_state = COIN_ACCEPT;
				end	
			DISPENSE:
				begin
					if (delay)
						next_state = DISPENSE;
					else
						next_state = CHANGE;					
				end	
			CHANGE:	
				begin
					if (total - 5'd25 >= 0) begin
						total = total - 5'd25;
						quarter_out = 1'b1;
						dime_out = 1'b0;
						nickel_out = 1'b0;
						next_state = DISPENSE_CHANGE;
					end
					else if (total - 4'd10 >= 0) begin
						total = total - 4'd10;
						quarter_out = 1'b0;
						dime_out = 1'b1;
						nickel_out = 1'b0;
						next_state = DISPENSE_CHANGE;
					end
					else if (total - 3'd5 >= 0) begin
						total = total - 3'd5;
						quarter_out = 1'b0;
						dime_out = 1'b0;
						nickel_out = 1'b1;
						next_state = DISPENSE_CHANGE;
					end
					else 
						next_state = COIN_ACCEPT;
				end
			DISPENSE_CHANGE:	
				begin
					if (delay)
						next_state = DISPENSE_CHANGE;
					else if (total == 0) 
						next_state = COIN_ACCEPT;
					else if (total > 0)
						next_state = CHANGE;
				end
		endcase
		
	end

	
	

endmodule 