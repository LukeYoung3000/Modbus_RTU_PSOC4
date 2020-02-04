/**
 * @file    modbus-local-timer-tcpwm.c
 * @brief   Provides modbus-local.h with timer function defintions
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
 * This file defines the timeing related functions used by modbus.h/c and 
 * modbus-local.h . The functions are all specfic to the PSOC 4200 micro
 * controller and require the timer counter component in the PSOC creator 
 * project to have the exact name "MODBUS_TIMER" additionally, the attached 
 * timer overflow external interrupt MUST be named "MODBUS_TIMEOUT".
 * 
 * When porting the libray, all the functions in this this file 
 * MUST be adapted to the API of the micro controller architecture you are
 * useing.
 */

#include "modbus-local.h"

CY_ISR(modbus_timeout)
{
    _modbus_timer_finished = 1;
    MODBUS_TIMER_ClearInterrupt(MODBUS_TIMER_INTR_MASK_TC);
}

void _modbus_timer_stop_and_reset(void)
{

    // Reset timer to 0 counts
    MODBUS_TIMER_TriggerCommand(MODBUS_TIMER_MASK, MODBUS_TIMER_CMD_RELOAD);

    // Wait a few CLK cycles:
    // Stopping the timer on the same CLK cycle as a reset will overide reset
    CyDelay(2);

    MODBUS_TIMER_Stop();
    //MODBUS_TIMER_TriggerCommand(MODBUS_TIMER_MASKMODBUS_TIMER_CMD_STOP);

    _modbus_timer_finished = 0;
}

void _modbus_timer_reset(void)
{
    MODBUS_TIMER_TriggerCommand(MODBUS_TIMER_MASK, MODBUS_TIMER_CMD_RELOAD);
}

void _modbus_timer_start(void)
{
    MODBUS_TIMER_Enable();
}

void _modbus_timer_init(void)
{
    MODBUS_TIMER_Init();
    MODBUS_TIMEOUT_StartEx(modbus_timeout);
}
