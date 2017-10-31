module FPGA10(
	input  wire 	  CLK,
	input  wire [70:0]IL07,
	input  wire [70:0]IL68,
	output wire [70:0]IL12,
	output wire [70:0]IL34,
	output wire  [3:0]error_dect_50,
	output wire  [3:0]error_dect_125,
	output wire 	led
	);
	
wire clk_50,clk125;
sys_pll __ (.CLKI(CLK), .CLKOP(clk_50), .CLKOS(clk125), .LOCK(led));


reg [45:0]IL12_125=0;
reg [24:0]IL12_50=0;

reg [45:0]IL34_125=0;
reg [24:0]IL34_50=0;

reg [45:0]IL07_125=0;
reg [24:0]IL07_50=0;

reg [45:0]IL68_125=0;
reg [24:0]IL68_50=0;

reg [1:0]error_dect_IL07=0;
reg [1:0]error_dect_IL12=0;
reg [1:0]error_dect_IL34=0;
reg [1:0]error_dect_IL68=0;

assign IL12={IL12_125,IL12_50};
assign IL34={IL34_125,IL34_50};
assign error_dect_50  = {error_dect_IL07[0], error_dect_IL12[0], error_dect_IL34[0], error_dect_IL68[0]};
assign error_dect_125 = {error_dect_IL07[1], error_dect_IL12[1], error_dect_IL34[1], error_dect_IL68[1]};

reg [1:0]data_rx_flag_IL07 = 2'b00;
reg [2:0]counter_IL07_50   = 3'b001;
reg [2:0]counter_IL07_125  = 3'b001;

reg [1:0]data_rx_flag_IL68 = 2'b00;
reg [2:0]counter_IL68_50   = 3'b001;
reg [2:0]counter_IL68_125  = 3'b001;

reg [2:0]counter_50  = 3'b000; // For 50Mhz SDR pins  - IL12 & IL34
reg [2:0]counter_125 = 3'b000; // For 125Mhz DDR pins - IL12 & IL34

always @(posedge clk_50)
begin
	IL07_50 <= IL07[24:0];
	if(IL07_50 == 25'haaaaaaaaaaaaaaaaaa)
		data_rx_flag_IL07[0] <= 1'b1;
end

always @(posedge clk_50)
begin
	if(data_rx_flag_IL07[0]==1'b1) 
	begin
		case(counter_IL07_50)
			3'b000:	begin
						if(IL07_50==25'haaaaaaaaaaaaaaaaaa)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;
							counter_IL07_50<=3'b001;			
					end
			3'b001:	begin
						if(IL07_50==25'h555555555555555555)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;		
							counter_IL07_50<=3'b010;
					end						
			3'b010:	begin
						if(IL07_50==25'h0f0f0f0f0f0f0f0f0f)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;		
							counter_IL07_50<=3'b011;
					end	
			3'b011:	begin
						if(IL07_50==25'hf0f0f0f0f0f0f0f0f0)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;
							counter_IL07_50<=3'b100;
					end	
			3'b100:	begin
						if(IL07_50==25'h000000000000000000)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;
							counter_IL07_50<=3'b101;		
					end	
			3'b101:	begin
						if(IL07_50==25'hffffffffffffffffff)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;
							counter_IL07_50<=3'b110;							
					end
			3'b110:	begin
						if(IL07_50==25'hf5a0f5a0f5a0f5a0f5)
							error_dect_IL07[0]<=1'b0;
						else
							error_dect_IL07[0]<=1'b1;
							counter_IL07_50<=3'b000;							
					end	

			default: begin
						error_dect_IL07[0] <= 0;
					end
		endcase
	end	
end

always @(posedge clk125)
begin
	IL07_125 <= IL07[70:25];
	if(IL07_125 == 46'haaaaaaaaaaaaaaaaaa)
		data_rx_flag_IL07[1] <= 1'b1;
end

always @(posedge clk125)
begin
	if(data_rx_flag_IL07[1]==1'b1) 
	begin
		case(counter_IL07_125)
			3'b000:	begin
						if(IL07_125==46'haaaaaaaaaaaaaaaaaa)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;
							counter_IL07_125<=3'b001;			
					end
			3'b001:	begin
						if(IL07_125==46'h555555555555555555)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;		
							counter_IL07_125<=3'b010;
					end						
			3'b010:	begin
						if(IL07_125==46'h0f0f0f0f0f0f0f0f0f)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;		
							counter_IL07_125<=3'b011;
					end	
			3'b011:	begin
						if(IL07_125==46'hf0f0f0f0f0f0f0f0f0)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;
							counter_IL07_125<=3'b100;
					end	
			3'b100:	begin
						if(IL07_125==46'h000000000000000000)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;
							counter_IL07_125<=3'b101;		
					end	
			3'b101:	begin
						if(IL07_125==46'hffffffffffffffffff)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;
							counter_IL07_125<=3'b110;							
					end
			3'b110:	begin
						if(IL07_125==46'hf5a0f5a0f5a0f5a0f5)
							error_dect_IL07[1]<=1'b0;
						else
							error_dect_IL07[1]<=1'b1;
							counter_IL07_125<=3'b000;							
					end	

			default: begin
					error_dect_IL07[1] <= 0;
			end
		endcase
	end	
end

always @(posedge clk_50)
	begin
		IL68_50 <= IL68[24:0];
		if(IL68_50 == 25'haaaaaaaaaaaaaaaaaa)
			data_rx_flag_IL68[0] <= 1'b1;
	end

always @(posedge clk_50)
begin
	if(data_rx_flag_IL68[0]==1'b1) 
	begin
		case(counter_IL68_50)
			3'b000:	begin
						if(IL68_50==25'haaaaaaaaaaaaaaaaaa)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;
							counter_IL68_50<=3'b001;			
					end
			3'b001:	begin
						if(IL68_50==25'h555555555555555555)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;		
							counter_IL68_50<=3'b010;
					end						
			3'b010:	begin
						if(IL68_50==25'h0f0f0f0f0f0f0f0f0f)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;		
							counter_IL68_50<=3'b011;
					end	
			3'b011:	begin
						if(IL68_50==25'hf0f0f0f0f0f0f0f0f0)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;
							counter_IL68_50<=3'b100;
					end	
			3'b100:	begin
						if(IL68_50==25'h000000000000000000)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;
							counter_IL68_50<=3'b101;		
					end	
			3'b101:	begin
						if(IL68_50==25'hffffffffffffffffff)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;
							counter_IL68_50<=3'b110;							
					end
			3'b110:	begin
						if(IL68_50==25'hf5a0f5a0f5a0f5a0f5)
							error_dect_IL68[0]<=1'b0;
						else
							error_dect_IL68[0]<=1'b1;
							counter_IL68_50<=3'b000;							
					end	

			default:begin
						error_dect_IL68[0] <= 0;
					end
		endcase
	end
end

always @(posedge clk125)
begin
	IL68_125 <= IL68[70:25];
	if(IL68_125 == 46'haaaaaaaaaaaaaaaaaa)
		data_rx_flag_IL68[1] <= 1'b1;
end

always @(posedge clk125)
begin
	if(data_rx_flag_IL68[1]==1'b1)
	begin
		case(counter_IL68_125)
			3'b000:	begin
						if(IL68_125==46'haaaaaaaaaaaaaaaaaa)
							error_dect_IL68[1]<=1'b0;
						else
							error_dect_IL68[1]<=1'b1;
							counter_IL68_125<=3'b001;			
					end
			3'b001:	begin
						if(IL68_125==46'h555555555555555555)
							error_dect_IL68[1]<=1'b0;
						else
							error_dect_IL68[1]<=1'b1;		
							counter_IL68_125<=3'b010;
					end						
			3'b010:	begin
						if(IL68_125==46'h0f0f0f0f0f0f0f0f0f)
							error_dect_IL68[1]<=1'b0;
						else
							error_dect_IL68[1]<=1'b1;		
							counter_IL68_125<=3'b011;
					end	
			3'b011:	begin
						if(IL68_125==46'hf0f0f0f0f0f0f0f0f0)
							error_dect_IL68[1]<=1'b0;
						else
							error_dect_IL68[1]<=1'b1;
							counter_IL68_125<=3'b100;
					end	
			3'b100:	begin
						if(IL68_125==46'h000000000000000000)
							error_dect_IL68[1]<=1'b0;
						else
							begin
								error_dect_IL68[1]<=1'b1;
								counter_IL68_125<=3'b101;
							end		
					end	
			3'b101:	begin
						if(IL68_125==46'hffffffffffffffffff)
							error_dect_IL68[1]<=1'b0;
						else
							error_dect_IL68[1]<=1'b1;
							counter_IL68_125<=3'b110;							
					end
			3'b110:	begin
						if(IL68_125==46'hf5a0f5a0f5a0f5a0f5)
							error_dect_IL68[1]<=1'b0;
						else
							error_dect_IL68[1]<=1'b1;
							counter_IL68_125<=3'b000;							
					end	

			default:begin
						error_dect_IL68[1] <= 0;
					end
		endcase
	end
end

always @(posedge clk_50)
begin
		case(counter_50)
			3'b000 :	begin
							IL12_50    <= 71'haaaaaaaaaaaaaaaaaa; // Send the data packet-1	
							IL34_50    <= 71'haaaaaaaaaaaaaaaaaa; // Send the data packet-1	
							counter_50 <= 3'b001;
						end

			3'b001 :	begin
							IL12_50    <= 71'h555555555555555555; // Send the data packet-2
							IL34_50    <= 71'h555555555555555555; // Send the data packet-2
							counter_50 <= 3'b010;
						end

			3'b010 :	begin
							IL12_50    <= 71'h0f0f0f0f0f0f0f0f0f; // Send the data packet-3
							IL34_50    <= 71'h0f0f0f0f0f0f0f0f0f; // Send the data packet-3
							counter_50 <= 3'b011;
						end

			3'b011 :	begin
							IL12_50    <= 71'hf0f0f0f0f0f0f0f0f0; // Send the data packet-4
							IL34_50    <= 71'hf0f0f0f0f0f0f0f0f0; // Send the data packet-4
							counter_50 <= 3'b100; //
						end
			3'b100 :	begin
							IL12_50    <= 71'h000000000000000000; // Send the data packet-5
							IL34_50    <= 71'h000000000000000000; // Send the data packet-5
							counter_50 <= 3'b101; //
						end

			3'b101 :	begin
							IL12_50    <= 71'hffffffffffffffffff; // Send the data packet-6
							IL34_50    <= 71'hffffffffffffffffff; // Send the data packet-6
							counter_50 <= 3'b110; //
						end

			3'b110 :	begin
							IL12_50    <= 71'hf5a0f5a0f5a0f5a0f5; // Send the data packet-7
							IL34_50    <= 71'hf5a0f5a0f5a0f5a0f5; // Send the data packet-7
							counter_50 <= 3'b000; //
						end
			default: begin
							counter_50 <= 3'b000;
					end
		endcase		

end


always @(posedge clk125)
begin	
	case(counter_125)
			3'b000 :	begin
							IL12_125    <= 71'haaaaaaaaaaaaaaaaaa; // Send the data packet-1
							IL34_125    <= 71'haaaaaaaaaaaaaaaaaa; // Send the data packet-1
							counter_125 <= 3'b001;
						end

			3'b001 :	begin
							IL12_125    <= 71'h555555555555555555; // Send the data packet-2
							IL34_125    <= 71'h555555555555555555; // Send the data packet-2
							counter_125 <= 3'b010;
						end

			3'b010 :	begin
							IL12_125    <= 71'h0f0f0f0f0f0f0f0f0f; // Send the data packet-3
							IL34_125    <= 71'h0f0f0f0f0f0f0f0f0f; // Send the data packet-3
							counter_125 <= 3'b011;
						end

			3'b011 :	begin
							IL12_125    <= 71'hf0f0f0f0f0f0f0f0f0; // Send the data packet-4
							IL34_125    <= 71'hf0f0f0f0f0f0f0f0f0; // Send the data packet-4
							counter_125 <= 3'b100; //
						end
			3'b100 :	begin
							IL12_125    <= 71'h000000000000000000; // Send the data packet-5
							IL34_125    <= 71'h000000000000000000; // Send the data packet-5
							counter_125 <= 3'b101; //
						end

			3'b101 :	begin
							IL12_125    <= 71'hffffffffffffffffff; // Send the data packet-6
							IL34_125    <= 71'hffffffffffffffffff; // Send the data packet-6
							counter_125 <= 3'b110; //
						end

			3'b110 :	begin
							IL12_125    <= 71'hf5a0f5a0f5a0f5a0f5; // Send the data packet-7
							IL34_125    <= 71'hf5a0f5a0f5a0f5a0f5; // Send the data packet-7
							counter_125 <= 3'b000; //
						end
			default:    begin
							counter_125 <= 3'b000;
					    end
		endcase	
end

endmodule