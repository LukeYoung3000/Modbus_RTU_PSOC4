/*******************************************************************************
* File Name: RS485_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "RS485.h"


/***************************************
* Local data allocation
***************************************/

static RS485_BACKUP_STRUCT  RS485_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: RS485_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the RS485_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RS485_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RS485_SaveConfig(void)
{
    #if(RS485_CONTROL_REG_REMOVED == 0u)
        RS485_backup.cr = RS485_CONTROL_REG;
    #endif /* End RS485_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: RS485_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RS485_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling RS485_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void RS485_RestoreConfig(void)
{
    #if(RS485_CONTROL_REG_REMOVED == 0u)
        RS485_CONTROL_REG = RS485_backup.cr;
    #endif /* End RS485_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: RS485_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The RS485_Sleep() API saves the current component state. Then it
*  calls the RS485_Stop() function and calls 
*  RS485_SaveConfig() to save the hardware configuration.
*  Call the RS485_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RS485_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RS485_Sleep(void)
{
    #if(RS485_RX_ENABLED || RS485_HD_ENABLED)
        if((RS485_RXSTATUS_ACTL_REG  & RS485_INT_ENABLE) != 0u)
        {
            RS485_backup.enableState = 1u;
        }
        else
        {
            RS485_backup.enableState = 0u;
        }
    #else
        if((RS485_TXSTATUS_ACTL_REG  & RS485_INT_ENABLE) !=0u)
        {
            RS485_backup.enableState = 1u;
        }
        else
        {
            RS485_backup.enableState = 0u;
        }
    #endif /* End RS485_RX_ENABLED || RS485_HD_ENABLED*/

    RS485_Stop();
    RS485_SaveConfig();
}


/*******************************************************************************
* Function Name: RS485_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  RS485_Sleep() was called. The RS485_Wakeup() function
*  calls the RS485_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  RS485_Sleep() function was called, the RS485_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RS485_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RS485_Wakeup(void)
{
    RS485_RestoreConfig();
    #if( (RS485_RX_ENABLED) || (RS485_HD_ENABLED) )
        RS485_ClearRxBuffer();
    #endif /* End (RS485_RX_ENABLED) || (RS485_HD_ENABLED) */
    #if(RS485_TX_ENABLED || RS485_HD_ENABLED)
        RS485_ClearTxBuffer();
    #endif /* End RS485_TX_ENABLED || RS485_HD_ENABLED */

    if(RS485_backup.enableState != 0u)
    {
        RS485_Enable();
    }
}


/* [] END OF FILE */
