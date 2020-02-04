/**
 * @project Basic_1_Modbus_RTU_PSOC4
 * @file    main.c
 * @brief   Example for modbus_rtu_psoc4 libray
 * @author  Luke Young (L S Young Electrical)
 *
 * Slightly amended version of 
 * uC_Modbus by Kim Taylor <https://github.com/kmtaylor>
 * 
 * @copyright
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * @details
 * This example shows how to use a psoc CY8CKIT-049-42XX as a modbus RTU 
 * slave device.
 * 
 * When connected to a modbus master on pin0.1 and pin0.2
 * the master will be able use the; Write single register, Write multiple
 * registers, Read holding registers and Report slave ID function codes to read
 * and write data to the psoc4.
 * 
 * The CY8CKIT-049-42XX's on board USB serial device will write debuging 
 * messages to the computer whenever a modbus read or write message is 
 * processed (e.g "Register 0 Read from").
 * 
 * The on LED on pin1.6 will illuminate when modbus data is being read/received.
 */

#include <project.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../src/modbus.h"

/**
 * Modbus Register Access Interface
 * 
 * The following function declarations are used in modbus.c/h to read/write
 * values form this modbus slave device to the connected modbus master.
 * 
 * These declations are defined at the bottom of main.c . Descriptions of the 
 * three functions can be found above their defintions.
 * 
 * The functions can be placed in there own .c file however, defineing them
 * in main.c allows easy access to global variables in the application 
 * code without having to declare a number of extern varribles.
 */

int8_t modbus_respond(uint8_t function, uint8_t *msg);
int8_t modbus_write_regs(uint8_t *msg, uint8_t function);
int8_t modbus_read_regs(uint8_t *msg);

// Global Variable
uint16_t data[20];

int main()
{
    CyGlobalIntEnable;

    // Initialise Modbus Communications and Slave Address
    modbus_init(1);
    // Give Modbus Libray Access to Onboard LED
    modbus_rx_led(LED1_Write, MODBUS_ACTIVE_HIGH);
    modbus_tx_led(LED1_Write, MODBUS_ACTIVE_HIGH);

    // Set Modbus Timeout
    // (It Is Ok To Not Set The Timeout Value,
    // The Defult Timeout Peroid Will Be The Period Set In The TopDesign)
#define MODBUS_TIMER_CLK_SPEED_KHZ 1
    uint8_t modbus_timeout_ms = 200;
    MODBUS_TIMER_WritePeriod(MODBUS_TIMER_CLK_SPEED_KHZ * modbus_timeout_ms);

    // Set USBUart Debugging Component
    USB_UART_Start();
    USB_UART_UartPutString("Start_Up_USB_UART \r\n");

    // Blink LED for 1 Second
    LED1_Write(1);
    CyDelay(1000);
    LED1_Write(0);

    while (1)
    {
        // Run Modbus Libray
        modbus_poll();
    }
}

/**
  * @brief Allows the user to define the slaves executed code based on the
  * function code received by the modbus master.
  * @param[in] function: The function code received from modbus master.
  * See modbus.h for functioncode (FC) defintions.
  * @param[in] msg: Pointer to received modbus data frame.
  * @return The size of the modbus responce.
  * Return -1 if a problem or error occurs (e.g Invalid register range).
  */
int8_t modbus_respond(uint8_t function, uint8_t *msg)
{
    switch (function)
    {
    case _FC_WRITE_SINGLE_REGISTER:
    case _FC_WRITE_MULTIPLE_REGISTERS:
        return modbus_write_regs(msg, function);
    case _FC_READ_HOLDING_REGISTERS:
        return modbus_read_regs(msg);
    case _FC_REPORT_SLAVE_ID:
        return modbus_slave_id_response(msg);
    }
    return -1;
}

/**
  * @brief Modbus messages send from master to slave. This function allows the 
  * user to specify what code is executed when a Write single register (FC6) or 
  * Write multiple register (FC16) function code is received.
  * @param[in] function: The function code received from modbus master.
  * See modbus.h for functioncode (FC) defintions.
  * @param[in] msg: Pointer to received modbus data frame.
  * @return The size of the modbus responce.
  * Return -1 if a problem or error occurs (e.g Invalid register range).
  */
int8_t modbus_write_regs(uint8_t *msg, uint8_t function)
{
    mb_data_init(msg, function);

    if (mb_nr_regs > MAX_NR_REGS(_FC_WRITE_MULTIPLE_REGISTERS))
        return -1;

    while (mb_nr_regs)
    {
        switch (mb_address)
        {
        case 0:
            // This section of code is entered when the modbus master sends a
            // write register 0 request.

            // Update data[0] = to the value corresponding to modbus register 0
            data[0] = mb_data_next(msg);
            // USBUart debugging message
            USB_UART_UartPutString("Register 0 Written to \r\n");
            break;
        case 1:
            // This section of code is entered when the modbus master sends a
            // write register 1 request.

            // Update data[1] = to the value corresponding to modbus register 1
            data[1] = mb_data_next(msg);
            // USBUart debugging message
            USB_UART_UartPutString("Register 1 Written to \r\n");
            break;
        case 2:
            data[2] = mb_data_next(msg);
            USB_UART_UartPutString("Register 2 Written to \r\n");
            break;
        case 3:
            data[3] = mb_data_next(msg);
            USB_UART_UartPutString("Register 3 Written to \r\n");
            break;
        case 4:
            data[4] = mb_data_next(msg);
            USB_UART_UartPutString("Register 4 Written to \r\n");
            break;
        case 5:
            data[5] = mb_data_next(msg);
            USB_UART_UartPutString("Register 5 Written to \r\n");
            break;
        case 6:
            data[6] = mb_data_next(msg);
            USB_UART_UartPutString("Register 6 Written to \r\n");
            break;
        case 7:
            data[7] = mb_data_next(msg);
            USB_UART_UartPutString("Register 7 Written to \r\n");
            break;
        case 8:
            data[8] = mb_data_next(msg);
            USB_UART_UartPutString("Register 8 Written to \r\n");
            break;
        case 9:
            data[9] = mb_data_next(msg);
            USB_UART_UartPutString("Register 9 Written to \r\n");
            break;
        case 10:
            data[10] = mb_data_next(msg);
            USB_UART_UartPutString("Register 10 Written to \r\n");
            break;
        default:
            return -1;
        }
    }
    return MODBUS_WRITE_RESP_SIZE;
}

/**
  * @brief Modbus messages send from slave to master. This function allows the 
  * user to specify the code executed when a Read holding registers (FC3) 
  * message is received.
  * @param[in] msg: Pointer to received modbus data frame.
  * @return The size of the modbus responce.
  * Return -1 if a problem or error occurs (e.g Invalid register range).
  */
int8_t modbus_read_regs(uint8_t *msg)
{
    mb_data_init(msg, _FC_READ_HOLDING_REGISTERS);

    if (mb_nr_regs > MAX_NR_REGS(_FC_READ_HOLDING_REGISTERS))
        return -1;

    while (mb_nr_regs)
    {
        switch (mb_address)
        {
        case 0:
            // This section of code is entered when the modbus master sends a
            // read register 0 request.

            // Add the value "data[0]" to the responce/reply data frame
            mb_data_resp(msg, data[0]);
            // USBUart debugging message
            USB_UART_UartPutString("Register 0 Read from \r\n");
            break;
        case 1:
            // This section of code is entered when the modbus master sends a
            // read register 1 request.

            // Add the value "data[1]" to the responce/reply data frame
            mb_data_resp(msg, data[1]);
            // USBUart debugging message
            USB_UART_UartPutString("Register 1 Read from \r\n");
            break;
        case 2:
            mb_data_resp(msg, data[2]);
            USB_UART_UartPutString("Register 2 Read from \r\n");
            break;
        case 3:
            mb_data_resp(msg, data[3]);
            USB_UART_UartPutString("Register 3 Read from \r\n");
            break;
        case 4:
            mb_data_resp(msg, data[4]);
            USB_UART_UartPutString("Register 4 Read from \r\n");
            break;
        case 5:
            mb_data_resp(msg, data[5]);
            USB_UART_UartPutString("Register 5 Read from \r\n");
            break;
        case 6:
            mb_data_resp(msg, data[6]);
            USB_UART_UartPutString("Register 6 Read from \r\n");
            break;
        case 7:
            mb_data_resp(msg, data[7]);
            USB_UART_UartPutString("Register 7 Read from \r\n");
            break;
        case 8:
            mb_data_resp(msg, data[8]);
            USB_UART_UartPutString("Register 8 Read from \r\n");
            break;
        case 9:
            mb_data_resp(msg, data[9]);
            USB_UART_UartPutString("Register 9 Read from \r\n");
            break;
        case 10:
            mb_data_resp(msg, data[10]);
            USB_UART_UartPutString("Register 10 Read from \r\n");
            break;
        default:
            return -1;
        }
    }
    return mb_resp_bytes;
}
