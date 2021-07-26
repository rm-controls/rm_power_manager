# rm_power_manager

![Version](https://img.shields.io/badge/Version-1.4.7-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;[![License](https://img.shields.io/badge/License-BSD3.0-blue.svg)](
LICENSE)
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

***

### FSM Status Introduction

1. Normal Mode: Charge with a constant power or a power proportional to the maximum power, and the remaining power is
   directly supplied to the chassis through the high side switch. Due to the direct supply chassis, the energy loss of
   this mode is the smallest.
2. OverPower Mode: All power is supplied to the capacitor for charging. After boosting the capacitor to 26V, it is
   supplied to the chassis. The efficiency of this mode is only about 85%, but it can provide super power output of
   about 200W in a short time.
3. NoCharge Mode: All power is supplied directly to the chassis through the high side switch. Due to the direct supply
   chassis, the energy loss of this mode is the smallest.
4. Halt Mode: Turn off the capacitor charging module, turn off the capacitor boost module and turn off the high side
   switch. At this time, the input, output and capacitor bank are completely disconnected for the mode of robot death or
   overload protection.

***

### Optimization Schemes Introduction

&nbsp;&nbsp;There are three optimization schemes for different scenarios:

1. Charge priority: When the power of the capacitor bank is not full, charge the capacitor bank with 15% limited power,
   at this time, only 85% of the limited power of the chassis can be used. When the power of the capacitor bank exceeds
   95%, it will switch to the non charging mode, and all power will be supplied to the chassis.
2. discharge priority: When the power of the capacitor bank is greater than 10%, the state machine works in the super
   power state, and the output power can reach 200W. When the power of the capacitor bank is less than 10%, it will be
   switched to the non charging mode, and all power will be supplied to the chassis.
3. Normal: Under this optimization scheme, the capacitor bank is not charged, and all power will be given to the
   chassis.
