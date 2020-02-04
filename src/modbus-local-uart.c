/**
 * @file    modbus-local-uart.c
 * @brief   Provides modbus-local.h with local UART function defintions
 * @author  Luke Young (L S Young Electrical)
 * 
 * Slightly amended version of 
 * uC_Modbus by Kim Taylor <https://github.com/kmtaylor>
 * 
 * Based on libmodbus by Stéphane Raimbault <stephane.raimbault@gmail.com>
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
 * This file defines the UART related functions used by modbus.h/c and 
 * modbus-local.h . The functions are all specfic to the PSOC 4200 micro
 * controller and require the UART component in the PSOC creator 
 * project to have the exact name "RS485" additionally, the attached 
 * rx interrupt MUST be named "RS485_RX_ISR".
 * 
 * In order to buffer/store incoming UART data this file uses the ring_buf_s
 * data type from Kim Taylor's ringbuffer.h .
 * 
 * When porting the libray, all the functions in this this file 
 * MUST be adapted to the API of the micro controller architecture you are
 * useing.
 */

#include "modbus-local.h"
#include "ringbuffer.h"

volatile struct ring_buf_s rs_rd_buf = {0};

CY_ISR(rs485_rx_isr)
{
    while (RS485_GetRxBufferSize())
    {
        if (ring_buf_full(&rs_rd_buf))
            break;
        ring_buf_in(&rs_rd_buf, RS485_ReadRxData());
        modbus_timer_reset();
    }
}

void _modbus_UART_init(void)
{
    RS485_Start();
    RS485_RX_ISR_StartEx(rs485_rx_isr);
}

uint8_t modbus_get_byte(void)
{
    uint8_t byte;
    RS485_RX_ISR_Disable();
    byte = ring_buf_out(&rs_rd_buf);
    RS485_RX_ISR_Enable();
    return byte;
}

uint8_t modbus_read_ready(void)
{
    uint8_t status;
    RS485_RX_ISR_Disable();
    status = ring_buf_count(&rs_rd_buf);
    RS485_RX_ISR_Enable();
    return status;
}

void modbus_write_data(uint8_t *_data, uint8_t length)
{
    RS485_PutArray(_data, length);
}