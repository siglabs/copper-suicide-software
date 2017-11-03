module cs_rev2_cfg_fpga(
							output wire       led,
							input  wire       CLK
						);


assign led = counter[$bits(counter)-1];

reg [23:0]counter=24'b0;

always @(posedge CLK)
begin
	counter <= counter+1;
end

endmodule

