# rm_power_manager

***

### Breif Introduction

&nbsp;&nbsp;&nbsp;&nbsp;This is all the code of the power management module used by all the robots in dynamicx team. The
main task of the power management module is to redistribute and manage the power path of the robot chassis. Because the
competition rules allow the use of super capacitor, the main function of the module is to distribute electric energy to
charge the super capacitor, so that the super capacitor can generate more than 200W power to the robot chassis when
needed, so that the robot can obtain better mobility.

***

### Develop Environment

+ IDE: CLion 2020.3 - CL203.7717.62
+ Compiler: gcc-arm-none-eabi 9-2020-q2-update
+ Debugger: OpenOCD 0.10.0 (2020-07-19)
+ MCU: STM32H750VBT6 &nbsp;&nbsp; 128K Flash &nbsp;&nbsp; 1M RAM
