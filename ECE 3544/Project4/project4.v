////////////////////////////////////////////////////////////////////////////////
// You should modify this header with your name, date, etc. and then delete
// this comment.
//
// Filename: project4.v
// Author:   Jason Thweatt
// Date:     07 April 2017
// Revision: 1
//
// Description:
// This is the top level module for ECE 3544 Project 4.
// Do not modify the module declarations or ports in this file.
// When synthesizing to the DE1-SoC, this file should be used with the provided
// Quartus project so that the FPGA pin assignments are made correctly.
//
// YOU MUST MAKE THE PIN ASSIGNMENTS FOR THE INPUTS AND OUTPUTS OF THIS MODULE.
// FOLLOW THE INSTRUCTIONS THAT YOU USED IN PROJECT 3.
// CONSULT THE MANUAL FOR INFORMATION ON THE PIN LOCATIONS.
//
// Modified for use with the DE1-SoC
	
module project4(CLOCK_50, KEY, SW, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0, LED);
	
//==============================================================================
// PORT declarations: Do NOT modify these declarations.
//==============================================================================

// You must use this clock for the clock of all of your sequential hardware.

	input        CLOCK_50;										// This is the system clock.
	input  [1:0] KEY;												// KEY[1] should be used to enter operation codes and operands.
																		// KEY[0] should be used as reset, active low.
																		// The KEY pushbuttons are 1 when not pressed and 0 when pressed.
	input  [2:0] SW;												// SW[2] controls the mode.
																		// SW[1:0] represents a coin input.
	output [6:0] HEX5, HEX4, HEX3, HEX2, HEX1, HEX0;	// The hex display outputs drive the seven segment displays.
	output [7:0] LED;												// The LED output drives the LEDs.

// You should add your reg/wire/parameter declarations here.
	wire enable;
	wire   [23:0] hexDigits, customer_hexDigits;
	wire [3:0] coin_enable, customer_counter_enable;
	
	wire product_out, dime_out, nickel_out;
	
	wire delay_out, delay_in;
	wire [3:0] delay_enable;
//==============================================================================
// Module instantiantions: You should add your module instantions here.
//==============================================================================
	buttonpressed U1(.clock(CLOCK_50), .reset(KEY[0]), .button_in(KEY[1]), .pulse_out(enable));
	
	decoder_2_to_4 SWITCH(.enable(enable), .in(SW[1:0]), .out(coin_enable));
	eight_bit_counter QUARTER_COUNTER(.clock(CLOCK_50), .clr_l(KEY[0]), .ld_l(1'b1), .enp(coin_enable[3]), .ent(1'b1), .ins(8'b00000000), .state(hexDigits[23:16]));
	eight_bit_counter DIME_COUNTER(.clock(CLOCK_50), .clr_l(KEY[0]), .ld_l(1'b1), .enp(coin_enable[2]), .ent(1'b1), .ins(8'b00000000), .state(hexDigits[15:8]));
	eight_bit_counter NICKEL_COUNTER(.clock(CLOCK_50), .clr_l(KEY[0]), .ld_l(1'b1), .enp(coin_enable[1]), .ent(1'b1), .ins(8'b00000000), .state(hexDigits[7:0]));
	
	and CUSTOMER_ENABLE1(customer_counter_enable[3], SW[2], coin_enable[3]);
	and CUSTOMER_ENABLE2(customer_counter_enable[2], SW[2], coin_enable[2]);
	and CUSTOMER_ENABLE3(customer_counter_enable[1], SW[2], coin_enable[1]);
	eight_bit_counter QUARTER_COUNTER(.clock(CLOCK_50), .clr_l(KEY[0]), .ld_l(1'b1), .enp(customer_counter_enable[3]), .ent(1'b1), .ins(8'b00000000), .state(customer_hexDigits[23:16]));
	eight_bit_counter DIME_COUNTER(.clock(CLOCK_50), .clr_l(KEY[0]), .ld_l(1'b1), .enp(customer_counter_enable[2]), .ent(1'b1), .ins(8'b00000000), .state(customer_hexDigits[15:8]));
	eight_bit_counter NICKEL_COUNTER(.clock(CLOCK_50), .clr_l(KEY[0]), .ld_l(1'b1), .enp(customer_counter_enable[1]), .ent(1'b1), .ins(8'b00000000), .state(customer_hexDigits[7:0]));

	
	
	one_second_delay DELAY(.clock(CLOCK_50), .enable(delay_in), .out(delay_out));
//	operation_fsm VENDING(.clock(CLOCK_50), .reset(KEY[0]), .enable(enable), 
//		.quarter_count(customer_hexDigits[23:16]), .dime_count(customer_hexDigits[15:8]), 
//		.nickel_count(customer_hexDigits[7:0]), .delay(delay_out), .product_out(delay_enable[3]), 
//		.dime_out(delay_enable[2]), .nickel_out(delay_enable[1]), .quarter_out(delay_enable[0]));
	
	//or DELAY_OR(delay_in, delay_enable[3], delay_enable[2], delay_enable[1], delay_enable[0]);
	
	
	sevensegdecoder_assign Q1(.digit(hexDigits[23:20]), .hex_driver(HEX5));
	sevensegdecoder_assign Q2(.digit(hexDigits[19:16]), .hex_driver(HEX4));
	sevensegdecoder_assign D1(.digit(hexDigits[15:12]), .hex_driver(HEX3));
	sevensegdecoder_assign D2(.digit(hexDigits[11:8]), .hex_driver(HEX2));
	sevensegdecoder_assign N1(.digit(hexDigits[7:4]), .hex_driver(HEX1));
	sevensegdecoder_assign N2(.digit(hexDigits[3:0]), .hex_driver(HEX0));
	
	//LED_handler handler(.dime_count(hexDigits[15:8]), .nickel_count(hexDigits[7:0]), .product(delay_out), .dime(delay_out), .nickel(delay_out), .out(LED[3:0]));
	
// 

					  
endmodule
