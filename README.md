# Modbus_RTU_PSOC4

Modbus_RTU_PSOC4 is a Modbus RTU slave library for the PSOC4 micro controller with; PSOC Creator examples for the **CY8CKIT-049-42XX**,
Python modbus RTU master test scripts to check your devices ability to connect to a master device.    


The library is a modified version of uC_Modbus by Kim Taylor (https://github.com/kmtaylor) which is a port of libmodbus by Stéphane Raimbault (https://libmodbus.org/).
![PSOC4200](https://jpralves.net/img/mcb/cy8ckit_1.jpg)

## Example Projects
### Basic_1_Modbus_RTU_PSOC4
This example shows how to use a psoc CY8CKIT-049-42XX as a basic modbus RTU slave device.

The master will be able use the; *Write Single Register*, *Write Multiple
Registers*, *Read Holding Registers*, *Report Slave ID* function codes to read
and write data to the psoc4. Only register number 0 - 10 have been made available.

The CY8CKIT-049-42XX's on board USB serial device will write debugging 
messages to the computer whenever a modbus read or write message is 
processed (e.g "Register 0 Read from").

The LED on pin1.6 will illuminate when modbus data is being read/received.

#### Hardware Setup
![Basic_1_Setup](https://github.com/LukeYoung3000/Modbus_RTU_PSOC4/blob/dev_doc/example_projects/Basic_1_Modbus_RTU_PSOC4.cydsn/Basic_1_Setup.jpg)
1. CY8CKIT-049-42XX PSOC4 Dev Board
2. MAX485 RS485 to TTL Module
3. CH340 USB to RS485 Module

See DesignWideResources in PSOC Creator for Pin Out.

#### Testing
The folder *tests_python* contains the file *Basic_1_Modbus_RTU_PSOC4_Test.py* and the
*minimalmodbus* module. Enter your COMPORT number at the top of the script and run it to test
modbus connectivity.

## Future Improvements
* Create driver file for the UART Serial Communications Block (SCB) component.
Using the SCB for Modbus will save UDB resources for the application.
* Create driver file for a software timer instead of using a dedicated TCPWM component
for the Modbus timeout functionality.
* Setup a doxygen page for code documentation.

