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
1. **Max Frequency**: Maximum Frequency specified in `SYS_FUNC_TBD`
1. **Rated Frequency Range**: 0Hz to the Max Frequency specified in `SYS_FUNC_TBD`
1. **Unwanted Signal Overlay**: Any signal on top of the ideal square waveform that is undersired (noise, ripple)
1. **Device Efficiency**: The ratio between the power to a 100Ω load and the power supplied by the device's battery, expressed as a percentage %
1. **Worst-Case Device Efficiency**: The device efficiency under conditions that cause maximum losses (conditions TBD)

## Requirements
1. `HRD_01`: The device shall be within ± 10% of these dimensions:
   - Height: 76mm  
   - Width: 100mm  
   - Depth: 30mm  
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
1. `AIN_SETPOT`: Analog input for the SetPot
1. `AIN_SCREEN_KNOB`: Analog input for the screen knob
1. `AIN_DIAG_FRQ_OUT`: Diagnostic analog input to independently measure the output signal and confirm that it is within spec
1. `AIN_DIAG_FRQ_OUT_CURRENT`: Diagnostic analog input to measure the output current and detect overcurrents
1. `AIN_BAT`: Main battery supply's voltage
1. `AIN_COIN_CELL`: Coin cell's voltage

#### Digital
1. `DIN_SETPOINT_MODE`: Digital input to set the setpoint mode
1. `DIN_VOLT_SET_MODE`: Digital input to set the voltage setpoint mode
1. `DIN_COARSENESS_SW`: Digital input to set the coarseness of the SetPot
1. `DIN_OUT_EN_SW`: Digital input switch to enable/disable the output
1. `DIN_DIAG_FRQ_OUT`: Diagnostic digital input to independently measure the output signal and confirm that it is within spec (to be used as a PWM input)
1. `DIN_DIAG_UI_SETPOT_FRQ_LED`: Diagnostic digital input for the LED that indicates frequency is the active setpoint mode
1. `DIN_DIAG_UI_SETPOT_HPT_LED`: Diagnostic digital input for the LED that indicates high pulse time is the active setpoint mode
1. `DIN_DIAG_UI_COARSE_LED`: Diagnostic digital input for the LED that indicates coarse mode is active
1. `DIN_DIAG_UI_FINE_LED`: Diagnostic digital input for the LED that indicates fine mode is active
1. `DIN_DIAG_LED_OUT_EN`: Diagnostic digital input for the green LED that indicates the output is active
1. `DIN_DIAG_LED_GREEN`: Diagnostic digital input for the green LED that indicates no fault is active
1. `DIN_DIAG_LED_AMBER`: Diagnostic digital input for the amber LED that indicates a mild fault is active
1. `DIN_DIAG_LED_RED`: Diagnostic digital input for the red LED that indicates a catastrophic fault is active

### Outputs
1. `FRQ_OUT`: The star of the show! Output signal with two voltage levels
1. `OUT_EN`: Enable/disable the output, which will include the enable LED, output regulators / analog switches, and any other applicable device involved in enabling/disabling the output
1. `UI_SETPOT_FRQ_LED`: Digital output for the UI LED that represents frequency setpoint mode
1. `UI_SETPOT_HPT_LED`: Digital output for the UI LED that represents HPT setpoint mode
1. `UI_COARSE_LED`: Digital output for the coarse LED
1. `UI_FINE_LED`: Digital output for the fine LED
1. `DIAG_LED_GREEN`: Digital output for the diagnostic green LED
1. `DIAG_LED_AMBER`: Digital output for the diagnostic amber LED
1. `DIAG_LED_RED`: Digital output for the diagnostic red LED

## Component Selection
To meet the above requirements, the following components have been selected.

### Electronic
#### MCU
:scroll: Different MCUs will be supported in order to diversify this critical component.   
:scroll: Furthermore, low-power MCUs are prioritized.   
:scroll: If possible, get one /w a floating-point unit.   
:scroll: Based on the general I/O requirements, we're looking at a ≥ 48 pin count unit that at _minimum_ supports SPI/I2C (most µC out there).   
:scroll: Ideally, there would also be support for CAN/USB for the future, but that may be asking for too much...   
:scroll: Amazingly, the Arm® Cortex®-M0+ MCUs out-perform the 8-bit and 16-bit low power MCUs (e.g., STM8L, MSP430FR2x) variants when it comes to ultra-low-power performance!

1. STM32U031R8
   - ST Webpage: [STM32U031R8: Ultra-low-power Arm® Cortex®-M0+ MCU with 64 Kbytes of Flash memory, 56 MHz CPU](https://www.st.com/en/microcontrollers-microprocessors/stm32u031r8.html)
   - Digi-Key: [497-STM32U031R8T6-ND](https://www.digikey.com/en/products/detail/stmicroelectronics/STM32U031R8T6/22337319)

1. MSPM0L1227
   - TI Webpage: [MSPM0L1227: 32MHz Arm® Cortex®-M0+ MCU with 128KB dual-bank flash, 32KB SRAM, 12-bit ADC, COMP, VBAT, PSA-L1<](https://www.ti.com/product/MSPM0L1227)
   - DigiKey: [296-MSPM0L1227SPMRCT-ND](https://www.digikey.com/en/products/detail/texas-instruments/MSPM0L1227SPMR/25619050?0=%2Fmicrocontrollers&any=embedded&s=N4IgjCBcoLQExVAYygFwE4FcCmAaEA9lANogCsIAugL7X4KSkCyAygApMAMAMmHHAHYq1IA)

1. R7F102GGC2DFB (RL78/G22 Family)
   - Renesas Webpage: [RL78/G22: General-Purpose Microcontrollers with Excellent Low-Power Performance and Rich Capacitive Touch Channels](https://www.renesas.com/en/products/microcontrollers-microprocessors/rl78-low-power-8-16-bit-mcus/rl78g22-general-purpose-microcontrollers-excellent-low-power-performance-and-rich-capacitive-touch-channels)
   - Digi-Key: [559-R7F102GGC2DFB#AA0-ND](https://www.digikey.com/en/products/detail/renesas-electronics-corporation/R7F102GGC2DFB-AA0/19241771)

1. EFM32PG23B210F64IM48-C (Silicon Labs EFM32 PG23 Family)
   - Silicon Labs Webpage: [EFM32PG23B210F64IM48: EFM32PG23 Series 2 MCUs](https://www.silabs.com/mcu/32-bit-microcontrollers/efm32pg23-series-2/device.efm32pg23b210f64im48?tab=techdocs)
   - Digi-Key: [336-EFM32PG23B210F64IM48-C-ND](https://www.digikey.com/en/products/detail/silicon-labs/EFM32PG23B210F64IM48-C/16396764)

#### DAC
:scroll: Different DACs will be supported in order to diversify this critical component.
:scroll: ≤ 500 µA /w inactive comms bus

1. Analog Devices AD5622BKSZ-2REEL7
   - AD Webpage: [AD5622: 2.7 V to 5.5 V, <100 µA, 12-Bit nanoDAC® with I2C Compatible Interface, Tiny SC70 Package](https://www.analog.com/en/products/ad5622.html)
   - DigiKey: [505-AD5622BKSZ-2REEL7CT-ND](https://www.digikey.com/en/products/detail/analog-devices-inc/AD5622BKSZ-2REEL7/995859)

1. MCP47CMD21
   - Microchip Webpage: [MCP47CMD21: 12-Bit Single Output DAC w/MTP and I2C™](https://www.microchip.com/en-us/product/mcp47cmd21)
   - DigiKey: [150-MCP47CMD21T-E/MFCT-ND](https://www.digikey.com/en/products/detail/microchip-technology/MCP47CMD21T-E-MF/16709763)

#### Display
:scroll: Definitely needs to have ≥ 100 x 100 px to support the various screens.   
:scroll: It's going to need to be low-power, which means **no backlight** :no_entry_sign::bulb:   
Crystalfontz [CFAG240128U0-NFH Low Power 240x128 Graphic LCD Display](https://www.crystalfontz.com/product/cfag240128u0nfh-low-power-240x128-graphic-lcd)

### Device Enclosure
3D model, material, and fabrication details coming soon...
See the general sketch of the front face of the device below:   
   
![FSE Device Sketch drawio](https://github.com/user-attachments/assets/d2e239ff-ba75-486b-832b-5c3469d33ba1)

