# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\Inclinometer_Firmware\Nexysa7fpga\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\Inclinometer_Firmware\Nexysa7fpga\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Nexysa7fpga}\
-hw {C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\Inclinometer\inclinometer_rtos.xsa}\
-proc {microblaze_0} -os {freertos10_xilinx} -out {C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/Inclinometer_Firmware}

platform write
platform generate -domains 
platform active {Nexysa7fpga}
platform generate
platform generate -domains freertos10_xilinx_domain 
platform generate
platform active {Nexysa7fpga}
bsp reload
platform config -updatehw {C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/Inclinometer/inclinometer_rtos.xsa}
platform generate -domains 
