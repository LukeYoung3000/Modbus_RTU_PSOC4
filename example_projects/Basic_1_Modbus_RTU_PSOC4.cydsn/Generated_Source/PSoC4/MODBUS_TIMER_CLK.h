/*******************************************************************************
* File Name: MODBUS_TIMER_CLK.h
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

#if !defined(CY_CLOCK_MODBUS_TIMER_CLK_H)
#define CY_CLOCK_MODBUS_TIMER_CLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void MODBUS_TIMER_CLK_StartEx(uint32 alignClkDiv);
#define MODBUS_TIMER_CLK_Start() \
    MODBUS_TIMER_CLK_StartEx(MODBUS_TIMER_CLK__PA_DIV_ID)

#else

void MODBUS_TIMER_CLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void MODBUS_TIMER_CLK_Stop(void);

void MODBUS_TIMER_CLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 MODBUS_TIMER_CLK_GetDividerRegister(void);
uint8  MODBUS_TIMER_CLK_GetFractionalDividerRegister(void);

#define MODBUS_TIMER_CLK_Enable()                         MODBUS_TIMER_CLK_Start()
#define MODBUS_TIMER_CLK_Disable()                        MODBUS_TIMER_CLK_Stop()
#define MODBUS_TIMER_CLK_SetDividerRegister(clkDivider, reset)  \
    MODBUS_TIMER_CLK_SetFractionalDividerRegister((clkDivider), 0u)
#define MODBUS_TIMER_CLK_SetDivider(clkDivider)           MODBUS_TIMER_CLK_SetDividerRegister((clkDivider), 1u)
#define MODBUS_TIMER_CLK_SetDividerValue(clkDivider)      MODBUS_TIMER_CLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define MODBUS_TIMER_CLK_DIV_ID     MODBUS_TIMER_CLK__DIV_ID

#define MODBUS_TIMER_CLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define MODBUS_TIMER_CLK_CTRL_REG   (*(reg32 *)MODBUS_TIMER_CLK__CTRL_REGISTER)
#define MODBUS_TIMER_CLK_DIV_REG    (*(reg32 *)MODBUS_TIMER_CLK__DIV_REGISTER)

#define MODBUS_TIMER_CLK_CMD_DIV_SHIFT          (0u)
#define MODBUS_TIMER_CLK_CMD_PA_DIV_SHIFT       (8u)
#define MODBUS_TIMER_CLK_CMD_DISABLE_SHIFT      (30u)
#define MODBUS_TIMER_CLK_CMD_ENABLE_SHIFT       (31u)

#define MODBUS_TIMER_CLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << MODBUS_TIMER_CLK_CMD_DISABLE_SHIFT))
#define MODBUS_TIMER_CLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << MODBUS_TIMER_CLK_CMD_ENABLE_SHIFT))

#define MODBUS_TIMER_CLK_DIV_FRAC_MASK  (0x000000F8u)
#define MODBUS_TIMER_CLK_DIV_FRAC_SHIFT (3u)
#define MODBUS_TIMER_CLK_DIV_INT_MASK   (0xFFFFFF00u)
#define MODBUS_TIMER_CLK_DIV_INT_SHIFT  (8u)

#else 

#define MODBUS_TIMER_CLK_DIV_REG        (*(reg32 *)MODBUS_TIMER_CLK__REGISTER)
#define MODBUS_TIMER_CLK_ENABLE_REG     MODBUS_TIMER_CLK_DIV_REG
#define MODBUS_TIMER_CLK_DIV_FRAC_MASK  MODBUS_TIMER_CLK__FRAC_MASK
#define MODBUS_TIMER_CLK_DIV_FRAC_SHIFT (16u)
#define MODBUS_TIMER_CLK_DIV_INT_MASK   MODBUS_TIMER_CLK__DIVIDER_MASK
#define MODBUS_TIMER_CLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_MODBUS_TIMER_CLK_H) */

/* [] END OF FILE */
