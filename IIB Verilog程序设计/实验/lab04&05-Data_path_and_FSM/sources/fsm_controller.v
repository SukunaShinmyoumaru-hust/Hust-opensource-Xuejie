`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// HUST Verilog Experiment 4 and 5 FSM controller
//
// Pan Yue
//
//////////////////////////////////////////////////////////////////////////////////

module fsm_controller(clk, rst, start, next_zero, ld_sum, ld_next, sum_sel, next_sel, a_sel, done);
  parameter START = 4'b1000;
  parameter COMPUTE_SUM = 4'b0100;
  parameter GET_NEXT = 4'b0010;
  parameter DONE = 4'b0001;

  input clk;
  input rst, start;
  input next_zero;
  output ld_sum, ld_next;
  output sum_sel, next_sel;
  output a_sel;
  output done;

  reg [3:0] state;
  reg [5:0] output_value;

  assign {ld_sum, ld_next, sum_sel, next_sel, a_sel, done} = output_value;

initial begin
  state <= START;
  output_value <= 6'b000000;
end

always @(posedge clk) begin
  if (rst) begin
    state <= START;
    output_value <= 6'b000000;
  end
  else begin
    case (state)
      START:begin
        if (start == 0) begin
          state <= START;
          output_value <= 6'b000000;
        end
        else if (start == 1) begin
          state <= COMPUTE_SUM;
          output_value <= 6'b101110;
        end
      end
      COMPUTE_SUM:begin
        state <= GET_NEXT;
        output_value <= 6'b011100;
      end
      GET_NEXT:begin
        if (next_zero == 0) begin
          state <= COMPUTE_SUM;
          output_value <= 6'b101110;
        end
        else if (next_zero == 1) begin
          state <= DONE;
          output_value <= 6'b000001;
        end
      end
      DONE:begin
        if (start == 0) begin
          state <= START;
          output_value <= 6'b000000;
        end
        else if (start == 1) begin
          state <= DONE;
          output_value <= 6'b000001;
        end
      end
  	endcase
  end
end
endmodule