`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 3 - 2
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////


module counter(clk, out);
    input clk;                    // 计数时钟
    output reg [2:0] out;         // 计数值

    always @(posedge clk)         // 在时钟上升沿计数器加1
    begin
        out <= out + 1;
    end
endmodule