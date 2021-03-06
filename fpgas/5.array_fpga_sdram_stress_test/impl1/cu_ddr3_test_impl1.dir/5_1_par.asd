[ActiveSupport PAR]
; Global primary clocks
GLOBAL_PRIMARY_USED = 10;
; Global primary clock #0
GLOBAL_PRIMARY_0_SIGNALNAME = u_ddr3_sdram_mem_top/inst1_inst/U1_clocking/clkop;
GLOBAL_PRIMARY_0_DRIVERTYPE = PLL;
GLOBAL_PRIMARY_0_LOADNUM = 1;
; Global primary clock #1
GLOBAL_PRIMARY_1_SIGNALNAME = fpga_int_clk;
GLOBAL_PRIMARY_1_DRIVERTYPE = OSC;
GLOBAL_PRIMARY_1_LOADNUM = 10;
; Global primary clock #2
GLOBAL_PRIMARY_2_SIGNALNAME = jtaghub16_jtck;
GLOBAL_PRIMARY_2_DRIVERTYPE = JTAG;
GLOBAL_PRIMARY_2_LOADNUM = 536;
; Global primary clock #3
GLOBAL_PRIMARY_3_SIGNALNAME = sclk;
GLOBAL_PRIMARY_3_DRIVERTYPE = CLKDIV;
GLOBAL_PRIMARY_3_LOADNUM = 2479;
; Global primary clock #4
GLOBAL_PRIMARY_4_SIGNALNAME = u_ddr3_sdram_mem_top/inst1_inst/U1_clocking/clkos;
GLOBAL_PRIMARY_4_DRIVERTYPE = PLL;
GLOBAL_PRIMARY_4_LOADNUM = 12;
; Global primary clock #5
GLOBAL_PRIMARY_5_SIGNALNAME = sa5pht/rdcnt[9];
GLOBAL_PRIMARY_5_DRIVERTYPE = SLICE;
GLOBAL_PRIMARY_5_LOADNUM = 23;
; Global primary clock #6
GLOBAL_PRIMARY_6_SIGNALNAME = sa5pht/lclk;
GLOBAL_PRIMARY_6_DRIVERTYPE = SLICE;
GLOBAL_PRIMARY_6_LOADNUM = 8;
; Global primary clock #7
GLOBAL_PRIMARY_7_SIGNALNAME = ddr3_test_top_reveal_coretop_instance/ddr3_test_top_la0_inst_0/reset_rvl_n;
GLOBAL_PRIMARY_7_DRIVERTYPE = SLICE;
GLOBAL_PRIMARY_7_LOADNUM = 0;
; Global primary clock #8
GLOBAL_PRIMARY_8_SIGNALNAME = jtaghub16_jrstn;
GLOBAL_PRIMARY_8_DRIVERTYPE = JTAG;
GLOBAL_PRIMARY_8_LOADNUM = 0;
; Global primary clock #9
GLOBAL_PRIMARY_9_SIGNALNAME = u_ddr3_sdram_mem_top/inst1_inst/U1_clocking/clk_in_c;
GLOBAL_PRIMARY_9_DRIVERTYPE = CLK_PIN;
GLOBAL_PRIMARY_9_LOADNUM = 1;
; # of global secondary clocks
GLOBAL_SECONDARY_USED = 0;
; I/O Bank 0 Usage
BANK_0_USED = 0;
BANK_0_AVAIL = 56;
BANK_0_VCCIO = NA;
BANK_0_VREF1 = NA;
BANK_0_VREF2 = NA;
; I/O Bank 1 Usage
BANK_1_USED = 1;
BANK_1_AVAIL = 48;
BANK_1_VCCIO = 1.5V;
BANK_1_VREF1 = NA;
BANK_1_VREF2 = NA;
; I/O Bank 2 Usage
BANK_2_USED = 0;
BANK_2_AVAIL = 48;
BANK_2_VCCIO = NA;
BANK_2_VREF1 = NA;
BANK_2_VREF2 = NA;
; I/O Bank 3 Usage
BANK_3_USED = 51;
BANK_3_AVAIL = 64;
BANK_3_VCCIO = 1.5V;
BANK_3_VREF1 = 0.75V@VREF1;
BANK_3_VREF2 = NA;
; I/O Bank 4 Usage
BANK_4_USED = 21;
BANK_4_AVAIL = 24;
BANK_4_VCCIO = 1.5V;
BANK_4_VREF1 = NA;
BANK_4_VREF2 = NA;
; I/O Bank 6 Usage
BANK_6_USED = 0;
BANK_6_AVAIL = 64;
BANK_6_VCCIO = NA;
BANK_6_VREF1 = NA;
BANK_6_VREF2 = NA;
; I/O Bank 7 Usage
BANK_7_USED = 0;
BANK_7_AVAIL = 48;
BANK_7_VCCIO = NA;
BANK_7_VREF1 = NA;
BANK_7_VREF2 = NA;
; I/O Bank 8 Usage
BANK_8_USED = 0;
BANK_8_AVAIL = 13;
BANK_8_VCCIO = NA;
BANK_8_VREF1 = NA;
BANK_8_VREF2 = NA;
