# Hardware Description

## High-Level Requirements
1. This device shall last a year on a single AA battery
2. TODO

## Physical I/O
### Inputs
#### Analog
1. `AIN_BAT`: Main battery supply's voltage
1. `AIN_COIN_CELL`: Coin cell's voltage

#### Digital
1. `DIN_GENERATE_RN`: Trigger the generation of a random number
1. `DIN_EN_DISABLE_DIGIT1`: Enable/disable digit 1 (left-most digit)
1. `DIN_EN_DISABLE_DIGIT2`: Enable/disable digit 2 (middle digit)
1. `DIN_EN_DISABLE_DIGIT3`: Enable/disable digit 3 (right-most digit)

### Outputs
TODO

## Component Selection
To meet the above requirements, the following components have been selected.

### Electronic
#### MCU
:scroll: Different MCUs will be supported in order to diversify this critical component (and just for fun!).   
:scroll: Low-power MCUs are prioritized.   
:scroll: Based on the general I/O requirements, we're looking at a < 48 pin count unit   
:scroll: Arm® Cortex®-M0+ MCUs out-perform 8-bit / 16-bit low power MCUs (e.g., STM8L, MSP430FR2x) in ultra-low-power performance!😲   

1. STM32U031R8
   - ST Webpage: [STM32U031R8: Ultra-low-power Arm® Cortex®-M0+ MCU with 64 Kbytes of Flash memory, 56 MHz CPU](https://www.st.com/en/microcontrollers-microprocessors/stm32u031r8.html)
   - Digi-Key: [497-STM32U031R8T6-ND](https://www.digikey.com/en/products/detail/stmicroelectronics/STM32U031R8T6/22337319)
   - Vendor Dev Board:
      - ST: [NUCLEO-U031R8](https://www.st.com/en/evaluation-tools/nucleo-u031r8.html)
      - Digi-Key: [497-NUCLEO-U031R8-ND](https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-U031R8/22470455)

1. MSP430FR2355
   - The 32-bit MCUs actually beat this one power-wise, but I wanted an 8/16-bit MCU in the mix
   - TI Webpage: [MSP430FR2355: 24-MHz 105-C integrated analog microcontroller with 32-KB FRAM, Op-Amps/PGAs, 12-bit DACs, 12-bit AD](https://www.ti.com/product/MSP430FR2355)
   - Digi-Key: [296-50260-ND](https://www.digikey.com/en/products/detail/texas-instruments/MSP430FR2355TPT/9597611)
   - Vendor Dev Board:
      - TI: [MSP-EXP430FR2355: MSP430FR2355 LaunchPad™ development kit](https://www.ti.com/tool/MSP-EXP430FR2355)
      - Digi-Key: [296-50211-ND](https://www.digikey.com/en/products/detail/texas-instruments/MSP-EXP430FR2355/9491427)

1. R7F102GGC2DFB (RL78/G22 Family)
   - Renesas Webpage: [RL78/G22: General-Purpose Microcontrollers with Excellent Low-Power Performance and Rich Capacitive Touch Channels](https://www.renesas.com/en/products/microcontrollers-microprocessors/rl78-low-power-8-16-bit-mcus/rl78g22-general-purpose-microcontrollers-excellent-low-power-performance-and-rich-capacitive-touch-channels)
   - Digi-Key: [559-R7F102GGC2DFB#AA0-ND](https://www.digikey.com/en/products/detail/renesas-electronics-corporation/R7F102GGC2DFB-AA0/19241771)

1. CH32V203 - RISC-V (WCH)
   - WCH Webpage: [32-bit Enhanced Low-Power RISC-V MCU – CH32V203](https://www.wch-ic.com/products/CH32V203.html)
   - Digi-Key: Closest thing is this breakout board: [1528-5996-ND](https://www.digikey.com/en/products/detail/adafruit-industries-llc/5996/24713464)

#### Digits
AlfaZeta's [S7S Displays](https://flipdots.com/en/products-services/small-7-segment-displays/). They're the inspiration for project!

#### Power Supply
To produce the 3.3V power for all the ICs this device will likely have, here are some options:   

1. [MAXM17225](https://www.analog.com/en/products/maxm17225.html)
   - NanoPower Boost Module /w extremely lowest quiescent current (300nA running, 0.5nA shutdown)
   - True Shutdown supported (output disconnected from input)
   - Vin: 0.4V - 5.5V
   - Minimum Startup Voltage: 0.88V
   - Vout: 1.8V - 5V (single resistor adjustment)
   - 1A peak inductor current limit
   - Internal current limiting
   - Internal soft-start
   - µModule package from AD - as in, system-in-module (SIM) that includes much of the power electronics circuitry on-chip

According to the S7S datasheet, each solenoid segment of the display will require a 19V 1ms pulse (/w ≥ 900ms recovery time) to drive 335mA through and get the necessary magnetic reversal effect. Part of the trickiness of the necessary solution here is I want to supply this device using just a single AAA battery. Going from 1.5V → 19V and supplying 335mA of current would pull way too much from the AAA battery. So, I plan to "buffer" the solenoid demands from the battery by having a capacitor supply the necessary current pulses, and the battery to simply charge the capacitor at a slower, easier pace. I made a [Reddit post](https://www.reddit.com/r/AskElectronics/comments/1lcbyfl/single_aaa_battery_lowerpower_mcu_pulsed/) detailing some of the initial design thoughts.

Given that, here are the options for the needed step-up converter:

1. [MAX17291B](https://www.analog.com/en/products/max17291b.html) - This really seems like a perfectly fitting glove for my needs here!
   - Internal power switch boost regular
   - 28µA quiescent current from input, 13nA shutdown
   - 1.8V - 4.5V input range
   - 5.5V - 20V output range
   - 100mA peak inductor current limit
   - True Shutdown supported
   - Output short circuit protection + overtemperature protection

2. [TI LM5158](https://www.ti.com/product/LM5158)   
   - The S7S display /w controller that I got uses an [MC34063A](https://www.ti.com/product/MC34063A) switching regulator (1.5A boost/buck/inverting), which takes as input 3-40V and can output 1.25-40V and up to 1.5A.   
   - On the product page of the MC34063A, TI recommended a newer version of the product /w the asme functionality and better performance - the LM51581. This upgrade has a wider input and output voltage range (1.5-60V and 2-83V respectively), features an enable pin, has far less quiescent current (670µA instead of 4000µA), wider temperature range, max duty cycle, bandwidth, and much more. Genuinely an upgrade as far as I can tell.
   - The LM5158 is the higher current version of the LM51581, /w an integrated power switch that can tolerate 3.26A instead of just 1.63A.

And here are my capacitors of choice to supply the solenoids the necessary pulses:

1. [Panasonic ECA-1EM221BJ 220µF 25V Electrolytic Capacitor](https://www.digikey.com/en/products/detail/panasonic-electronic-components/ECA-1EM221BJ/268499)

### Device Enclosure
- Should be an aluminum frame.
- The digits should be ≥ 60% of the front area of the product and ≥ 80% of the depth
3D model, material, and fabrication details coming soon...

See the general sketch of the front face of the device below:   
   
TODO

