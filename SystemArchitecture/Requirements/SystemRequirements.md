# System Requirements Document
## General Description
The **Frequency Sensor Emulator** device is intended to serve a simple purpose - generate a square wave signal that emulates certain speed sensors that have a single data line where frequency gives speed information and high pulse time gives direction information. To get this device to be professional-grade and fulfill its purpose reliably and conveniently, considerable thought has been put into the user experience as well as diagnostics and the selection of its components.  

This particular document is the System Requirements Document, where top-level system behavior is specified, as well as critical user interface items. Downstream of this are the [Hardware](../Hardware/Requirements/) and [Software](../Software/Requirements/) requirements documents which go a layer lower than the system level behavior.

## Terminology
1. **Diagnostic Routine**: A routine that checks that a particular aspect of the system's behavior is operating within certain constraints
1. **Diagnostic Fault**: If a particular diagnostic routine's check fails
1. **DC**: "Direct Current"; signal with a frequency of zero
1. **Square Wave**: A signal that ideally only takes on two different voltage levels
1. **HIGH**: The higher of the two voltage levels that the output square wave signal is driven to
1. **LOW**: The lower of the two voltage levels that the output square wave signal is driven to
1. **High Pulse Time**: The amount of time that the output signal's square wave is HIGH, usually given in microseconds (µs)
1. **HPT**: Abbreviation for "High Pulse Time"
1. **Setpoint Mode**: The mode that indicates whether the user's potentiometer input will be setting output frequency or output high pulse time
1. **User Interface Device**: A button, slide switch, potentiometer, LED, display, or other device that is directly interacted with by a user
1. **Power Switch**: The switch that turns on or off the device
1. **Enable Switch**: The switch that enables or disables the output signal
1. **SetPot**: The potentiometer that configures the setpoint for either frequency or HPT
1. **Setpoint Mode Switch**: The switch that toggles the setpoint mode
1. **Mild Fault**: A fault that, although active, does not prevent the device from producing a valid output that may still be usable to a user
1. **Catastrophic Fault**: A fault that indicates the output signal is out-of-spec
1. **Coarse Mode**: The SetPot's range is mapped to the full range of either frequency or HPT
1. **Fine Mode**: The SetPot's range is mapped to ±10% of the frequency/HPT value that was present at the moment of switching into fine mode
1. **Coarseness Switch**: The switch to toggle between coarse and fine mode
1. **Fault Context Values**: Values that provide more context to a fault
1. **Screen Knob**: The knob that will be near the edge of the screen and used to scroll through the screen, set the two voltage levels of the output square wave, and support several other functions
1. **Main Screen**: The screen that includes the output waveform graphic as well as the frequency and HPT text fields
1. **Diagnostic Screen**: The screen that lays out the diagnostic faults and other information
1. **Voltage Setpoint Modes**: One of three modes:
   - Set Upper Voltage Level
   - Set Lower Voltage Level
   - Voltage Level Setpoint Off
1. **Voltage Setpoint Mode Button**: The button that cycles through the voltage setpoint modes
1. **Idle**: The device is on but its output is disabled
1. **Frequency Range**: The range of frequencies set by the minimum frequency and maximum frequency of `SYS_FUNC_TBD` with resolution set by `UI_TBD`.   
1. **High Pulse Time (HPT) Range**: The range of high pulse times set by the minimum HPT and maximum HPT of `SYS_FUNC_TBD` with resolution set by `UI_TBD`.   
1. **Max Voltage**: The maximum voltage that the device may output, set by `SYS_FUNC_TBD`
1. **Plot Screen**: The screen on the display that visualizes the signal to be output

## Functional Requirements
1. `SYS_FUNC_01`: The device shall output a square wave signal.  
   - `SYS_FUNC_01A`: The output waveform's HIGH voltage level shall be settable from 0.100V to a **Max Voltage** of 5.0V.  
   - `SYS_FUNC_01B`: The output waveform's LOW voltage level shall be settable from a **Minimum Voltage** of 0.0V to 0.100V _below_ Maximum Voltage.  
   - `SYS_FUNC_01C`: The output waveform's frequency shall be settable from **Minimum Frequency** of 0Hz to a **Maximum Frequency** of 12kHz.  
   - `SYS_FUNC_01D`: The output waveform's high pulse time shall be settable from a **Minimum HPT** of 0µs to a **Maximum HPT** of (period of frequency - 1µs).  
   - `SYS_FUNC_01E`: The output waveform's frequency shall be within ±1% of the set frequency.  
   - `SYS_FUNC_01F`: The output waveform's HPT shall be within ±0.5µs of the set HPT.  
   - `SYS_FUNC_01G`: The output waveform's two voltage levels shall be within ±2% of the set voltage levels when the voltage setpoint is ≥ 0.100V.  
   - Reference the [Hardware Requirements document](../../Hardware/Requirements/HardwareRequirements.md) for further specifications on the output signal (e.g., bandwidth, noise, etc.)
1. `SYS_FUNC_02`: Only one Setpoint Mode shall be active at a time.
1. `SYS_FUNC_03`: The device shall last ≥ 2 hours driving a 1kΩ (ideal) resistive load across the output frequency range specified in `SYS_FUNC_01C`.
1. `SYS_FUNC_04`: The device shall last ≥ 1 week idling.
1. `SYS_FUNC_05`: The present session shall be saved and reloaded at the start of the next session. Included in the session are:  
   - HIGH and LOW voltage levels (1)  
   - Frequency setpoint (2)  
   - HPT setpoint (3)  
   - Diagnostic faults (4)  
   - Coarseness setting (5)  
   - Setpoint mode (6)  
1. `SYS_FUNC_06`: The following session parameters/configurations shall be excluded from the session save specified in `SYS_FUNC_TBD`:  
   - Output enabled  
   - Active screen  
1. `SYS_FUNC_07`: At startup, the following shall be true:  
   - Output is disabled  
1. `SYS_FUNC_08`: The device shall support an output current up to ±50mA.
1. `SYS_FUNC_09`: The device shall keep track of time using a real-time clock, and support the user setting a time.
   - _Rationale_: The idea of this is to support time-stamping of faults

## Diagnostics
### Diagnostic Faults
Every diagnostic fault has a name, a description, a severity level, an associated diagnostic check, and one or more values that provide further context for the fault.
#### Mild
Generally speaking, each digital output (except the Enable LED) will have mild diagnostics associated with it. In addition, a few other mild diagnostics are defined.
1. `UI_SETPOT_LED_FRQ_INVALID`:  
   - Description: The state of the SetPot frequency selection LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `UI_SETPOT_LED_HPT_INVALID`:  
   - Description: The state of the SetPot HPT selection LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `UI_OUT_COARSENESS_LED_COARSE_INVALID`:  
   - Description: The state of the coarse LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `UI_OUT_COARSENESS_LED_FINE_INVALID`:  
   - Description: The state of the fine LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `DIAG_GREEN_LED_INVALID`:  
   - Description: The state of the green LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `DIAG_AMBER_LED_INVALID`:  
   - Description: The state of the amber LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `DIAG_RED_LED_INVALID`:  
   - Description: The state of the amber LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `SESSION_SAVE_FAILED`:  
   - Description: Saving the present session's settings failed to save to non-volatile memory  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: Indices of failed session items, as specified in `SYS_FUNC_04`  
1. `BATTERY_LOW`:
   - Description: The battery is ≤ 20% of its nominal open-circuit voltage
   - Associated Diagnostic Check: `SYS_DIAG_TBD`
   - Context Values: Battery voltage
1. `INTERNAL_COIN_CELL_LOW`:
   - Description: The internal coin cell battery for graceful shutdown is ≤ 25% of its nominal open-circuit voltage
   - Associated Diagnostic Check: `SYS_DIAG_TBD`
   - Context Values: Battery voltage

#### Catastrophic
Catastrophic faults will cause the output to be disabled. It is critical that false positives do **NOT** occur for the sake of user experience. It is also critical that false negatives do **NOT** occur for device reliability.
1. `FRQ_OUTPUT_FRQ_LOW`:  
   - Description: The output square wave signal's frequency is lower than the acceptable tolerance  
   - Associated Diagnostic Check: `SYS_DIAG_TBDA`  
   - Context Values:  
      - measured frequency  
      - setpoint frequency  
1. `FRQ_OUTPUT_FRQ_HIGH`  
   - Description: The output square wave signal's frequency is higher than the acceptable tolerance  
   - Associated Diagnostic Check: `SYS_DIAG_TBDA`  
   - Context Values:  
      - measured frequency  
      - setpoint frequency  
1. `FRQ_OUTPUT_FRQ_NILL`:  
   - Description: The output square wave signal's frequency is zero (it is DC) when the user has specified a non-zero frequency  
   - Associated Diagnostic Check: `SYS_DIAG_TBDA`  
   - Context Values: None  
1. `HPT_LOW`:  
   - Description: The output square wave signal's HPT is lower than the acceptable tolerance  
   - Associated Diagnostic Check: `SYS_DIAG_TBDC`  
   - Context Values:  
      - measured HPT  
      - setpoint HPT  
1. `HPT_HIGH`  
   - Description: The output square wave signal's HPT is higher than the acceptable tolerance  
   - Associated Diagnostic Check: `SYS_DIAG_TBDC`  
   - Context Values:  
      - measured HPT  
      - setpoint HPT  
1. `UPPER_VOLTAGE_OUT_OF_SPEC`:  
   - Description: The output square wave signal's HIGH voltage level is outside acceptable tolerance  
   - Associated Diagnostic Check: `SYS_DIAG_TBDB`  
   - Context Values:  
      - measured HIGH voltage  
      - setpoint HIGH voltage  
1. `LOWER_VOLTAGE_OUT_OF_SPEC`:  
   - Description: The output square wave signal's LOW voltage level is outside acceptable tolerance  
   - Associated Diagnostic Check: `SYS_DIAG_TBDB`  
   - Context Values:  
      - measured LOW voltage  
      - setpoint LOW voltage  
1. `OVERCURRENT`:  
   - Description: The output current is above device limitations  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: Measured current before disable  
1. `UI_OUT_EN_LED_INVALID`:  
   - Description: The state of the output enable LED differs from the intended drive  
   - Associated Diagnostic Check: `SYS_DIAG_TBD`  
   - Context Values: None  
1. `BATTERY_LOW_REPLACE_NOW`:
   - Description: The battery is ≤ 5% of its nominal open-circuit voltage
   - Associated Diagnostic Check: `SYS_DIAG_TBD`
   - Context Values: Battery voltage

### Requirements
1. `SYS_DIAG_01`: Catastrophic faults shall cause the output to be disabled.
1. `SYS_DIAG_02`: Mild faults shall **not** disable the output.
1. `SYS_DIAG_03`: The device shall store diagnostic faults in non-volatile memory.
1. `SYS_DIAG_04`: The diagnostic faults shall be retrievable through the following means:  
   - `SYS_DIAG_04A`: The display on the diagnostic screen  
   - `SYS_DIAG_04B`: (FUTURE) Over CAN  
   - `SYS_DIAG_04C`: (FUTURE) Over USB  
1. `SYS_DIAG_05`: Each fault shall have a name and one or more context values.
1. `SYS_DIAG_06`: The device shall monitor its square wave output and confirm that the waveform meets the following specs:  
   - `SYS_DIAG_06A`: Frequency is within the constraints defined by `SYS_FUNC_TBD`
   - `SYS_DIAG_06B`: Voltage levels within the constraints defined by `sys_func_tbd`  
   - `SYS_DIAG_06C`: HPT is within the constraints defined by `sys_func_tbd`  
1. `SYS_DIAG_07`: The device shall monitor its output current and if the current is ≥ 95% of the current specified in `SYS_FUNC_TBD` for ≥ 500ms, throw fault `OVERCURRENT`.
1. `SYS_DIAG_08`: For every digital output, the device shall confirm that the output level matches the intended drive to within 100ms.
1. `SYS_DIAG_09`: The device shall monitor display faults and record a separate fault for each fault reported by the display.
1. `SYS_DIAG_10`: The device shall support clearing of all faults from memory through the input specified in `UI_24`.
1. `SYS_DIAG_11`: (FUTURE) The device shall support timestamping faults.
1. `SYS_DIAG_12`: The device shall monitor its battery and report the following diagnostics:
   - `BATTERY_LOW` (see description above)
   - `BATTERY_LOW_REPLACE_NOW` (see description above)
      - In addition to the above description, the **Plot Screen** shall be replaced by a battery low indicator screen.

## User Interface
### Screens
1. `Plot Screen`: TBD
1. `Battery Low Replace Now Screen`: TBD
1. `Diagnostics Screen`: TBD

### Requirements
1. `UI_01`: The user shall turn on or off the device using a slide switch (power switch).
1. `UI_02`: The slide switch that turns on or off the device shall have at least two different mitigating measures to prevent accidental switching.
1. `UI_03`: The user shall enable or disable the output signal using a switch (enable switch).
1. `UI_04`: The output signal shall be made available through at least two different ports.
1. `UI_05`: The user shall set the desired output frequency using a potentiometer.
1. `UI_06`: The user shall set the desired output high pulse time using a potentiometer.
1. `UI_07`: There shall only be one potentiometer - the SetPot.
1. `UI_08`: The user shall be able to switch between setting frequency and setting HPT (i.e., select the setpoint mode) using a momentary push button (setpoint mode switch).
1. `UI_09`: There shall be two LEDs that indicate whether frequency or HPT is being set, or neither (both off).
1. `UI_10`: There shall be an amber LED that indicates a mild fault is active.
1. `UI_11`: There shall be a red LED that indicates a catastrophic fault is active.
1. `UI_12`: There shall be a green LED that indicates a no fault is active.
1. `UI_13`: There shall be a display to present various pieces of information and screens.
1. `UI_14`: Holding down the setpoint mode switch shall  
   - `UI_14A`: Cause the diagnostic screen to take over or to return to the main screen  
   - `UI_14B`: **NOT** cause the setpoint mode to change  
1. `UI_15`: The screen shall display the set frequency in text form at all times with unit `Hz`.
1. `UI_16`: The screen shall display the corresponding period in text form at all times with unit `us` or `ms`.  
1. `UI_17`: The screen shall display the set high pulse time in text form at all times with unit `us` or `ms`.
1. `UI_18`: The signal information described in the preceding requirements shall be in `000.000` format, with the appropriate unit.
   - `UI_18A`: No leading 0's
   - `UI_18B`: No decimal point or trailing 0's if the number being represented is an integer
   - `UI_18C`: If the number is a not an integer, then all three trailing digits after the decimal point shall be displayed
1. `UI_19`: The screen shall display a waveform graphic that represents the set output waveform. It shall:  
   - `UI_19A`: Include a separate horizontal axis for each frequency decade (0 to 10Hz, 10 to 100Hz, etc.) from 0 to 10kHz  
   - `UI_19B`: Only display one horizontal axis at a time  
   - `UI_19D`: Include a vertical axis that goes from 0.0V to (Max Voltage)  
   - `UI_19E`: Plot a period of the set square waveform against the two axes  
1. `UI_20`: The user shall be able to toggle between a coarse and a fine mode using a momentary push button (coarseness switch)  
   - `UI20A`: Coarse Mode:
      - `UI_20A1`: The frequency resolution shall be the greater of 1Hz or `(Maximum Frequency) / (2 ^ (ADC Resolution))`
      - `UI_20A2`: The HPT resolution shall be 1us
   - `UI20B`: Fine Mode:
      - `UI_20B1`: The frequency resolution shall be `(0.2 x Present Frequency) / (2 ^ (ADC Resolution))`, with `Present Frequency` being the frequency that was set the moment fine mode was entered into  
      - `UI_20B2`: The HPT resolution shall be be `(0.2 x Present HPT) / (2 ^ (ADC Resolution))`, with `Present HPT` being the HPT that set the moment fine mode was entered into
      - `UI_20B3`: (FUTURE) The fine resolution range shall be configurable from 1% to 50%.
1. `UI_21`: Pressing the voltage setpoint mode button shall cycle the screen through the three voltage setpoint modes.
1. `UI_22`: The voltage setpoint mode button shall be a momentary push button.
1. `UI_23`: The voltage setpoint modes shall be defined as follows:  
   - `UI_23A`: Set Upper Voltage Level:  
      - The top level of the square waveform shall take on a different color.  
      - The screen knob shall move the top of the waveform up or down.  
      - The voltage level shall be greater than 0.0V and less than or equal to (Max Voltage).   
   - `UI_23B`: Set Lower Voltage Level:  
      - The bottom level of the square waveform shall take on a different color.  
      - The screen knob shall move the bottom of the waveform up or down.  
      - The voltage level shall be greater than or equal to 0.0V and less than (Max Voltage).  
   - `UI_23C`: Voltage Level Setpoint Off:  
      - The screen knob does not affect either voltage level.  
1. `UI_24`: (FUTURE) The device shall support a sweep for each of frequency and HPT.  
1. `UI_23`: There shall be a green LED to indicate the output is enabled.
1. `UI_24`: All recorded faults in memory shall be cleared by holding the voltage setpoint mode button for 5 seconds.
1. `UI_25`: The screen shall be visible during normal office indoor lighting conditions.
1. `UI_26`: The screen shall be visible from a 120° viewing angle from all sides.
1. `UI_27`: At startup, the active screen shall be the **Plot Screen**.
   - `UI_27A`: The horizontal axis scale shall be set to the smallest decade range that a period of the (previously set) output signal can fit in
   - `UI_27B`: The vertical axis scale shall be set the full voltage range defined in `SYS_FUNC_TBD`
