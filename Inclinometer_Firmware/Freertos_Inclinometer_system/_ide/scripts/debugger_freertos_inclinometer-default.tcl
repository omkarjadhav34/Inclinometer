# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\github_repo\Inclinometer\Inclinometer_Firmware\Freertos_Inclinometer_system\_ide\scripts\debugger_freertos_inclinometer-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\github_repo\Inclinometer\Inclinometer_Firmware\Freertos_Inclinometer_system\_ide\scripts\debugger_freertos_inclinometer-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
loadhw -hw C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/github_repo/Inclinometer/Inclinometer_Firmware/Nexysa7fpga/export/Nexysa7fpga/hw/inclinometer_rtos.xsa -regs
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
rst -system
after 3000
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
dow C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/github_repo/Inclinometer/Inclinometer_Firmware/Freertos_Inclinometer/Debug/Freertos_Inclinometer.elf
bpadd -addr &main
