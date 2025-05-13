# System Requirements Document
## General Description
TODO

This particular document is the System Requirements Document, where top-level system behavior is specified, as well as critical user interface items. Downstream of this are the [Hardware](../Hardware/Requirements/) and [Software](../Software/Requirements/) requirements documents which go a layer lower than the system level behavior.

## Terminology
TODO
1. **Example**: example

## Functional Requirements
1. `SYS_FUNC_01`: TODO

## Diagnostics
### Diagnostic Faults
Every diagnostic fault has a name, a description, a severity level, an associated diagnostic check, and one or more values that provide further context for the fault.
#### Mild
Generally speaking, each digital output (except the Enable LED) will have mild diagnostics associated with it. In addition, a few other mild diagnostics are defined.
1. `EXAMPLE`:  
   - Description: example
   - Associated Diagnostic Check: `EXAMPLE`  
   - Context Values: None  

#### Catastrophic
Catastrophic faults will cause the output to be disabled. It is critical that false positives do **NOT** occur for the sake of user experience. It is also critical that false negatives do **NOT** occur for device reliability.
1. `EXAMPLE`:  
   - Description: example
   - Associated Diagnostic Check: `EXAMPLE`  
   - Context Values: None  

### Requirements
1. `SYS_DIAG_01`: TODO

## User Interface
### Screens
1. `Hi`: TODO
1. `Bye`: TODO
1. `Error`: TODO

### Requirements
1. `UI_01`: TODO
