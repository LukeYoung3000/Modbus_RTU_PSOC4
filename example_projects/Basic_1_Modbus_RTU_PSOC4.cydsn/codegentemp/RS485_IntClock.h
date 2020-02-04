/*******************************************************************************
* File Name: RS485_IntClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_RS485_IntClock_H)
#define CY_CLOCK_RS485_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void RS485_IntClock_StartEx(uint32 alignClkDiv);
#define RS485_IntClock_Start() \
    RS485_IntClock_StartEx(RS485_IntClock__PA_DIV_ID)

#else

void RS485_IntClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void RS485_IntClock_Stop(void);

void RS485_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 RS485_IntClock_GetDividerRegister(void);
uint8  RS485_IntClock_GetFractionalDividerRegister(void);

#define RS485_IntClock_Enable()                         RS485_IntClock_Start()
#define RS485_IntClock_Disable()                        RS485_IntClock_Stop()
#define RS485_IntClock_SetDividerRegister(clkDivider, reset)  \
    RS485_IntClock_SetFractionalDividerRegister((clkDivider), 0u)
#define RS485_IntClock_SetDivider(clkDivider)           RS485_IntClock_SetDividerRegister((clkDivider), 1u)
#define RS485_IntClock_SetDividerValue(clkDivider)      RS485_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define RS485_IntClock_DIV_ID     RS485_IntClock__DIV_ID

#define RS485_IntClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define RS485_IntClock_CTRL_REG   (*(reg32 *)RS485_IntClock__CTRL_REGISTER)
#define RS485_IntClock_DIV_REG    (*(reg32 *)RS485_IntClock__DIV_REGISTER)

#define RS485_IntClock_CMD_DIV_SHIFT          (0u)
#define RS485_IntClock_CMD_PA_DIV_SHIFT       (8u)
#define RS485_IntClock_CMD_DISABLE_SHIFT      (30u)
#define RS485_IntClock_CMD_ENABLE_SHIFT       (31u)

#define RS485_IntClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << RS485_IntClock_CMD_DISABLE_SHIFT))
#define RS485_IntClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << RS485_IntClock_CMD_ENABLE_SHIFT))

#define RS485_IntClock_DIV_FRAC_MASK  (0x000000F8u)
#define RS485_IntClock_DIV_FRAC_SHIFT (3u)
#define RS485_IntClock_DIV_INT_MASK   (0xFFFFFF00u)
#define RS485_IntClock_DIV_INT_SHIFT  (8u)

#else 

#define RS485_IntClock_DIV_REG        (*(reg32 *)RS485_IntClock__REGISTER)
#define RS485_IntClock_ENABLE_REG     RS485_IntClock_DIV_REG
#define RS485_IntClock_DIV_FRAC_MASK  RS485_IntClock__FRAC_MASK
#define RS485_IntClock_DIV_FRAC_SHIFT (16u)
#define RS485_IntClock_DIV_INT_MASK   RS485_IntClock__DIVIDER_MASK
#define RS485_IntClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_RS485_IntClock_H) */

/* [] END OF FILE */
