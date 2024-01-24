/**
 * nexys7fpga.v - Implements the top level module for the ECE 544 Getting Started project
 *
 * @author		Roy Kravitz (roy.kravitz@pdx.edu)
 * @date		14-December 2022
 *
 * This is the top-level model for the ECE 544 Getting Started project.  It can be used as a model
 * for creating top-level models for other ECE 544 projects and your own design project.
 *
 * Instantiates the embedded system and connects the ports from the embedded system to ports/pins on the NexysA7 (or
 * Nexys4 DDR board).  Routes GPIO4 to the Control Register for the 3-channel rgb PWM generator.  Also brings the
 * PWM clock and PWM outputs from the PWM generator to JA (one of the Pmod headers on the NexysA7 board). 
 */
`timescale 1 ps / 1 ps

module nexysa7fpga
(
    sw,
    btnC,
    btnU,
    btnR,
    btnL,
    btnD,
    btnCpuReset,
    led,
    RGB1_Blue,
    RGB1_Green,
    RGB1_Red,
    RGB2_Blue,
    RGB2_Green,
    RGB2_Red,
    clk,
    seg,
    an,
    dp,
    JC_0,
    JC_1,
    usb_uart_rxd,
    usb_uart_txd
 );

  input [15:0] sw;
  input btnC, btnU, btnR, btnL, btnD;
  input btnCpuReset;
  output [15:0]led;
  output RGB1_Blue;
  output RGB1_Green;
  output RGB1_Red;
  output RGB2_Blue;
  output RGB2_Green;
  output RGB2_Red;
  input clk; 
  output [6:0] seg;
  output [7:0] an;
  output dp;
  inout  JC_0;
  inout JC_1;
  input usb_uart_rxd;
  output usb_uart_txd;
  wire [15:0] sw;
  wire [4:0]  push_buttons_5bits_tri_i;     // {btnD, btnR, btnL, btnU, btnC}
  wire [15:0] led;
  wire btnCpuReset;
  wire RGB1_Blue;
  wire RGB1_Green;
  wire RGB1_Red;
  wire RGB2_Blue;
  wire RGB2_Green;
  wire RGB2_Red;
  wire [7:0]an;
  wire btnC;
  wire btnD;
  wire btnL;
  wire btnR;
  wire btnU;
  wire clkPWM;
  wire dp;
  wire clk;
  wire sda_io;
  wire sda_i;
  wire sda_o;
  wire sda_t;
  wire sclk_io;
  wire sclk_i;
  wire sclk_o;
  wire sclk_t;
  wire IIC_0_scl_i;
  wire IIC_0_scl_io;
  wire IIC_0_scl_o;
  wire IIC_0_scl_t;
  wire IIC_0_sda_i;
  wire IIC_0_sda_io;
  wire IIC_0_sda_o;
  wire IIC_0_sda_t;
  wire  JC_0;
  wire JC_1;
 /*
  IOBUF IIC_0_scl_iobuf
       (.I(IIC_0_scl_o),
        .IO(JB[0]),
        .O(IIC_0_scl_i),
        .T(IIC_0_scl_t));
  IOBUF IIC_0_sda_iobuf
       (.I(IIC_0_sda_o),
        .IO(JB[1]),
        .O(IIC_0_sda_i),
        .T(IIC_0_sda_t)); 
         
bidirec iic_scl  (.oe(IIC_0_scl_t ), .inp(IIC_0_scl_o ), .outp(IIC_0_scl_i ), .bidir(JB[0] ));
bidirec iic_sda  (.oe(IIC_0_sda_t ), .inp(IIC_0_sda_o ), .outp(IIC_0_sda_i ), .bidir(JB[1] ));
   */
   // instantiate the embedded system  
   inclinometer embsys_i
    (
        .RGB2_Blue(RGB2_Blue),
        .RGB2_Green(RGB2_Green),
        .RGB2_Red(RGB2_Red),
        .RGB1_Blue(RGB1_Blue),
        .RGB1_Green(RGB1_Green),
        .RGB1_Red(RGB1_Red),
        .seg(seg),
        .an(an),
        .btnC(btnC),
        .btnD(btnD),
        .btnL(btnL),
        .btnR(btnR),
        .btnU(btnU),
        .dp_0(dp),/*
        .iic_rtl_scl_i(IIC_0_scl_i),
        .iic_rtl_scl_o(IIC_0_scl_o),
        .iic_rtl_scl_t(IIC_0_scl_t),
        .iic_rtl_sda_i(IIC_0_sda_i),
        .iic_rtl_sda_o(IIC_0_sda_o),
        .iic_rtl_sda_t(IIC_0_sda_t),*/
        .JC_0(JC_0),
        .JC_1(JC_1),
        .led(led),
        .sw(sw),
        .resetn(1'b1),
        .sys_clock(clk),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd));
endmodule

// GPIO Extended
module bidirec (input wire oe, input wire inp, output wire outp, inout wire bidir);

assign bidir = oe ? inp : 1'bZ ;
assign outp  = bidir;

endmodule