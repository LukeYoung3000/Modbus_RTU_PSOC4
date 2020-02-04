/**
 * @file    modbus.h
 * @brief   Modbus RTU stack and API
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
 */

#ifndef _MODBUS_H_
#define _MODBUS_H_

#include <project.h>
#include "modbus-local.h"

/* Modbus function codes */
#define _FC_READ_COILS 0x01
#define _FC_READ_DISCRETE_INPUTS 0x02
#define _FC_READ_HOLDING_REGISTERS 0x03
#define _FC_READ_INPUT_REGISTERS 0x04
#define _FC_WRITE_SINGLE_COIL 0x05
#define _FC_WRITE_SINGLE_REGISTER 0x06
#define _FC_READ_EXCEPTION_STATUS 0x07
#define _FC_WRITE_MULTIPLE_COILS 0x0F
#define _FC_WRITE_MULTIPLE_REGISTERS 0x10
#define _FC_REPORT_SLAVE_ID 0x11
#define _FC_MASK_WRITE_REGISTER 0x16
#define _FC_WRITE_AND_READ_REGISTERS 0x17

/* Modbus exceptions */
#define MODBUS_EXCEPTION 0x80
enum
{
  MODBUS_EXCEPTION_ILLEGAL_FUNCTION = 0x01,
  MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
  MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
  MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
  MODBUS_EXCEPTION_ACKNOWLEDGE,
  MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
  MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
  MODBUS_EXCEPTION_MEMORY_PARITY,
  MODBUS_EXCEPTION_NOT_DEFINED,
  MODBUS_EXCEPTION_GATEWAY_PATH,
  MODBUS_EXCEPTION_GATEWAY_TARGET,
  MODBUS_EXCEPTION_MAX,
};

/* User API functions --------------------------------------------------------*/
#if !MODBUS_USE_FUNCTION_POINTERS
/**
 * @brief Initialises the modbus timer and UART and sets slave address.  
 * @param[in] slave_addr: Devices slave ID 
 */
extern void modbus_init(uint8_t slave_addr);
#endif

/**
 * @brief Runs the modbus stack. Should be called in function main.
 * @return Number of data bytes in the current modbus message
 * storage array (modbus_msg[]).
 */
extern uint8_t modbus_poll(void);

/**
  * @brief Resets the modbus timer.
  */
extern void modbus_timer_reset(void);

/**
  * @brief Passes an incoming modbus message to another UART inteface if the
  * slave address dose NOT match the address of this device.
  * @param[in] val: If val > 0 forwarding will be enabled.
  */
extern void modbus_enable_forwarding(uint8_t val);

#define MODBUS_ACTIVE_HIGH 1
#define MODBUS_ACTIVE_LOW 0

/**
  * @brief Gives the modbus stack access to an indictor LED. The LED is linked 
  * via a function pointer and will turn ON while a modbus message is being 
  * transmitted from this device. This function should be used in the setup/
  * initialising section of the main program.
  * @example
  * 
  * #define MODBUS_ACTIVE_HIGH 1
  * 
  * // Turn on LED
  * LED_Write(MODBUS_ACTIVE_HIGH);
  * 
  * // Pass LED control to modbus
  * modbus_tx_led(LED_Write, MODBUS_ACTIVE_HIGH);
  * 
  * @param[in] led_enable: Pointer to a function that controlles an LED pin.
  * @param[in] val: The value that when given to the fuction pointed to by
  * led_enable will result in an LED turning on.
  * Use MODBUS_ACTIVE_HIGH if the LED is active high and MODBUS_ACTIVE_LOW if 
  * the LED is active low.
  */
extern void modbus_tx_led(void (*led_enable)(uint8_t val), uint8_t val);

/**
  * @brief Gives the modbus stack access to an indictor LED. The LED is linked 
  * via a function pointer and will turn ON while a modbus message is being 
  * received from the master device. This function should be used in the setup/
  * initialising section of the main program.
  * @example
  * 
  * #define MODBUS_ACTIVE_HIGH 1
  * 
  * // Turn on LED
  * LED_Write(MODBUS_ACTIVE_HIGH);
  * 
  * // Pass LED control to modbus
  * modbus_rx_led(LED_Write, MODBUS_ACTIVE_HIGH);
  * 
  * @param[in] led_enable: Pointer to a function that controlles an LED pin.
  * @param[in] val: The value that when given to the fuction pointed to by
  * led_enable will result in an LED turning on.
  * Use MODBUS_ACTIVE_HIGH if the LED is active high and MODBUS_ACTIVE_LOW if 
  * the LED is active low.
  */
extern void modbus_rx_led(void (*led_enable)(uint8_t val), uint8_t val);

/* ---------------------------------------------------------------------------*/

/* Offsets from the user function perspective */
#define MODBUS_MSG_LEN_OFFSET 0
#define MODBUS_MSG_ADDR_OFFSET 0
#define MODBUS_MSG_NR_REGS_OFFSET 2
#define MODBUS_WRITE_RESP_SIZE 4

/* Helper functions */
extern volatile uint8_t _modbus_timer_finished;
extern uint16_t mb_address;
extern uint8_t mb_nr_regs;
extern uint8_t mb_resp_bytes;
extern int8_t modbus_slave_id_response(uint8_t *msg);
extern void mb_data_init(uint8_t *msg, uint8_t fn);
extern void mb_data_resp(uint8_t *msg, uint16_t val);
extern uint16_t mb_data_next(uint8_t *msg);

/* Limits for user function */
#define MAX_NR_REGS(fn) (fn == _FC_READ_HOLDING_REGISTERS ? (MODBUS_MAX_PACKET_LENGTH - 5) / 2 : fn == _FC_WRITE_MULTIPLE_REGISTERS ? (MODBUS_MAX_PACKET_LENGTH - 9) / 2 : 0)

#define MB_UINT16_T(ptr) ((uint16_t)*ptr << 8 | *(ptr + 1))

/* Function pointer defintions */
typedef void (*modbus_write_t)(const uint8_t *msg, uint8_t bytes);
typedef uint8_t (*modbus_read_t)(void);
typedef uint8_t (*modbus_read_ready_t)(void);
typedef int8_t (*modbus_process_t)(uint8_t function, uint8_t *msg);
typedef void (*modbus_forward_t)(const uint8_t *msg, uint8_t bytes);

#if MODBUS_USE_FUNCTION_POINTERS
extern void modbus_init(
    uint8_t slave_addr,
    modbus_write_t modbus_write,
    modbus_read_ready_t modbus_read_ready,
    modbus_read_t modbus_read,
    modbus_process_t modbus_process,
    modbus_forward_t modbus_forward);
#endif /* MODBUS_USE_FUNCTION_POINTERS */

#endif /* _MODBUS_H_ */