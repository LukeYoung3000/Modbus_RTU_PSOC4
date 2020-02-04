/*******************************************************************************
* File Name: MODBUS_TIMEOUT.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_MODBUS_TIMEOUT_H)
#define CY_ISR_MODBUS_TIMEOUT_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void MODBUS_TIMEOUT_Start(void);
void MODBUS_TIMEOUT_StartEx(cyisraddress address);
void MODBUS_TIMEOUT_Stop(void);

CY_ISR_PROTO(MODBUS_TIMEOUT_Interrupt);

void MODBUS_TIMEOUT_SetVector(cyisraddress address);
cyisraddress MODBUS_TIMEOUT_GetVector(void);

void MODBUS_TIMEOUT_SetPriority(uint8 priority);
uint8 MODBUS_TIMEOUT_GetPriority(void);

void MODBUS_TIMEOUT_Enable(void);
uint8 MODBUS_TIMEOUT_GetState(void);
void MODBUS_TIMEOUT_Disable(void);

void MODBUS_TIMEOUT_SetPending(void);
void MODBUS_TIMEOUT_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the MODBUS_TIMEOUT ISR. */
#define MODBUS_TIMEOUT_INTC_VECTOR            ((reg32 *) MODBUS_TIMEOUT__INTC_VECT)

/* Address of the MODBUS_TIMEOUT ISR priority. */
#define MODBUS_TIMEOUT_INTC_PRIOR             ((reg32 *) MODBUS_TIMEOUT__INTC_PRIOR_REG)

/* Priority of the MODBUS_TIMEOUT interrupt. */
#define MODBUS_TIMEOUT_INTC_PRIOR_NUMBER      MODBUS_TIMEOUT__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable MODBUS_TIMEOUT interrupt. */
#define MODBUS_TIMEOUT_INTC_SET_EN            ((reg32 *) MODBUS_TIMEOUT__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the MODBUS_TIMEOUT interrupt. */
#define MODBUS_TIMEOUT_INTC_CLR_EN            ((reg32 *) MODBUS_TIMEOUT__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the MODBUS_TIMEOUT interrupt state to pending. */
#define MODBUS_TIMEOUT_INTC_SET_PD            ((reg32 *) MODBUS_TIMEOUT__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the MODBUS_TIMEOUT interrupt. */
#define MODBUS_TIMEOUT_INTC_CLR_PD            ((reg32 *) MODBUS_TIMEOUT__INTC_CLR_PD_REG)



#endif /* CY_ISR_MODBUS_TIMEOUT_H */


/* [] END OF FILE */
