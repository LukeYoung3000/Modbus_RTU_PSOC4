/*******************************************************************************
* File Name: MODBUS_TIMER_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "MODBUS_TIMER.h"

static MODBUS_TIMER_BACKUP_STRUCT MODBUS_TIMER_backup;


/*******************************************************************************
* Function Name: MODBUS_TIMER_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: MODBUS_TIMER_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_Sleep(void)
{
    if(0u != (MODBUS_TIMER_BLOCK_CONTROL_REG & MODBUS_TIMER_MASK))
    {
        MODBUS_TIMER_backup.enableState = 1u;
    }
    else
    {
        MODBUS_TIMER_backup.enableState = 0u;
    }

    MODBUS_TIMER_Stop();
    MODBUS_TIMER_SaveConfig();
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: MODBUS_TIMER_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_Wakeup(void)
{
    MODBUS_TIMER_RestoreConfig();

    if(0u != MODBUS_TIMER_backup.enableState)
    {
        MODBUS_TIMER_Enable();
    }
}


/* [] END OF FILE */
