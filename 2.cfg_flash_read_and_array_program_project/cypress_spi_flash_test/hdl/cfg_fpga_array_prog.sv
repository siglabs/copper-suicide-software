
/* 
 * This is needed to allow user logic to drive MCLK of the config bank after the FPGA is programmed.
 *
 * See ECP5 sysCONFIG Usage Guide pgs. 17-18
 */
 
`default_nettype none

module USRMCLK (USRMCLKI, USRMCLKTS) /* synthesis syn_blackbox=1 */;
input USRMCLKI, USRMCLKTS;
endmodule

module cfg_fpga_array_prog (

    /* THESE NAMES WERE CHOSEN IN ORDER TO MATCH THE NAME OF THE NET CONNECTED TO THE CFG FPGA IN THE COPPER SUICIDE SCHEMATIC */
        
    output FPGA_LEDA,

//    input CLK_RC,
//    input G8_D3_N, // I think this is the push button reset 

    /* ECP5 FPGA SLAVE SERIAL PROGRAMMING INTERFACE */
    
    /* note: bug in rev. 1 copper-suicide schematic resulted in INITN and DONE not getting connected to the CFG FPGA.  This should be fixed in rev. 2 */
//    input  [15:0] CFG_INITN,
//    input  [15:0] CFG_DONE,
    output [15:0] CFG_PROG_N,
    output [15:0] CFG_MCLK,
    output [15:0] CFG_DIN,

    /* NOR FLASH QSPI INTERFACE */
    output       CFG_CSS_N,
    input        CFG_D3,
    input        CFG_D2,
    input        CFG_D1,
    inout        CFG_D0

);

    localparam                       FPGA_CONFIG_BYTES       = 2_293_750; // Max ECP5 config file size in bytes (you should pad out your actual bit file to be this size if it's smaller)
    localparam                       NUM_SLAVE_FPGAS         = 16;
    localparam                       FLASH_ADDR_BITS         = 32; // N25Q/MT25Q parts larger than 128Mbit use 4-byte addresses, while 128Mbit and smaller use 3-byte addresses
    localparam                       FLASH_256MBIT_OR_LARGER = (FLASH_ADDR_BITS > 24) ? 1 : 0;
    localparam [FLASH_ADDR_BITS-1:0] FLASH_RD_BYTES          = (FPGA_CONFIG_BYTES * NUM_SLAVE_FPGAS);

//    logic clk50                                                          /* synthesis syn_keep=1 */;
//    logic clk50_srst             = 0;
//    logic sys_pll_locked;
    
    logic                      fpga_int_osc /* synthesis syn_keep=1 */;
    logic                      fpga_int_osc_srst;
    logic [7:0]                fpga_int_osc_srst_cntr = '0;

    logic                      usrmclk_ts = 1 /* synthesis syn_keep=1 */;
    logic [3:0]                usrmclk_ts_cntr = 0;

    logic                      qspi_clk;
    logic                      cfg_d0_high_z;
    logic                      cfg_d0_out;

    logic                      fpga_prog_start_pulse                      /* synthesis syn_keep=1 */;
    logic [7:0]                fpga_prog_start_cntr; // adjust this to delay the start of FPGA programming by more or less.  I used it so I could capture the beginning of programming in Reveal
    logic [1:0]                fpga_prog_start_regs                       /* synthesis syn_noprune=1 */;
    logic [3:0]                flash_rd_byte_route_index; // index for which slave serial programmer gets the current byte read from flash

    logic                      flash_rd_pause           = 0               /* synthesis syn_keep=1 */;
    logic                      flash_rd_busy                              /* synthesis syn_keep=1 */; 
    logic                      flash_rd_byte_vld                          /* synthesis syn_keep=1 */;
    logic [7:0]                flash_rd_byte                              /* synthesis syn_keep=1*/; 

    logic                      flash_rd_busy_reg                          /* synthesis syn_noprune=1 */; 
    logic                      flash_rd_byte_vld_reg                      /* synthesis syn_noprune=1 */;
    logic [7:0]                flash_rd_byte_reg                          /* synthesis syn_noprune=1 */; 

    
    logic [15:0]               ss_mclk;
    logic [15:0]               ss_dout_high_z;
    logic [15:0]               ss_dout;
    logic [15:0]               ss_mclk_reg            = 0               /* synthesis syn_keep=1 */;
    logic [15:0]               ss_dout_high_z_reg     = 0               /* synthesis syn_keep=1 */;
    logic [15:0]               ss_dout_reg            = 0               /* synthesis syn_keep=1 */; 
    logic [15:0]               ss_byte_vld                              /* synthesis syn_preserve=1 */;
    logic [15:0] [7:0]         ss_byte                                  /* synthesis syn_preserve=1 */; 
    logic [15:0]               ss_byte_ack                              /* synthesis syn_keep=1 */;
    logic [15:0]               ss_prog_idle                             /* synthesis syn_keep=1 */;
    logic [15:0]               ss_prog_fpga_stat_vld                    /* synthesis syn_keep=1 */;
    logic [15:0]               ss_prog_fpga_cfg_err                     /* synthesis syn_keep=1 */;
    logic [15:0]               ss_prog_fpga_programmed                  /* synthesis syn_keep=1 */;


    logic [15:0]               ss_prog_idle_reg                         /* synthesis syn_noprune=1 */;
    logic [15:0]               ss_prog_fpga_stat_vld_reg                /* synthesis syn_noprune=1 */;
    logic [15:0]               ss_prog_fpga_cfg_err_reg                 /* synthesis syn_noprune=1 */;
    logic [15:0]               ss_prog_fpga_programmed_reg              /* synthesis syn_noprune=1 */;

    
    logic [23:0]               fpga_leda_cntr;
    
    
    
//    sys_pll sys_pll_inst (
//        .CLKI(CLK_RC), 
//        .CLKOP(clk50), 
//        .LOCK(sys_pll_locked));  
    
    
    /* 
     * 
     * INTERNAL OSCILLATOR SINCE COPPER SUICIDE 100MHz CLOCK IS ON THE WRONG BANK FOR IT'S IO STANDARD. 
     * 
     */
    
    OSCG #(.DIV(8)) oscg_inst (.OSC(fpga_int_osc)); // DIV = 8 sets frequency of 38.8MHz +/- 20% 
    
    always_ff @(posedge fpga_int_osc) begin

        fpga_int_osc_srst <= 0;

        if (fpga_int_osc_srst_cntr != 8'hff) begin
            fpga_int_osc_srst_cntr <= fpga_int_osc_srst_cntr + 1;
            fpga_int_osc_srst      <= 1;
        end
    end
    
    
    /*
     *  BLINK CFG FPGA LED
     */
    
    always_ff @(posedge fpga_int_osc) begin
        fpga_leda_cntr <= fpga_leda_cntr + 1;
    end
    
    assign FPGA_LEDA = fpga_leda_cntr[$bits(fpga_leda_cntr)-1];
       
    /*
     * 
     * 
     *  WORKAROUND FOR INITN NOT BEING CONNECTED ON COPPER-SUICIDE REV. 1.  THIS SHOULD BE FIXED IN REV. 2 SO REMOVE THIS FOR REV. 2 BOARDS.
     * 
     * 
     */
    
    logic [15:0] CFG_INITN;
    logic [15:0] CFG_DONE = '1; // ecp5 slave serial programmer only checks this after finishing programming so it's ok to just tie it high for the workaround
    logic [15:0] [19:0] cfg_initn_cntr;
    
    always_ff @(posedge fpga_int_osc) begin
        for (int i=0; i<16; i++) begin
            if ( fpga_int_osc_srst ) begin
                CFG_INITN[i]      <= 1;
                cfg_initn_cntr[i] <= '1;
            end else if (CFG_PROG_N[i] == 0) begin
                CFG_INITN[i]      <= 0;
                cfg_initn_cntr[i] <= '0;
            end else begin
                if (cfg_initn_cntr[i] != {$bits(cfg_initn_cntr[0]){1'b1}}) begin
                    CFG_INITN[i]      <= 0;
                    cfg_initn_cntr[i] <= cfg_initn_cntr[i] + 1;
                end else begin
                    CFG_INITN[i] <= 1;
                end
            end
        end
    end


 
    USRMCLK u1 (.USRMCLKI(qspi_clk), .USRMCLKTS(usrmclk_ts)) /* synthesis syn_noprune=1 */;



    always_ff @(posedge fpga_int_osc) begin

//        if (fpga_int_osc_srst | (~sys_pll_locked)) begin
        if (fpga_int_osc_srst) begin
            usrmclk_ts <= 1;
            usrmclk_ts_cntr <= 0;
        end else begin
            if (usrmclk_ts_cntr != {$bits(usrmclk_ts_cntr){1'b1}}) begin
                usrmclk_ts_cntr <= usrmclk_ts_cntr + 1;
                usrmclk_ts <= 1;
            end else begin
                usrmclk_ts <= 0;
            end
        end
    end
    
    always_ff @(posedge fpga_int_osc) begin
        if (fpga_int_osc_srst) begin
            fpga_prog_start_regs <= '0;
            fpga_prog_start_cntr <= '0;
        end else begin
            if (fpga_prog_start_cntr != {$bits(fpga_prog_start_cntr){1'b1}}) begin
                fpga_prog_start_cntr <= fpga_prog_start_cntr + 1;
                fpga_prog_start_regs <= '0;
            end else begin
                fpga_prog_start_regs <= {fpga_prog_start_regs[0], 1'b1};
            end
        end
    end
    
    assign fpga_prog_start_pulse = fpga_prog_start_regs[0] & ~fpga_prog_start_regs[1];


    /* REGISTERS FOR MONITORING IN REVEAL */
    always_ff @(posedge fpga_int_osc) begin
        flash_rd_busy_reg           <= flash_rd_busy;
        flash_rd_byte_vld_reg       <= flash_rd_byte_vld;
        flash_rd_byte_reg           <= flash_rd_byte;

        ss_prog_idle_reg            <= ss_prog_idle;
        ss_prog_fpga_stat_vld_reg   <= ss_prog_fpga_stat_vld;
        ss_prog_fpga_cfg_err_reg    <= ss_prog_fpga_cfg_err;
        ss_prog_fpga_programmed_reg <= ss_prog_fpga_programmed;
    end


    /* ECP5 ARRAY SLAVE SERIAL PROGRAMMERS */

    
    generate

        genvar i;

        for(i=0; i<16; i++) begin

            ecp5_slave_serial_programmer #(
                .P_CONFIG_BYTES(FPGA_CONFIG_BYTES)
            ) ss_programmer (
                /* USER INTERFACE */
                
                .i_clk            (fpga_int_osc),
                .i_srst           (fpga_int_osc_srst),
                .i_start          (fpga_prog_start_pulse),
                .i_byte_vld       (ss_byte_vld[i]),
                .i_byte           (ss_byte[i]),
                .o_byte_ack       (ss_byte_ack[i]),
                .o_idle           (ss_prog_idle[i]),
                .o_fpga_status_vld(ss_prog_fpga_stat_vld[i]),
                .o_fpga_cfg_err   (ss_prog_fpga_cfg_err[i]),
                .o_fpga_programmed(ss_prog_fpga_programmed[i]),

                /* FPGA PROGRAMMING INTERFACE */

                .i_init_n         (CFG_INITN[i]),
                .i_done           (CFG_DONE[i]),
                .o_prog_n         (CFG_PROG_N[i]),
                .o_mclk           (ss_mclk[i]),
                .o_dout_high_z    (ss_dout_high_z[i]),
                .o_dout           (ss_dout[i])); 


        end
    endgenerate


    /* REGISTERS AND TRI-STATES FOR SLAVE SERIAL PROGRAMMERS */

    always_ff @(posedge fpga_int_osc) begin
        if (fpga_int_osc_srst) begin
            ss_mclk_reg        <= '0;
            ss_dout_high_z_reg <= '1;
            ss_dout_reg        <= '0;
        end else begin
            ss_mclk_reg        <= ss_mclk;
            ss_dout_high_z_reg <= ss_dout_high_z;
            ss_dout_reg        <= ss_dout;
        end
    end

    assign CFG_MCLK = ss_mclk_reg;
    assign CFG_DIN = (~ss_dout_high_z_reg) ? ss_dout_reg : 'z;


    /* THIS LOGIC ROUTES ALTERNATING BYTES TO EACH OF THE ECP5 SLAVE SERIAL PROGRAMMERS */
    always_ff @(posedge fpga_int_osc) begin

        if (fpga_int_osc_srst | ~flash_rd_busy) begin // first read flash byte must be for the ecp5 00 FPGA (i.e. row 0, col 0 FPGA of the array) 
            flash_rd_byte_route_index <= 4'h0;
            ss_byte_vld               <= '0;

//        end else if (flash_rd_byte_vld) begin
        end else begin

            /* defaults */
            ss_byte_vld <= '0;

            if (ss_byte_ack != 16'h0000) begin
                flash_rd_byte_route_index <= flash_rd_byte_route_index + 1; // roll-over desired (0, 1, 2, ..., 15, 0, 1, 2, ...15, etc.)
            end

            ss_byte[flash_rd_byte_route_index]     <= flash_rd_byte;
            ss_byte_vld[flash_rd_byte_route_index] <= flash_rd_byte_vld;
        end
    end

    assign flash_rd_pause = (ss_byte_ack == 16'h0000) ? 1 : 0;


    /* READS ARRAY BIT FILES FROM CONFIG FLASH ATTACHED TO CFG FGPA */
    
    s25fl_qspi_reader #(
        .P_256MBIT_OR_LARGER (FLASH_256MBIT_OR_LARGER  )
    ) flash_rd_inst (

        /* USER INTERFACE */
        
        .i_clk                (fpga_int_osc),
        .i_srst               (fpga_int_osc_srst),
        .i_rd_start           (fpga_prog_start_pulse),
        .i_rd_start_byte_addr (FPGA_CONFIG_BYTES),
        .i_rd_num_bytes       (FLASH_RD_BYTES),
        .i_rd_pause           (flash_rd_pause),
        .o_rd_busy            (flash_rd_busy),
        .o_rd_byte_vld        (flash_rd_byte_vld),
        .o_rd_byte            (flash_rd_byte),
    
        /* QSPI INTERFACE */
    
        .o_qspi_clk       (qspi_clk),
        .o_qspi_sel_n     (CFG_CSS_N),
        .o_qspi_d0_high_z (cfg_d0_high_z),
        .i_qspi_d3        (CFG_D3),
        .i_qspi_d2        (CFG_D2),
        .i_qspi_d1        (CFG_D1),
        .i_qspi_d0        (CFG_D0),
        .o_qspi_d0        (cfg_d0_out)
    );

    /* FLASH D0 TRI-STATE */
    assign CFG_D0 = (~cfg_d0_high_z) ? cfg_d0_out : 1'bz;

endmodule

`default_nettype wire 