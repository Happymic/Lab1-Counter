module counter #(
    parameter WIDTH = 8
)(
    // interface signals
    input logic clk, //clock
    input logic rst, //reset
    input logic en, //enable signal
    output logic signed [WIDTH-1:0] count //count output
);
/* Original Counter Logic, Only Count Up
always_ff @ (posedge clk)
    if (rst) count <= {WIDTH{1'b0}};
    else count <= count + {{WIDTH-1{1'b0}},en};

endmodule
*/
always_ff @ (posedge clk or posedge rst) begin
    if (rst) begin
        count <= 0;  // 复位时将计数器置零
    end else if (en) begin
        count <= count + 5;  // en为高时，增加
    end else begin
        count <= count - 1;  // en为低时，减少
    end
end
endmodule