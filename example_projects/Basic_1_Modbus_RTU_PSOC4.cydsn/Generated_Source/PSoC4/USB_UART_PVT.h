/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_USB_UART_H)
#define CY_SCB_PVT_USB_UART_H

#include "USB_UART.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define USB_UART_SetI2CExtClkInterruptMode(interruptMask) USB_UART_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define USB_UART_ClearI2CExtClkInterruptSource(interruptMask) USB_UART_CLEAR_INTR_I2C_EC(interruptMask)
#define USB_UART_GetI2CExtClkInterruptSource()                (USB_UART_INTR_I2C_EC_REG)
#define USB_UART_GetI2CExtClkInterruptMode()                  (USB_UART_INTR_I2C_EC_MASK_REG)
#define USB_UART_GetI2CExtClkInterruptSourceMasked()          (USB_UART_INTR_I2C_EC_MASKED_REG)

#if (!USB_UART_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define USB_UART_SetSpiExtClkInterruptMode(interruptMask) \
                                                                USB_UART_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define USB_UART_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                USB_UART_CLEAR_INTR_SPI_EC(interruptMask)
    #define USB_UART_GetExtSpiClkInterruptSource()                 (USB_UART_INTR_SPI_EC_REG)
    #define USB_UART_GetExtSpiClkInterruptMode()                   (USB_UART_INTR_SPI_EC_MASK_REG)
    #define USB_UART_GetExtSpiClkInterruptSourceMasked()           (USB_UART_INTR_SPI_EC_MASKED_REG)
#endif /* (!USB_UART_CY_SCBIP_V1) */

#if(USB_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void USB_UART_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (USB_UART_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (USB_UART_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_USB_UART_CUSTOM_INTR_HANDLER)
    extern cyisraddress USB_UART_customIntrHandler;
#endif /* !defined (CY_REMOVE_USB_UART_CUSTOM_INTR_HANDLER) */
#endif /* (USB_UART_SCB_IRQ_INTERNAL) */

extern USB_UART_BACKUP_STRUCT USB_UART_backup;

#if(USB_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 USB_UART_scbMode;
    extern uint8 USB_UART_scbEnableWake;
    extern uint8 USB_UART_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 USB_UART_mode;
    extern uint8 USB_UART_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * USB_UART_rxBuffer;
    extern uint8   USB_UART_rxDataBits;
    extern uint32  USB_UART_rxBufferSize;

    extern volatile uint8 * USB_UART_txBuffer;
    extern uint8   USB_UART_txDataBits;
    extern uint32  USB_UART_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 USB_UART_numberOfAddr;
    extern uint8 USB_UART_subAddrSize;
#endif /* (USB_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (USB_UART_SCB_MODE_I2C_CONST_CFG || \
        USB_UART_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 USB_UART_IntrTxMask;
#endif /* (! (USB_UART_SCB_MODE_I2C_CONST_CFG || \
              USB_UART_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(USB_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define USB_UART_SCB_MODE_I2C_RUNTM_CFG     (USB_UART_SCB_MODE_I2C      == USB_UART_scbMode)
    #define USB_UART_SCB_MODE_SPI_RUNTM_CFG     (USB_UART_SCB_MODE_SPI      == USB_UART_scbMode)
    #define USB_UART_SCB_MODE_UART_RUNTM_CFG    (USB_UART_SCB_MODE_UART     == USB_UART_scbMode)
    #define USB_UART_SCB_MODE_EZI2C_RUNTM_CFG   (USB_UART_SCB_MODE_EZI2C    == USB_UART_scbMode)
    #define USB_UART_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (USB_UART_SCB_MODE_UNCONFIG == USB_UART_scbMode)

    /* Defines wakeup enable */
    #define USB_UART_SCB_WAKE_ENABLE_CHECK       (0u != USB_UART_scbEnableWake)
#endif /* (USB_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!USB_UART_CY_SCBIP_V1)
    #define USB_UART_SCB_PINS_NUMBER    (7u)
#else
    #define USB_UART_SCB_PINS_NUMBER    (2u)
#endif /* (!USB_UART_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_USB_UART_H) */


/* [] END OF FILE */
