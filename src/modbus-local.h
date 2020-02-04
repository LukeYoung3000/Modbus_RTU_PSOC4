/**
 * @file    modbus-local.h
 * @brief   Provides modbus.h/c with access to controllers HW UART and Timer
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
 * This file provides the modbus stack (modbus.h/c) with the fuction
 * prototypes used to access the UART and Timer on the host micro controller.
 * 
 * The function declations used in modbus-local.h are defined in the files
 * modbus-local-uart.c and modbus-local-timer.c
 * 
 * modbus-local.h also includes the following important defintions:
 * MODBUS_SLAVE_STRING  - This string will be returned to a modbus master when 
 * useing the modbus report ID function code (_FC_REPORT_SLAVE_ID).
 * MODBUS_MAX_PACKET_LENGTH - Defines the number of bytes the modbus stack is 
 * allocated. Any in/out message larger than this will result in an error. 
 * 
 * When porting this libary to a different micro controller platform 
 * all of these fuctions must be defined in terms of the new controllers API.
 */

#ifndef _MODBUS_LOCAL_H_
#define _MODBUS_LOCAL_H_

#include <project.h>
#include "modbus.h"

#define MODBUS_SLAVE_STRING "PSOC 4200 MODBUS SLAVE"
#define MODBUS_BRIDGE_ADDRESS 1

#define MODBUS_MAX_PACKET_LENGTH 64

#define MODBUS_PROCESS_CONFIRMATION 1
#define MODBUS_USE_FUNCTION_POINTERS 0

// UART Interface --------------------------------------------------------------
#define MODBUS_WRITE_FUNC modbus_write_data
#define MODBUS_READ_READY_FUNC modbus_read_ready
#define MODBUS_READ_FUNC modbus_get_byte
#define MODBUS_FORWARD_FUNC RS485_PutArray
#define MODBUS_PROCESS_FUNC modbus_respond

/**
 * @brief Gets and returns the oldest unread byte from the UART register.
 */
uint8_t modbus_get_byte(void);

/**
 * @brief Checks for available UART data.
 * @return Number of unread bytes in UART data register.
 */
uint8_t modbus_read_ready(void);

/**
 * @brief Writes bytes of data to UART
 * @param[in] _data: pointer to array of data byte/s. 
 * @param[in] length: number of data byte/s.  
 */
void modbus_write_data(uint8_t *_data, uint8_t length);

/**
  * @brief Includes all of the set up code required to start the UART interface.
  * This function is call in the API function modbus_init(slave_id).   
  */
void _modbus_UART_init(void);
// -----------------------------------------------------------------------------

// Timer Interface -------------------------------------------------------------
/**
  * @brief The _modbus_timer_finished variable must be set to 1 when the modbus
  * timer times out (overflows). This is generally done in a interupt service 
  * routine.
  */
volatile uint8_t _modbus_timer_finished;

/**
  * @brief Stops and resets the modbus timer.
  * This function must include _modbus_timer_finished = 0  
  */
void _modbus_timer_stop_and_reset(void);

/**
  * @brief Resets the modbus timer.
  */
void _modbus_timer_reset(void);

/**
  * @brief Starts the modbus timer.
  */
void _modbus_timer_start(void);

/**
  * @brief Includes all of the set up code to initialise the timer interface.
  * This function is called in the API function modbus_init(slave_id).
  */
void _modbus_timer_init(void);
// -----------------------------------------------------------------------------

/*
// Register Interface
#define MODBUS_PROCESS_FUNC modbus_respond
int8_t modbus_respond(uint8_t function, uint8_t *msg);
int8_t modbus_write_regs(uint8_t *msg, uint8_t function);
int8_t modbus_read_regs(uint8_t *msg);
*/

#endif