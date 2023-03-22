`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 4 and 5
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////

module topset(clk, rst, start, Done, SEG, AN);
  input clk, rst, start;
  output Done;
  output [7:0] SEG;
  output [7:0] AN;

  wire clk_N1, clk_N2;
  wire [31:0]sum_out;
  wire ld_sum, ld_next;
  wire sum_sel, next_sel;
  wire a_sel;
  wire next_zero;

  divider Divider1(.clk(clk), .frequency(1), .clk_N(clk_N1));
  divider Divider2(.clk(clk), .frequency(2000), .clk_N(clk_N2));

  show_numbers SHOW(.clk(clk_N2), .sum_out(sum_out), .AN(AN), .SEG(SEG));

  fsm_controller FSM1(
    .clk(clk_N1), 
    .rst(rst), 
    .start(start), 
    .next_zero(next_zero), 
    .ld_sum(ld_sum), 
    .ld_next(ld_next), 
    .sum_sel(sum_sel), 
    .next_sel(next_sel), 
    .a_sel(a_sel), 
    .done(Done));

  data_path_top DST1(
    .clk(clk_N1), 
    .rst(rst), 
    .ld_sum(ld_sum), 
    .ld_next(ld_next), 
    .sum_sel(sum_sel), 
    .next_sel(next_sel), 
    .a_sel(a_sel), 
    .next_zero(next_zero), 
    .sum_out(sum_out));
endmodule