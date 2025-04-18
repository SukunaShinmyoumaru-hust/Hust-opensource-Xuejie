`timescale 1ns / 1ps


module divider(clk, clk_N, SW);
    input clk;
    input [2:0]SW;         
    output reg clk_N;               
    integer N= 100_000_000;
    reg [31:0] counter;
    
                                     
    always @(posedge clk) begin 
        case(SW) 
            3'b010: N = 1_000;
            default: N = 100_000_000;
        endcase
            counter<=counter+1;                      
            if(counter>=(N/2)-1) begin
                clk_N<=~clk_N;
                counter<=0;
        end
    end                           
endmodule
