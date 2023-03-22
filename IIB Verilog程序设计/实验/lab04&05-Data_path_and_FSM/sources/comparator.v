`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 4 and 5 Comparator
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////

module comparator(A, B, is_equal, is_great, is_less);
	parameter WIDTH = 32;

	input [WIDTH-1:0] A, B;
	output is_equal, is_great, is_less;

	assign is_equal = (A == B) ? 1'b1 : 1'b0;
	assign is_great = (A > B) ? 1'b1 : 1'b0;
	assign is_less = (A < B) ? 1'b1 : 1'b0;
endmodule