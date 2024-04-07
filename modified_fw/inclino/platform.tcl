# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\modified_fw\inclino\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\modified_fw\inclino\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {inclino}\
-hw {C:\Users\jadha\OneDrive\Desktop\TA\ECE544-Winter24\github_repo\Inclinometer\Inclinometer\inclinometer_rtos_1.xsa}\
-proc {microblaze_0} -os {freertos10_xilinx} -out {C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/modified_fw}

platform write
platform generate -domains 
platform active {inclino}
platform generate
platform config -updatehw {C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/github_repo/Inclinometer/Inclinometer/inclino.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/jadha/OneDrive/Desktop/TA/ECE544-Winter24/github_repo/Inclinometer/Inclinometer/inclino_1.xsa}
platform generate -domains 
platform active {inclino}
bsp reload
bsp reload
bsp config compiler "mb-gcc"
bsp config dependency_flags "-MMD -MP"
bsp config extra_compiler_flags "-g -ffunction-sections -fdata-sections -Wall -Wextra -fno-tree-loop-distribute-patterns"
bsp config extra_compiler_flags "-g -ffunction-sections -fdata-sections -Wall -Wextra -fno-tree-loop-distribute-patterns"
bsp config extra_compiler_flags "-g -ffunction-sections -fdata-sections -Wall -Wextra -fno-tree-loop-distribute-patterns -mxl-float-sqrt"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config compiler "mb-gcc"
bsp config assembler "mb-as"
bsp config compiler "mb-gcc -O3"
bsp write
bsp reload
catch {bsp regenerate}
bsp config compiler "mb-gcc"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config extra_compiler_flags "-g -ffunction-sections -fdata-sections -Wall -Wextra -fno-tree-loop-distribute-patterns -mxl-float-sqrt"
bsp reload
bsp reload
platform generate -domains 
bsp reload
platform active {inclino}
bsp reload
platform generate -domains 
platform active {inclino}
bsp reload
bsp config use_counting_semaphores "false"
bsp config use_freertos_asserts "true"
bsp config use_mutexes "false"
bsp config software_timers "false"
bsp config total_heap_size "512"
bsp config max_priorities "8"
bsp write
bsp reload
catch {bsp regenerate}
bsp config use_recursive_mutexes "false"
bsp write
bsp reload
catch {bsp regenerate}
platform generate
platform active {inclino}
bsp reload
bsp reload
platform generate
bsp config total_heap_size "4096"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config total_heap_size "4096"
bsp config total_heap_size "16384"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config total_heap_size "32768"
bsp config max_priorities "4"
bsp config max_task_name_len "10"
bsp config minimal_stack_size "200"
bsp config tick_rate "100"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp reload
bsp config total_heap_size "32768"
bsp config total_heap_size "16384"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp config total_heap_size "16384"
bsp config total_heap_size "4096"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp reload
bsp config total_heap_size "4096"
bsp config total_heap_size "8192"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains freertos10_xilinx_domain 
bsp reload
platform active {inclino}
platform active {inclino}
bsp reload
bsp config stdin "axi_uartlite_0"
bsp reload
bsp config use_newlib_reent "false"
bsp reload
platform active {inclino}
bsp reload
bsp reload
platform generate
platform generate
platform active {inclino}
platform generate
