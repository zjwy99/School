///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filename:    buttonpressed.v
// Author(s):	 Tom Martin, Jason Thweatt
// Date:        24 October 2013
// Version:     3 (28 March 2017)
// Description: This FSM generates a one clock-cycle output pulse each time a pushbutton is pressed and released.
//              The pushbuttons on your board output 0 when PRESSED and 1 when NOT PRESSED.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

module buttonpressed(clock, reset, button_in, pulse_out);
	input        clock;			// The system clock. (Connect to an FPGA clock signal in the top-level module.)
	input	       reset;			// Active-low reset. (Connect to the RESET pushbutton in the top-level module.)
	input        button_in;		// Input pushbutton signal. (Connect to some other pushbutton in the top-level module.)
	output       pulse_out;		// The output is high for one clock cycle each time the button is pressed and released.
	reg          pulse_out;
	reg    [1:0] state;			// The module's present state.
	reg    [1:0] next_state;	// The module's next state.

	// Set up parameters for the state of the pushbutton.
	// Since there are three states, we are using two bits to represent the state in a "dense" assignment.
	parameter [1:0] BUTTON_FREE = 2'b00, BUTTON_PRESSED = 2'b01, BUTTON_RELEASED = 2'b10;
	
	// REGISTER BLOCK	: This always block represents sequential logic, so it uses non-blocking assignments.
	// It is sensitized to appropriate edges of the clock input and the reset input.
	// You should picture this block as a 2-bit register with an active-low asynchronous clear.
	
	always@(posedge clock or negedge reset) begin
		// If reset = 0, there must have been a negative edge on the reset.
		// Since the effect of the reset occurs in the absence of a clock pulse, the reset is ASYNCHRONOUS.
		if(reset == 1'b0)
			state <= BUTTON_FREE;
		// If reset is not zero but this always block is executing, there must have been a positive clock edge.
		// On each positive clock edge, the next state becomes the present state.
		else
			state <= next_state;
	end

	// REGISTER LOGIC: This always block represents combinational logic, so it uses blocking assignments.
	// It is sensitized to changes in the pushbutton's present state and the pushbutton input.
	// You should picture this block as the combinational logic that feeds the register inputs.
	// It determines the next state based on the current state and the enable input.
	
	always@(state or button_in) begin
		// To be safe, assign a default value to next_state.
		// That way, if none of the paths in the case statement apply, the variable will have a known value.
		// This will be overridden by one of the assignments in the case structure below.
		next_state = state;
		// Use the present state to determine the next state.
		case(state)
			// BUTTON_FREE is the state where the pushbutton is currently unpressed and was not just released.
			// In this state, if the button value is 0, make the next state BUTTON_PRESSED.
			BUTTON_FREE: begin
				if(button_in == 1'b0)
					next_state = BUTTON_PRESSED;
			end
			// BUTTON_PRESSED is the state where the pushbutton is currently pressed down.
			// In this state, if the button value is 1, make the next state BUTTON_RELEASED.
			BUTTON_PRESSED: begin
				if(button_in == 1'b1)
					next_state = BUTTON_RELEASED;
			end
			// BUTTON_RELEASED is the state where the pushbutton has just been released.
			// In this state, make the next state BUTTON_FREE.
			// Note that this state makes its transition independent of the input value.
			// This state lasts for exactly one clock cycle - the clock cycle right after the button was released.
			BUTTON_RELEASED: next_state = BUTTON_FREE;
			// If none of the above - something that should never happen - make the next state unknown.
			default: next_state = 2'bxx;
		endcase
	end

	// OUTPUT MACHINE: This always block represents combinational logic, so it uses blocking assignments.
	// This is a Moore output, so the block is sensitized only to the state.
	// In a Mealy machine, the output would have been sensitized to a state and to an input.
	// You should picture this block as a combinational circuit that operates on the state to determine the output.

	// I have structured the output machine as an always block to provide an example of how you should do it in general. 
	// The behavior of this output is simple enough that a continuous assignment could have been used more effectively.

	always@(state) begin
		// Assign a default value to pulse_out. This will be overridden by one of the assignments below.
		pulse_out = 1'b0;			
		// Use the present state to determine the output.
		case(state)
			// If the pushbutton is currently unpressed and was not just released, pulse_out should be 0.
			BUTTON_FREE:		pulse_out = 1'b0;
			// If the pushbutton is currently being pressed, pulse_out should be 0.
			BUTTON_PRESSED:	pulse_out = 1'b0;
			// If the pushbutton has has just been released, pulse_out should be 1.
			// This state only lasts for one clock cycle, so pulse_out is 1 for only one clock cycle.
			BUTTON_RELEASED:	pulse_out = 1'b1;
			// If none of the above - something that should never happen - make the output unknown.
			default:				pulse_out = 1'bx;	
		endcase
	end
endmodule
