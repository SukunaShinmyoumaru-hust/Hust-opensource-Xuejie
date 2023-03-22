`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 2.3
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////


module _7Seg_Driver_Choice(SW, SEG, AN, LED);
    input [15:0] SW;       // 16位拨动开关
    output [7:0] SEG;      // 7段数码管驱动，低电平有效
    output [7:0] AN;       // 7段数码管片选信号，低电平有效
    output [15:0] LED;     // 16位LED显示

    decoder_3to8(.in(SW[15:13]), .out(AN[7:0]));
    decoder_4to16(.in(SW[3:0]), .out(SEG[7:0]));
    
    assign LED[15:0] = SW[15:0];
endmodule