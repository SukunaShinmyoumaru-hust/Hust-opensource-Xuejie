`timescale 1ns / 1ps


module counter(clk, out, SW);
    input clk;
    input [2:0] SW;
    output reg [2:0] out;
    
    always @(posedge clk)  begin
        case(SW)
            3'b100: out <= out+1;
            3'b001: out <= out-1;
            3'b000: out <= out;
            default: out<=out-1;
        endcase
    end
endmodule
