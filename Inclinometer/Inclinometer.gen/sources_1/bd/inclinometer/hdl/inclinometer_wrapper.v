//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2022.2 (win64) Build 3671981 Fri Oct 14 05:00:03 MDT 2022
//Date        : Sun Jan 21 17:21:56 2024
//Host        : Omkar running 64-bit major release  (build 9200)
//Command     : generate_target inclinometer_wrapper.bd
//Design      : inclinometer_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module inclinometer_wrapper
   (JC_0,
    JC_1,
    RGB1_Blue,
    RGB1_Green,
    RGB1_Red,
    RGB2_Blue,
    RGB2_Green,
    RGB2_Red,
    an,
    btnC,
    btnD,
    btnL,
    btnR,
    btnU,
    dp,
    led,
    resetn,
    seg,
    sw,
    sys_clock,
    usb_uart_rxd,
    usb_uart_txd);
  inout JC_0;
  inout JC_1;
  output RGB1_Blue;
  output RGB1_Green;
  output RGB1_Red;
  output RGB2_Blue;
  output RGB2_Green;
  output RGB2_Red;
  output [7:0]an;
  input btnC;
  input btnD;
  input btnL;
  input btnR;
  input btnU;
  output dp;
  output [15:0]led;
  input resetn;
  output [6:0]seg;
  input [15:0]sw;
  input sys_clock;
  input usb_uart_rxd;
  output usb_uart_txd;

  wire JC_0;
  wire JC_1;
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
  wire dp;
  wire [15:0]led;
  wire resetn;
  wire [6:0]seg;
  wire [15:0]sw;
  wire sys_clock;
  wire usb_uart_rxd;
  wire usb_uart_txd;

  inclinometer inclinometer_i
       (.JC_0(JC_0),
        .JC_1(JC_1),
        .RGB1_Blue(RGB1_Blue),
        .RGB1_Green(RGB1_Green),
        .RGB1_Red(RGB1_Red),
        .RGB2_Blue(RGB2_Blue),
        .RGB2_Green(RGB2_Green),
        .RGB2_Red(RGB2_Red),
        .an(an),
        .btnC(btnC),
        .btnD(btnD),
        .btnL(btnL),
        .btnR(btnR),
        .btnU(btnU),
        .dp(dp),
        .led(led),
        .resetn(resetn),
        .seg(seg),
        .sw(sw),
        .sys_clock(sys_clock),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd));
endmodule
