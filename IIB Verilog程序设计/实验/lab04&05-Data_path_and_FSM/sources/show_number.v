`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 4 and 5 Show numbers
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////

module show_numbers(clk, sum_out, AN, SEG);
  input clk;
  input [31:0] sum_out;
  output reg [7:0]AN;
	output reg [7:0]SEG;

  wire [7:0] tube[7:0];
  reg [7:0]tube_now;

  decoder_4to16 Decoder1(.in(sum_out[3:0]), .out(tube[0]));
  decoder_4to16 Decoder2(.in(sum_out[7:4]), .out(tube[1]));
  decoder_4to16 Decoder3(.in(sum_out[11:8]), .out(tube[2]));
  decoder_4to16 Decoder4(.in(sum_out[15:12]), .out(tube[3]));
  decoder_4to16 Decoder5(.in(sum_out[19:16]), .out(tube[4]));
  decoder_4to16 Decoder6(.in(sum_out[23:20]), .out(tube[5]));
  decoder_4to16 Decoder7(.in(sum_out[27:24]), .out(tube[6]));
  decoder_4to16 Decoder8(.in(sum_out[31:28]), .out(tube[7]));

initial 
    begin
	    AN = 8'b11111110;
	end
	
always @(posedge clk)
  begin
	  case(AN)
		  8'b11111110:begin
			  AN <= 8'b11111101;
			  tube_now <= tube[2];
      end
      8'b11111101:begin
			  AN <= 8'b11111011;	
			  tube_now <= tube[3];
      end
      8'b11111011:begin
			  AN <= 8'b11110111;
			  tube_now <= tube[4];
      end
      8'b11110111:begin
			  AN <= 8'b11101111;
			  tube_now <= tube[5];
      end
      8'b11101111:begin
			  AN <= 8'b11011111;
			  tube_now <= tube[6];
      end
      8'b11011111:begin
			  AN <= 8'b10111111;
			  tube_now <= tube[7];
      end
      8'b10111111:begin
			  AN <= 8'b01111111;
			  tube_now <= tube[0];
      end
      default:begin
			  AN <= 8'b11111110;		
			  tube_now <= tube[1];
      end
    endcase
    SEG <= tube_now; 		
	end
endmodule