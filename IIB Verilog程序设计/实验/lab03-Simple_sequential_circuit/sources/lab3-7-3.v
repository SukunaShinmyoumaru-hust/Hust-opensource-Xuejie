`timescale 1ns / 1ps



module divide_and_count(clk, out);
    input clk;
    output [2:0] out;
    wire clk_tmp;

    divider d1(.clk(clk), .clk_N(clk_tmp));
    counter c1(.clk(clk_tmp), .out(out[2:0]));
endmodule