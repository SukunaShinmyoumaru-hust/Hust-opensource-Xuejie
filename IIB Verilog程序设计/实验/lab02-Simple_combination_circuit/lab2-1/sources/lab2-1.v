`timescale 1ns / 1ps

module _7Seg_Driver_Direct(SW, CA, CB, CC, CD, CE, CF, CG, DP, AN, LED);
    input [15:0] SW;                          ?
	output CA, CB, CC, CD, CE, CF, CG, DP;    
	output [7:0] AN;                          
	output [15:0] LED;                        

	assign {CA, CB, CC, CD, CE, CF, CG, DP} = SW[7:0];
	assign AN[7:0] = SW[15:8];
	assign LED[15:0] = SW[15:0];
endmodule
