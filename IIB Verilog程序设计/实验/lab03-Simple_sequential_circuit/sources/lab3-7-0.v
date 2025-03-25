`timescale 1ns / 1ps



module dynamic_scan(clk, SEG, AN, SW);
    input clk;
    input [2:0] SW;
    output [7:0] SEG;  		
    output [7:0] AN;
    wire clk_n;
    wire [2:0]count;
    wire [7:0]code;
    divider(.clk(clk), .clk_N(clk_n), .SW(SW));
    counter(.clk(clk_n), .out(count[2:0]), .SW(SW));
    decoder_3to8(.in(count[2:0]), .out(AN[7:0]));
    rom8x4(.addr(count[2:0]), .data(code[3:0]));
    decoder_4to16(.in(code[3:0]), .out(SEG[7:0]));
endmodule

