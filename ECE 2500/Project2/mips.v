////////////////////////////////////////////////////////////////
//  Filename     : mips.v
//  Module       : MIPS
//  Author       : L. Nazhand-Ali
//  Description  : single cycle MIPS 
//   
//     The top module of the single cycle MIPS is presented in this
//  file. You need to edit this module to complete the data path for
//  some of the instructions in your project.


module MIPS(clk, reset);

   input clk;
   input reset;

   // instruction and PC related wires
   wire [31:0] instruction;
   wire [31:0] PCplus4;
   wire [31:0] PC;
   
   // decoder related wires
   wire [5:0]  op, func;
   wire [4:0]  rs, rt, rd, shft;
   wire [15:0] imm16;
   wire [25:0] target;
   
   // control related wires
   wire        regWrite, regDst;
   wire        memRead, memWrite, memToReg;
   wire        extCntrl, ALUSrc;
   wire [3:0]  ALUCntrl;
   wire        branch, jump;

   // ALU related wires
   wire [31:0] A, B, ALUout;
   wire        zero;

   // register file related wires
   wire [31:0] regData2;
   wire [4:0]  regDstAddr;

   // immediate related wires
   wire [31:0] immExtended;

   
   // data memory related wires
	wire[31:0] dataMemOut;
	wire[31:0] dataMemMuxOut;
	
   // branching program counter wires
	wire[31:0] shiftLeft_out,branchAdd_out, branchMux_out;
	wire branchControl,orGate_out;
   
   // wires needed for various jumping instructions
	wire[31:0] shiftLeftJump_out,jumpMux_out,jumpRegisterMux_out,PCplus8, jumpLinkWriteMux_out;
	wire[31:0] jumpAddress;
	wire[4:0] jumpLinkRegMUX_out;
	wire jumpRegisterControl;
	wire jumpLinkRegControl;
   
   //////////////////////////////////////////////
   

   // instantiation of instruction memory
   IMEM	imem
     (
      .instruction_out(instruction),
      .address_in(PC)
      );


   // instantiation of register file
   REG_FILE reg_file
     (
      .clk(clk),
      .data1_out(A),
      .data2_out(regData2),
      .readAddr1_in(rs),
      .readAddr2_in(rt),
      .writeAddr_in(jumpLinkRegMUX_out),
      .writeData_in(jumpLinkWriteMux_out),
      .writeCntrl_in(regWrite)
      );

   // instantiation of PC register
   PC_REG pc_reg
     (
      .clk(clk),
      .reset(reset),
      .PC_out(PC),
      .PC_in(jumpRegisterMux_out)
      );

   // instantiation of the decoder
   MIPS_DECODE	mips_decode
     (
      .instruction_in(instruction), 
      .op_out(op), 
      .func_out(func), 
      .rs_out(rs), 
      .rt_out(rt), 
      .rd_out(rd), 
      .shft_out(shft), 
      .imm16_out(imm16), 
      .target_out(target)
      );

   // instantiation of the control unit
   MIPS_CONTROL mips_control
     (
      .op_in(op),
      .func_in(func),
      .branch_out(branch), 
      .regWrite_out(regWrite), 
      .regDst_out(regDst), 
      .extCntrl_out(extCntrl), 
      .ALUSrc_out(ALUSrc), 
      .ALUCntrl_out(ALUCntrl), 
      .memWrite_out(memWrite),
      .memRead_out(memRead),
      .memToReg_out(memToReg), 
      .jump_out(jump)
      );

   // instantiation of the ALU
   MIPS_ALU mips_alu
     (
      .ALUCntrl_in(ALUCntrl), 
      .A_in(A), 
      .B_in(B), 
      .ALU_out(ALUout), 
      .zero_out(zero)
      );
	  
   // instantiation of the sign/zero extender
   EXTEND extend
     (
      .word_out(immExtended),
      .halfWord_in(imm16),
      .extendCntrl_in(extCntrl)
      );

   // instantiation of a 32-bit adder used for computing PC+4
   ADDER32 plus4Adder
     (
      .result_out(PCplus4),
      .a_in(32'd4), 
      .b_in(PC)
      );

   // instantiation of a 32-bit MUX used for selecting between immediate and register as the second source of ALU
   MUX32_2X1 aluMux
     (
      .value_out(B),
      .value0_in(regData2), 
      .value1_in(immExtended), 
      .select_in(ALUSrc)
      );

   // instantiation of a 5-bit MUX used for selecting between RT or RD as the destination address of the operation
   MUX5_2X1 regMUX 
     (
      .value_out(regDstAddr),
      .value0_in(rt),
      .value1_in(rd),
      .select_in(regDst)
      );
	  
	// determines if the instruction is jump register
	and andgate3(jumpLinkRegControl,jump,instruction[26]);
	
   // instantiation of 5-bit MUX used to select between regDstAddr and register 31
   MUX5_2X1 jumpLinkRegMUX
     (
	  .value_out(jumpLinkRegMUX_out),
	  .value0_in(regDstAddr),
	  .value1_in(5'b11111),
	  .select_in(jumpLinkRegControl)
	 );

    //instantiation of the data memory
   DMEM data_memory
	 (
	  .clk(clk),
	  .writeCntrl_in(memWrite),
	  .readCntrl_in(memRead),
	  .address_in(ALUout),
	  .writeData_in(regData2),
	  .data_out(dataMemOut)
	 );
	 
	// initializes PCplus8 for jump and link instruction
	assign PCplus8=PCplus4 + 4'b0100;

	//instantiation Data memory mux
	 MUX32_2X1 dataMemMux
     (
      .value_out(dataMemMuxOut),
      .value0_in(ALUout), 
      .value1_in(dataMemOut), 
      .select_in(memToReg)
      );
	
	//instantiation
	 MUX32_2X1 jumpLinkWriteMux
	 (
	  .value_out(jumpLinkWriteMux_out),
	  .value0_in(dataMemMuxOut),
	  .value1_in(PCplus8),
	  .select_in(jumpLinkRegControl)
	 );
	  
	// instantiation of shift left 2 
	SHIFT2 shiftLeft_2
	(
	  .word_out(shiftLeft_out),
	  .word_in(immExtended)
	);
	
	// instantiation of 32-bit adder used for computing branch destination
	ADDER32 branchAdder
	(
	  .result_out(branchAdd_out),
	  .a_in(PCplus4),
	  .b_in(shiftLeft_out)
	);
	
	// determines if branching should occur
	and andgate1(branchControl, branch, orGate_out);
	xor orgate1(orGate_out,zero,instruction[26]);
	// instantiation of branching mux
	MUX32_2X1 branchMux
	(
	  .value_out(branchMux_out),
	  .value0_in(PCplus4),
	  .value1_in(branchAdd_out),
	  .select_in(branchControl)
	);
	
	//instantaion of shift left 2 for jump instruction
	SHIFT2 shiftLeftJump
	(
	  .word_out(shiftLeftJump_out),
	  .word_in({4'b0000,instruction[25:0]})
	);
	
	assign jumpAddress={PCplus4[31:28],shiftLeftJump_out[25:0]};
	
	MUX32_2X1 jumpMux
	(
	  .value_out(jumpMux_out),
	  .value0_in(branchMux_out),
	  .value1_in(jumpAddress),
	  .select_in(jump)
	);
	
	and andgate2(jumpRegisterControl,jump,~instruction[27]);
	
	MUX32_2X1 jumpRegisterMux
	(
	  .value_out(jumpRegisterMux_out),
	  .value0_in(jumpMux_out),
	  .value1_in(A),
	  .select_in(jumpRegisterControl)
	);
	
endmodule