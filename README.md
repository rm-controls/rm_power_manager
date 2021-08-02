# rm_power_manager

![Version](https://img.shields.io/badge/Version-1.5.2-brightgreen.svg)&nbsp;&nbsp;![Build](https://img.shields.io/badge/Build-Passed-success.svg)&nbsp;&nbsp;![License](https://img.shields.io/badge/License-BSD3.0-blue.svg)

**Author: Yanzhen Zhu <br>
Affiliation: DynamicX <br>
Maintainer:YanzhenZhu, acme.zhuyanzhen@gmail.com**

***

### Breif Introduction

&nbsp;&nbsp;&nbsp;&nbsp;This is all the code of the power management module used by all the robots in dynamicx team. The
main task of the power management module is to redistribute and manage the power path of the robot chassis. Because the
competition rules allow the use of super capacitor, the main function of the module is to distribute electric energy to
charge the super capacitor, so that the super capacitor can generate more than 200W power to the robot chassis when
needed, so that the robot can obtain better mobility.

**Keywords:** Power manager, Over Power, RoboMaster

***

### Develop Environment

+ IDE: CLion 2021.2 - CL212.4746.93
+ Compiler: gcc-arm-none-eabi 10-2020-q4-major
+ Debugger: OpenOCD 0.11.0 (2021-06-25)
+ MCU: STM32H750VBT6 with 128KB Flash & 1MB RAM

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
4. SucapTest Mode: Turn off all outputs to the chassis and charge the capacitor bank with full power. This status is
   only used during recording.
5. Halt Mode: Turn off the capacitor charging module, turn off the capacitor boost module and turn off the high side
   switch. At this time, the input, output and capacitor bank are completely disconnected for the mode of robot death or
   overload protection.

***

### Optimization Schemes Introduction

&nbsp;&nbsp;There are three optimization schemes for different scenarios:

1. Charge priority: When the power of the capacitor bank is not full, charge the capacitor bank with 15% limited power.
   at this time, only 85% of the limited power of the chassis can be used. When the power of the capacitor bank exceeds
   95%, it will switch to the non charging mode, and all power will be supplied to the chassis.
2. Use priority: When the power of the capacitor bank is greater than 10%, the state machine works in the super power
   state, and the output power can reach 200W. When the power of the capacitor bank is less than 10%, it will be
   switched to the non charging mode, and all power will be supplied to the chassis.
3. Normal: Under this optimization scheme, the capacitor bank is not charged, and all power will be given to the
   chassis.
4. Sucap Test: Charge the super capacitor with full power and enter the halt mode after it is fully charged until NUC
   sends information and switches to other modes.

***

### Power Calibrate Introduction

&nbsp;&nbsp;&nbsp;&nbsp;Since the measured power of the referee system (hereinafter referred to as "actual power") is
inconsistent with the measured power of the power management (hereinafter referred to as "measured power"), the power
management will correct the power at the initial stage of power on. The power correction scheme uses polynomial to fit
the function between the actual power and the measured power. <br>
&nbsp;&nbsp;&nbsp;&nbsp;There are two fitting methods: simple fitting and complex fitting, both of which are fitted by
the least square method. Simple fitting samples are taken in the range of 10 ~ 40W and fitted with a primary function.
The complex fitting is sampled in the range of 10 ~ 130W and fitted with quadratic function.

***

### Bugs & Feature Requests

Please report bugs and request features using
the [Issue Tracker](http://frp.acmetech.top:7070/dynamicx/rm_power_manager/-/issues)
