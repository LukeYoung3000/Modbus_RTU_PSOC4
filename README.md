# Modbus_RTU_PSOC4

Modbus_RTU_PSOC4 is a Modbus RTU slave library for the PSOC4 micro controller with PSOC Creator examples for the CY8CKIT-049-42XX.
The library is a modified version of uC_Modbus by Kim Taylor (https://github.com/kmtaylor) which is a port of libmodbus by Stéphane Raimbault (https://libmodbus.org/).
![PSOC4200](https://jpralves.net/img/mcb/cy8ckit_1.jpg)

## Example Projects
### Basic_1_Modbus_RTU_PSOC4
#### Hardware Setup
![Basic_1_Setup](Modbus_RTU_PSOC4/example_projects/Basic_1_Modbus_RTU_PSOC4.cydsn/Basic_1_Setup.jpg)



## Future Improvements
* Create driver file for the UART Serial Communications Block (SCB) component.
Using the SCB for Modbus will save UDB resources for the application.
* Create driver file for a software timer instead of using a dedicated TCPWM component
for the Modbus timeout functionality.

