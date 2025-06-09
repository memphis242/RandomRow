# Hardware Requirements Document
This requirements document specified hardware details that are a layer lower than the system level requirements. This includes:
- Output signal specifications in addition to the specifications of the [System Requirements document](../../SystemArchitecture/Requirements/SystemRequirements.md)
- Component selection
- Hardware ports
- Device enclosure

## Terminology
In addition to the terminology introduced by the [System Requirements document](../../SystemArchitecture/SystemRequirements.md), here is more terminology scoped to this document.
1. **Height**: Looking at the face of the device with the screen, the height is the vertical dimension
1. **Width**: Looking at the face of the device with the screen, the width is the horizontal dimension
1. **Depth**: How high the device comes off a flat surface if the face of the device with the screen is facing up
1. **Unwanted Signal Overlay**: Any signal on top of the ideal square waveform that is undersired (noise, ripple)
1. **Device Efficiency**: The ratio between the power to a 100Ω load and the power supplied by the device's battery, expressed as a percentage %
1. **Worst-Case Device Efficiency**: The device efficiency under conditions that cause maximum losses (conditions TBD)

## Requirements
1. `HRD_01`: The device shall be within ± 10% of these dimensions:
   - Height: TODO
   - Width: TODO  
   - Depth: TODO  
1. `HRD_02`: The device's output shall be single-ended.
1. `HRD_03`: (FUTURE) Differential outputs shall be supported.
1. `HRD_04`: To fulfill `UI_TBD`, the following shall be the output ports for this device:
   - `HRD_04A`: a BNC port (receptable) where the outer shell is connected to ground and the inner line is connected to the output signal
   - `HRD_04B`: a 0.1" pin header pair (ground + signal)
   - `HRD_04C`: two banana plug receptacles (ground + signal)
1. `HRD_05`: The device shall be powered by a single standard ANSI 1604A (IEC 6LR61) 9V alkaline battery.
   - :scroll: Rough rule of thumb is at 2mA, you'll last 2 weeks. See example [DURACELL PLUS MN1604 datasheet](https://www.duracell.com/wp-content/uploads/2020/02/9V-Duracell-Plus.pdf).
1. `HRD_06`: (FUTURE) The device shall support being powered via USB C.
1. `HRD_07`: The device shall have a 3V backup 2032 battery to support graceful shutdown.
1. `HRD_08`: The display shall have 10mm bevels above and below it.
1. `HRD_09`: The display shall be on the left half of the device.
1. `HRD_10`: The display's right edge shall be at 60% ± 5% of the device's width.
   - _Rationale_: This is approximately the golden ratio φ divided by 1 + φ
1. `HRD_11`: The output ports for the device shall be on the left, right, or top face of the device.
1. `HRD_12`: The output enabled LED shall be on the same face as the output ports.
1. `HRD_13`: The SetPot shall be support > 2 full turns.
1. `HRD_14`: The SetPot shall have a metal knurled knob.
1. `HRD_15`: The SetPot shall a diameter ≥ 60% of the height of the display.
1. `HRD_16`: The enable switch shall be a slide switch.
1. `HRD_17`: When driving a 10kΩ _resistive_ load to ground, the device shall have a rise and fall time of no greater than 10% of the **Maximum Frequency`'s period.
1. `HRD_18`: The device shall have a 100Ω ± 10% output impedance throughout its rated frequency range.
1. `HRD_19`: The device must be able to operate in typical indoor office conditions:   
   - `HRD_19A`: 10°C to 50°C ambient temperature   
   - `HRD_19B`: 30% to 60% relative humidity
1. `HRD_20`: The output signal shall have ≤ ±1mV of **unwanted signal overlay** on top of the base square waveform across its rated frequency range and output load range.
1. `HRD_21`: The worst-case device efficiency shall be ≥ 80%.
1. `HRD_22`: There shall be an on-board LED for each voltage rail that indicates the rail's voltage is within ±50% of its expected level.
   - `HRD_22A`: These LEDs are hidden from the user.
1. `HRD_23`: There shall be an on-board LED that is blinked at a 1Hz rate after software boot-up that helps indicate that software is generally operating normally.
   - Justification: The software shall drive this LED in a separate task and place this task at a low priority. If the LED blinks, this means the task is executing and it may be inferred that tasks at a higher priority must have completed execution as well.

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
:scroll: Different MCUs will be supported in order to diversify this critical component.   
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
According to the S7S datasheet, each solenoid segment of the display will require a 19V 1ms pulse (/w ≥ 900ms recovery time) to drive 335mA through and get the necessary magnetic reversal effect. Given that, here are the options:
1. [TI LM5158](https://www.ti.com/product/LM5158)   
   - The S7S display /w controller that I got uses an [MC34063A](https://www.ti.com/product/MC34063A) switching regulator (1.5A boost/buck/inverting), which takes as input 3-40V and can output 1.25-40V and up to 1.5A.   
   - On the product page of the MC34063A, TI recommended a newer version of the product /w the asme functionality and better performance - the LM51581. This upgrade has a wider input and output voltage range (1.5-60V and 2-83V respectively), features an enable pin, has far less quiescent current (670µA instead of 4000µA), wider temperature range, max duty cycle, bandwidth, and much more. Genuinely an upgrade as far as I can tell.
   - The LM5158 is the higher current version of the LM51581, /w an integrated power switch that can tolerate 3.26A instead of just 1.63A.

### Device Enclosure
- Should be an aluminum frame.
- The digits should be ≥ 60% of the front area of the product and ≥ 80% of the depth
3D model, material, and fabrication details coming soon...

See the general sketch of the front face of the device below:   
   
TODO

