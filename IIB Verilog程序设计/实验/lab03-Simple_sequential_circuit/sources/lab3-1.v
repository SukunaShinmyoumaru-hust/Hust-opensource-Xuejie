`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 3 - 1
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////


module divider(clk, clk_N);
    input clk;                      // 系统时钟
    output reg clk_N;                   // 分频后的时钟
    parameter N = 100_000_000;     // 1Hz的时钟,N=fclk/fclk_N
    reg [31:0] counter;             /* 计数器变量，通过计数实现分频。当计数器从0计数到(N/2-1)时，输出时钟翻转，计数器清零 */

    initial
    begin                           // 赋初始值
        counter = 0;
        clk_N = 0;
    end
    always @(posedge clk)           // 时钟上升
    begin
        counter <= counter + 1;
        if (counter > N / 2)
            begin
                clk_N <= ~clk_N;
                counter <= 0;
            end
    end
endmodule