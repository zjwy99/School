// Filename:    sevensegdecoder_assign.v
// Author:       Zachary Yee
// Date:         24 Feburary 2017
// Version:      1
// Description: Continuous assignment version of the seven segment decoder.  
//               
//  
module sevensegdecoder_assign(digit, hex_driver);
   input  [3:0] digit;
   output [6:0] hex_driver;

   assign hex_driver = (digit == 4'b0000) ? 7'b1000000 :
					   (digit == 4'b0001) ? 7'b1111001 :
                  (digit == 4'b0010) ? 7'b0100100 :
					   (digit == 4'b0011) ? 7'b0110000 :
					   (digit == 4'b0100) ? 7'b0011001 :
					   (digit == 4'b0101) ? 7'b0010010 :
					   (digit == 4'b0110) ? 7'b0000010 :
					   (digit == 4'b0111) ? 7'b1111000 :
					   (digit == 4'b1000) ? 7'b0000000 :
					   (digit == 4'b1001) ? 7'b0010000 :
					   (digit == 4'b1010) ? 7'b0001000 :
					   (digit == 4'b1011) ? 7'b0000011 :
					   (digit == 4'b1100) ? 7'b1000110 :
					   (digit == 4'b1101) ? 7'b0100001 :
					   (digit == 4'b1110) ? 7'b0000110 :
					   (digit == 4'b1111) ? 7'b0001110 : 7'b1111111;
endmodule
