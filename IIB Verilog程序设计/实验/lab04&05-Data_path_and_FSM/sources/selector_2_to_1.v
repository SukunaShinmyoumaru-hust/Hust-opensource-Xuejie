`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 4 and 5 : 2 to 1 Selector
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////

module selector_2_to_1(a, b, sel, out);
  parameter WIDTH = 32;

  input [WIDTH-1:0] a;
  input [WIDTH-1:0] b;
  input sel;
  output [WIDTH-1:0] out;

  assign out = (sel)? a : b;

endmodule