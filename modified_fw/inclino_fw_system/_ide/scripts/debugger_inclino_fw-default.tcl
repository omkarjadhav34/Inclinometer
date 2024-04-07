# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\modified_fw\inclino_fw_system\_ide\scripts\debugger_inclino_fw-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\modified_fw\inclino_fw_system\_ide\scripts\debugger_inclino_fw-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -filter {jtag_cable_name =~ "Digilent Nexys4DDR 210292743024A" && level==0 && jtag_device_ctx=="jsn-Nexys4DDR-210292743024A-13631093-0"}
fpga -file C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/modified_fw/inclino_fw/_ide/bitstream/download.bit
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
loadhw -hw C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/modified_fw/inclino/export/inclino/hw/inclino_1.xsa -regs
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
rst -system
after 3000
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
dow C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/modified_fw/inclino_fw/Debug/inclino_fw.elf
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
con
