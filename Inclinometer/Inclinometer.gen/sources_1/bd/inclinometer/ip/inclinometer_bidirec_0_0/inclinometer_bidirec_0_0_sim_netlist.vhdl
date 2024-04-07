-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2022.2 (win64) Build 3671981 Fri Oct 14 05:00:03 MDT 2022
-- Date        : Tue Jan 30 15:46:55 2024
-- Host        : Omkar running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode funcsim
--               c:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/github_repo/Inclinometer/Inclinometer/Inclinometer.gen/sources_1/bd/inclinometer/ip/inclinometer_bidirec_0_0/inclinometer_bidirec_0_0_sim_netlist.vhdl
-- Design      : inclinometer_bidirec_0_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xc7a100tcsg324-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity inclinometer_bidirec_0_0_bidirec is
  port (
    outp : out STD_LOGIC;
    bidir : inout STD_LOGIC;
    inp : in STD_LOGIC;
    oe : in STD_LOGIC
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of inclinometer_bidirec_0_0_bidirec : entity is "bidirec";
end inclinometer_bidirec_0_0_bidirec;

architecture STRUCTURE of inclinometer_bidirec_0_0_bidirec is
  attribute BOX_TYPE : string;
  attribute BOX_TYPE of IIC_0_scl_iobuf : label is "PRIMITIVE";
begin
IIC_0_scl_iobuf: unisim.vcomponents.IOBUF
     port map (
      I => inp,
      IO => bidir,
      O => outp,
      T => oe
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity inclinometer_bidirec_0_0 is
  port (
    oe : in STD_LOGIC;
    inp : in STD_LOGIC;
    outp : out STD_LOGIC;
    bidir : inout STD_LOGIC
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of inclinometer_bidirec_0_0 : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of inclinometer_bidirec_0_0 : entity is "inclinometer_bidirec_0_0,bidirec,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of inclinometer_bidirec_0_0 : entity is "yes";
  attribute IP_DEFINITION_SOURCE : string;
  attribute IP_DEFINITION_SOURCE of inclinometer_bidirec_0_0 : entity is "module_ref";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of inclinometer_bidirec_0_0 : entity is "bidirec,Vivado 2022.2";
end inclinometer_bidirec_0_0;

architecture STRUCTURE of inclinometer_bidirec_0_0 is
begin
inst: entity work.inclinometer_bidirec_0_0_bidirec
     port map (
      bidir => bidir,
      inp => inp,
      oe => oe,
      outp => outp
    );
end STRUCTURE;
