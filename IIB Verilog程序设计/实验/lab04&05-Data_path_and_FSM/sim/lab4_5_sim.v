`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 4 and 5 Simulation
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////


module test_module;
  reg clk;
  reg rst, start;
  wire ld_sum, ld_next, sum_sel, next_sel, a_sel, next_zero, done;

  fsm_controller FSM1(
    .clk(clk), 
    .rst(rst), 
    .start(start), 
    .next_zero(next_zero), 
    .ld_sum(ld_sum), 
    .ld_next(ld_next), 
    .sum_sel(sum_sel), 
    .next_sel(next_sel), 
    .a_sel(a_sel), 
    .done(done));
    
  data_path_top DST1(
    .clk(clk), 
    .rst(rst), 
    .ld_sum(ld_sum), 
    .ld_next(ld_next), 
    .sum_sel(sum_sel), 
    .next_sel(next_sel), 
    .a_sel(a_sel), 
    .next_zero(next_zero));

initial begin
  clk <= 0;
  rst <= 0;
  start <= 0;
  #50 start <= 1;
end

always begin
  #20 clk <= ~clk;
end

endmodule
