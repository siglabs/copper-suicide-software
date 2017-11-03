module cs_rev2_cfg_fpga(
							output wire [15:0]PROG_N,
							output wire       led,
							input  wire       CLK
						);

parameter TEST=16'b1111111111111111;

assign PROG_N = TEST;
assign led = counter[$bits(counter)-1];

reg [23:0]counter=24'b0;

always @(posedge CLK)
begin
	counter <= counter+1;
end

endmodule

