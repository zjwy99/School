module test(clock, reset, is_delay, machine_hex, customer_hex, load_machine, load_value_machine, load_customer, load_value_customer,
	product_out, quarter_out, dime_out, nickel_out);

	input clock, reset, is_delay;
	input [23:0] machine_hex, customer_hex;
	
	output reg load_machine, load_customer, product_out, quarter_out, dime_out, nickel_out;
	output reg [23:0] load_value_machine, load_value_customer;
	
	reg [7:0] total;
	
	reg state, next_state;
	parameter [1:0]	INIT = 2'b00,
					COIN = 2'b01,
					DISPENSE = 2'b10,
					CHANGE = 2'b11;
								
	always @ (posedge clock or negedge reset)
		if (~reset)
			state <= INIT;
		else	
			state <= next_state;
	
	always @ (is_delay or machine_hex or customer_hex or state or next_state) begin
		next_state = 2'bxx;
		load_machine = 1'b0;
		load_customer = 1'b0;
		product_out = 1'b0;
		quarter_out = 1'b0;
		dime_out = 1'b0;
		nickel_out = 1'b0;
		load_value_machine = 23'd0;
		load_value_customer = 23'd0;
		total = (customer_hex[23:16] * 5'd25) + (customer_hex[15:8] * 4'd10) + (customer_hex[7:0] * 3'd5);
		
		case (state)
			INIT:
				begin
					next_state = COIN;
				end
			COIN:
				begin
					if (total >= 6'd60) begin
						product_out = 1'b1;
						next_state = DISPENSE;
					end
					else
						next_state = COIN;
				end
			DISPENSE:
				begin
					if (is_delay)
						next_state = DISPENSE;
					else
						next_state = CHANGE;
				end
			CHANGE:
				begin
					total = total - 6'd60;
					next_state = COIN;
				end
			default:
				begin
				end 
		endcase
		
	end
		
	
	
	
endmodule