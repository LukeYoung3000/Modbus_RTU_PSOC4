/*******************************************************************************
* File Name: MODBUS_TIMER.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the MODBUS_TIMER
*  component
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

uint8 MODBUS_TIMER_initVar = 0u;


/*******************************************************************************
* Function Name: MODBUS_TIMER_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default MODBUS_TIMER configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (MODBUS_TIMER__QUAD == MODBUS_TIMER_CONFIG)
        MODBUS_TIMER_CONTROL_REG = MODBUS_TIMER_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        MODBUS_TIMER_TRIG_CONTROL1_REG  = MODBUS_TIMER_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        MODBUS_TIMER_SetInterruptMode(MODBUS_TIMER_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        MODBUS_TIMER_SetCounterMode(MODBUS_TIMER_COUNT_DOWN);
        MODBUS_TIMER_WritePeriod(MODBUS_TIMER_QUAD_PERIOD_INIT_VALUE);
        MODBUS_TIMER_WriteCounter(MODBUS_TIMER_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (MODBUS_TIMER__QUAD == MODBUS_TIMER_CONFIG) */

    #if (MODBUS_TIMER__TIMER == MODBUS_TIMER_CONFIG)
        MODBUS_TIMER_CONTROL_REG = MODBUS_TIMER_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        MODBUS_TIMER_TRIG_CONTROL1_REG  = MODBUS_TIMER_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        MODBUS_TIMER_SetInterruptMode(MODBUS_TIMER_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        MODBUS_TIMER_WritePeriod(MODBUS_TIMER_TC_PERIOD_VALUE );

        #if (MODBUS_TIMER__COMPARE == MODBUS_TIMER_TC_COMP_CAP_MODE)
            MODBUS_TIMER_WriteCompare(MODBUS_TIMER_TC_COMPARE_VALUE);

            #if (1u == MODBUS_TIMER_TC_COMPARE_SWAP)
                MODBUS_TIMER_SetCompareSwap(1u);
                MODBUS_TIMER_WriteCompareBuf(MODBUS_TIMER_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == MODBUS_TIMER_TC_COMPARE_SWAP) */
        #endif  /* (MODBUS_TIMER__COMPARE == MODBUS_TIMER_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (MODBUS_TIMER_CY_TCPWM_V2 && MODBUS_TIMER_TIMER_UPDOWN_CNT_USED && !MODBUS_TIMER_CY_TCPWM_4000)
            MODBUS_TIMER_WriteCounter(1u);
        #elif(MODBUS_TIMER__COUNT_DOWN == MODBUS_TIMER_TC_COUNTER_MODE)
            MODBUS_TIMER_WriteCounter(MODBUS_TIMER_TC_PERIOD_VALUE);
        #else
            MODBUS_TIMER_WriteCounter(0u);
        #endif /* (MODBUS_TIMER_CY_TCPWM_V2 && MODBUS_TIMER_TIMER_UPDOWN_CNT_USED && !MODBUS_TIMER_CY_TCPWM_4000) */
    #endif  /* (MODBUS_TIMER__TIMER == MODBUS_TIMER_CONFIG) */

    #if (MODBUS_TIMER__PWM_SEL == MODBUS_TIMER_CONFIG)
        MODBUS_TIMER_CONTROL_REG = MODBUS_TIMER_CTRL_PWM_BASE_CONFIG;

        #if (MODBUS_TIMER__PWM_PR == MODBUS_TIMER_PWM_MODE)
            MODBUS_TIMER_CONTROL_REG |= MODBUS_TIMER_CTRL_PWM_RUN_MODE;
            MODBUS_TIMER_WriteCounter(MODBUS_TIMER_PWM_PR_INIT_VALUE);
        #else
            MODBUS_TIMER_CONTROL_REG |= MODBUS_TIMER_CTRL_PWM_ALIGN | MODBUS_TIMER_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (MODBUS_TIMER_CY_TCPWM_V2 && MODBUS_TIMER_PWM_UPDOWN_CNT_USED && !MODBUS_TIMER_CY_TCPWM_4000)
                MODBUS_TIMER_WriteCounter(1u);
            #elif (MODBUS_TIMER__RIGHT == MODBUS_TIMER_PWM_ALIGN)
                MODBUS_TIMER_WriteCounter(MODBUS_TIMER_PWM_PERIOD_VALUE);
            #else 
                MODBUS_TIMER_WriteCounter(0u);
            #endif  /* (MODBUS_TIMER_CY_TCPWM_V2 && MODBUS_TIMER_PWM_UPDOWN_CNT_USED && !MODBUS_TIMER_CY_TCPWM_4000) */
        #endif  /* (MODBUS_TIMER__PWM_PR == MODBUS_TIMER_PWM_MODE) */

        #if (MODBUS_TIMER__PWM_DT == MODBUS_TIMER_PWM_MODE)
            MODBUS_TIMER_CONTROL_REG |= MODBUS_TIMER_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (MODBUS_TIMER__PWM_DT == MODBUS_TIMER_PWM_MODE) */

        #if (MODBUS_TIMER__PWM == MODBUS_TIMER_PWM_MODE)
            MODBUS_TIMER_CONTROL_REG |= MODBUS_TIMER_CTRL_PWM_PRESCALER;
        #endif  /* (MODBUS_TIMER__PWM == MODBUS_TIMER_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        MODBUS_TIMER_TRIG_CONTROL1_REG  = MODBUS_TIMER_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        MODBUS_TIMER_SetInterruptMode(MODBUS_TIMER_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (MODBUS_TIMER__PWM_PR == MODBUS_TIMER_PWM_MODE)
            MODBUS_TIMER_TRIG_CONTROL2_REG =
                    (MODBUS_TIMER_CC_MATCH_NO_CHANGE    |
                    MODBUS_TIMER_OVERLOW_NO_CHANGE      |
                    MODBUS_TIMER_UNDERFLOW_NO_CHANGE);
        #else
            #if (MODBUS_TIMER__LEFT == MODBUS_TIMER_PWM_ALIGN)
                MODBUS_TIMER_TRIG_CONTROL2_REG = MODBUS_TIMER_PWM_MODE_LEFT;
            #endif  /* ( MODBUS_TIMER_PWM_LEFT == MODBUS_TIMER_PWM_ALIGN) */

            #if (MODBUS_TIMER__RIGHT == MODBUS_TIMER_PWM_ALIGN)
                MODBUS_TIMER_TRIG_CONTROL2_REG = MODBUS_TIMER_PWM_MODE_RIGHT;
            #endif  /* ( MODBUS_TIMER_PWM_RIGHT == MODBUS_TIMER_PWM_ALIGN) */

            #if (MODBUS_TIMER__CENTER == MODBUS_TIMER_PWM_ALIGN)
                MODBUS_TIMER_TRIG_CONTROL2_REG = MODBUS_TIMER_PWM_MODE_CENTER;
            #endif  /* ( MODBUS_TIMER_PWM_CENTER == MODBUS_TIMER_PWM_ALIGN) */

            #if (MODBUS_TIMER__ASYMMETRIC == MODBUS_TIMER_PWM_ALIGN)
                MODBUS_TIMER_TRIG_CONTROL2_REG = MODBUS_TIMER_PWM_MODE_ASYM;
            #endif  /* (MODBUS_TIMER__ASYMMETRIC == MODBUS_TIMER_PWM_ALIGN) */
        #endif  /* (MODBUS_TIMER__PWM_PR == MODBUS_TIMER_PWM_MODE) */

        /* Set other values from customizer */
        MODBUS_TIMER_WritePeriod(MODBUS_TIMER_PWM_PERIOD_VALUE );
        MODBUS_TIMER_WriteCompare(MODBUS_TIMER_PWM_COMPARE_VALUE);

        #if (1u == MODBUS_TIMER_PWM_COMPARE_SWAP)
            MODBUS_TIMER_SetCompareSwap(1u);
            MODBUS_TIMER_WriteCompareBuf(MODBUS_TIMER_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == MODBUS_TIMER_PWM_COMPARE_SWAP) */

        #if (1u == MODBUS_TIMER_PWM_PERIOD_SWAP)
            MODBUS_TIMER_SetPeriodSwap(1u);
            MODBUS_TIMER_WritePeriodBuf(MODBUS_TIMER_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == MODBUS_TIMER_PWM_PERIOD_SWAP) */
    #endif  /* (MODBUS_TIMER__PWM_SEL == MODBUS_TIMER_CONFIG) */
    
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_Enable
********************************************************************************
*
* Summary:
*  Enables the MODBUS_TIMER.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    MODBUS_TIMER_BLOCK_CONTROL_REG |= MODBUS_TIMER_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (MODBUS_TIMER__PWM_SEL == MODBUS_TIMER_CONFIG)
        #if (0u == MODBUS_TIMER_PWM_START_SIGNAL_PRESENT)
            MODBUS_TIMER_TriggerCommand(MODBUS_TIMER_MASK, MODBUS_TIMER_CMD_START);
        #endif /* (0u == MODBUS_TIMER_PWM_START_SIGNAL_PRESENT) */
    #endif /* (MODBUS_TIMER__PWM_SEL == MODBUS_TIMER_CONFIG) */

    #if (MODBUS_TIMER__TIMER == MODBUS_TIMER_CONFIG)
        #if (0u == MODBUS_TIMER_TC_START_SIGNAL_PRESENT)
            MODBUS_TIMER_TriggerCommand(MODBUS_TIMER_MASK, MODBUS_TIMER_CMD_START);
        #endif /* (0u == MODBUS_TIMER_TC_START_SIGNAL_PRESENT) */
    #endif /* (MODBUS_TIMER__TIMER == MODBUS_TIMER_CONFIG) */
    
    #if (MODBUS_TIMER__QUAD == MODBUS_TIMER_CONFIG)
        #if (0u != MODBUS_TIMER_QUAD_AUTO_START)
            MODBUS_TIMER_TriggerCommand(MODBUS_TIMER_MASK, MODBUS_TIMER_CMD_RELOAD);
        #endif /* (0u != MODBUS_TIMER_QUAD_AUTO_START) */
    #endif  /* (MODBUS_TIMER__QUAD == MODBUS_TIMER_CONFIG) */
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_Start
********************************************************************************
*
* Summary:
*  Initializes the MODBUS_TIMER with default customizer
*  values when called the first time and enables the MODBUS_TIMER.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  MODBUS_TIMER_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time MODBUS_TIMER_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the MODBUS_TIMER_Start() routine.
*
*******************************************************************************/
void MODBUS_TIMER_Start(void)
{
    if (0u == MODBUS_TIMER_initVar)
    {
        MODBUS_TIMER_Init();
        MODBUS_TIMER_initVar = 1u;
    }

    MODBUS_TIMER_Enable();
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_Stop
********************************************************************************
*
* Summary:
*  Disables the MODBUS_TIMER.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_BLOCK_CONTROL_REG &= (uint32)~MODBUS_TIMER_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the MODBUS_TIMER. This function is used when
*  configured as a generic MODBUS_TIMER and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the MODBUS_TIMER to operate in
*   Values:
*   - MODBUS_TIMER_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - MODBUS_TIMER_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - MODBUS_TIMER_MODE_QUAD - Quadrature decoder
*         - MODBUS_TIMER_MODE_PWM - PWM
*         - MODBUS_TIMER_MODE_PWM_DT - PWM with dead time
*         - MODBUS_TIMER_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_MODE_MASK;
    MODBUS_TIMER_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - MODBUS_TIMER_MODE_X1 - Counts on phi 1 rising
*         - MODBUS_TIMER_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - MODBUS_TIMER_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_QUAD_MODE_MASK;
    MODBUS_TIMER_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - MODBUS_TIMER_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - MODBUS_TIMER_PRESCALE_DIVBY2    - Divide by 2
*         - MODBUS_TIMER_PRESCALE_DIVBY4    - Divide by 4
*         - MODBUS_TIMER_PRESCALE_DIVBY8    - Divide by 8
*         - MODBUS_TIMER_PRESCALE_DIVBY16   - Divide by 16
*         - MODBUS_TIMER_PRESCALE_DIVBY32   - Divide by 32
*         - MODBUS_TIMER_PRESCALE_DIVBY64   - Divide by 64
*         - MODBUS_TIMER_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_PRESCALER_MASK;
    MODBUS_TIMER_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the MODBUS_TIMER runs
*  continuously or stops when terminal count is reached.  By default the
*  MODBUS_TIMER operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_ONESHOT_MASK;
    MODBUS_TIMER_CONTROL_REG |= ((uint32)((oneShotEnable & MODBUS_TIMER_1BIT_MASK) <<
                                                               MODBUS_TIMER_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPWMMode(uint32 modeMask)
{
    MODBUS_TIMER_TRIG_CONTROL2_REG = (modeMask & MODBUS_TIMER_6BIT_MASK);
}



/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_PWM_SYNC_KILL_MASK;
    MODBUS_TIMER_CONTROL_REG |= ((uint32)((syncKillEnable & MODBUS_TIMER_1BIT_MASK)  <<
                                               MODBUS_TIMER_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_PWM_STOP_KILL_MASK;
    MODBUS_TIMER_CONTROL_REG |= ((uint32)((stopOnKillEnable & MODBUS_TIMER_1BIT_MASK)  <<
                                                         MODBUS_TIMER_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_PRESCALER_MASK;
    MODBUS_TIMER_CONTROL_REG |= ((uint32)((deadTime & MODBUS_TIMER_8BIT_MASK) <<
                                                          MODBUS_TIMER_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - MODBUS_TIMER_INVERT_LINE   - Inverts the line output
*         - MODBUS_TIMER_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_INV_OUT_MASK;
    MODBUS_TIMER_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: MODBUS_TIMER_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_WriteCounter(uint32 count)
{
    MODBUS_TIMER_COUNTER_REG = (count & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadCounter(void)
{
    return (MODBUS_TIMER_COUNTER_REG & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - MODBUS_TIMER_COUNT_UP       - Counts up
*     - MODBUS_TIMER_COUNT_DOWN     - Counts down
*     - MODBUS_TIMER_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - MODBUS_TIMER_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_UPDOWN_MASK;
    MODBUS_TIMER_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_WritePeriod(uint32 period)
{
    MODBUS_TIMER_PERIOD_REG = (period & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadPeriod(void)
{
    return (MODBUS_TIMER_PERIOD_REG & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_RELOAD_CC_MASK;
    MODBUS_TIMER_CONTROL_REG |= (swapEnable & MODBUS_TIMER_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_WritePeriodBuf(uint32 periodBuf)
{
    MODBUS_TIMER_PERIOD_BUF_REG = (periodBuf & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadPeriodBuf(void)
{
    return (MODBUS_TIMER_PERIOD_BUF_REG & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_CONTROL_REG &= (uint32)~MODBUS_TIMER_RELOAD_PERIOD_MASK;
    MODBUS_TIMER_CONTROL_REG |= ((uint32)((swapEnable & MODBUS_TIMER_1BIT_MASK) <<
                                                            MODBUS_TIMER_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void MODBUS_TIMER_WriteCompare(uint32 compare)
{
    #if (MODBUS_TIMER_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */

    #if (MODBUS_TIMER_CY_TCPWM_4000)
        currentMode = ((MODBUS_TIMER_CONTROL_REG & MODBUS_TIMER_UPDOWN_MASK) >> MODBUS_TIMER_UPDOWN_SHIFT);

        if (((uint32)MODBUS_TIMER__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)MODBUS_TIMER__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */
    
    MODBUS_TIMER_COMP_CAP_REG = (compare & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadCompare(void)
{
    #if (MODBUS_TIMER_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */

    #if (MODBUS_TIMER_CY_TCPWM_4000)
        currentMode = ((MODBUS_TIMER_CONTROL_REG & MODBUS_TIMER_UPDOWN_MASK) >> MODBUS_TIMER_UPDOWN_SHIFT);
        
        regVal = MODBUS_TIMER_COMP_CAP_REG;
        
        if (((uint32)MODBUS_TIMER__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)MODBUS_TIMER__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & MODBUS_TIMER_16BIT_MASK);
    #else
        return (MODBUS_TIMER_COMP_CAP_REG & MODBUS_TIMER_16BIT_MASK);
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void MODBUS_TIMER_WriteCompareBuf(uint32 compareBuf)
{
    #if (MODBUS_TIMER_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */

    #if (MODBUS_TIMER_CY_TCPWM_4000)
        currentMode = ((MODBUS_TIMER_CONTROL_REG & MODBUS_TIMER_UPDOWN_MASK) >> MODBUS_TIMER_UPDOWN_SHIFT);

        if (((uint32)MODBUS_TIMER__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)MODBUS_TIMER__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */
    
    MODBUS_TIMER_COMP_CAP_BUF_REG = (compareBuf & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadCompareBuf(void)
{
    #if (MODBUS_TIMER_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */

    #if (MODBUS_TIMER_CY_TCPWM_4000)
        currentMode = ((MODBUS_TIMER_CONTROL_REG & MODBUS_TIMER_UPDOWN_MASK) >> MODBUS_TIMER_UPDOWN_SHIFT);

        regVal = MODBUS_TIMER_COMP_CAP_BUF_REG;
        
        if (((uint32)MODBUS_TIMER__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)MODBUS_TIMER__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & MODBUS_TIMER_16BIT_MASK);
    #else
        return (MODBUS_TIMER_COMP_CAP_BUF_REG & MODBUS_TIMER_16BIT_MASK);
    #endif /* (MODBUS_TIMER_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadCapture(void)
{
    return (MODBUS_TIMER_COMP_CAP_REG & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadCaptureBuf(void)
{
    return (MODBUS_TIMER_COMP_CAP_BUF_REG & MODBUS_TIMER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - MODBUS_TIMER_TRIG_LEVEL     - Level
*     - MODBUS_TIMER_TRIG_RISING    - Rising edge
*     - MODBUS_TIMER_TRIG_FALLING   - Falling edge
*     - MODBUS_TIMER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_TRIG_CONTROL1_REG &= (uint32)~MODBUS_TIMER_CAPTURE_MASK;
    MODBUS_TIMER_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - MODBUS_TIMER_TRIG_LEVEL     - Level
*     - MODBUS_TIMER_TRIG_RISING    - Rising edge
*     - MODBUS_TIMER_TRIG_FALLING   - Falling edge
*     - MODBUS_TIMER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_TRIG_CONTROL1_REG &= (uint32)~MODBUS_TIMER_RELOAD_MASK;
    MODBUS_TIMER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << MODBUS_TIMER_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - MODBUS_TIMER_TRIG_LEVEL     - Level
*     - MODBUS_TIMER_TRIG_RISING    - Rising edge
*     - MODBUS_TIMER_TRIG_FALLING   - Falling edge
*     - MODBUS_TIMER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_TRIG_CONTROL1_REG &= (uint32)~MODBUS_TIMER_START_MASK;
    MODBUS_TIMER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << MODBUS_TIMER_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - MODBUS_TIMER_TRIG_LEVEL     - Level
*     - MODBUS_TIMER_TRIG_RISING    - Rising edge
*     - MODBUS_TIMER_TRIG_FALLING   - Falling edge
*     - MODBUS_TIMER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_TRIG_CONTROL1_REG &= (uint32)~MODBUS_TIMER_STOP_MASK;
    MODBUS_TIMER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << MODBUS_TIMER_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - MODBUS_TIMER_TRIG_LEVEL     - Level
*     - MODBUS_TIMER_TRIG_RISING    - Rising edge
*     - MODBUS_TIMER_TRIG_FALLING   - Falling edge
*     - MODBUS_TIMER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_TRIG_CONTROL1_REG &= (uint32)~MODBUS_TIMER_COUNT_MASK;
    MODBUS_TIMER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << MODBUS_TIMER_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - MODBUS_TIMER_CMD_CAPTURE    - Trigger Capture/Switch command
*     - MODBUS_TIMER_CMD_RELOAD     - Trigger Reload/Index command
*     - MODBUS_TIMER_CMD_STOP       - Trigger Stop/Kill command
*     - MODBUS_TIMER_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    MODBUS_TIMER_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the MODBUS_TIMER.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - MODBUS_TIMER_STATUS_DOWN    - Set if counting down
*     - MODBUS_TIMER_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 MODBUS_TIMER_ReadStatus(void)
{
    return ((MODBUS_TIMER_STATUS_REG >> MODBUS_TIMER_RUNNING_STATUS_SHIFT) |
            (MODBUS_TIMER_STATUS_REG & MODBUS_TIMER_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - MODBUS_TIMER_INTR_MASK_TC       - Terminal count mask
*     - MODBUS_TIMER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetInterruptMode(uint32 interruptMask)
{
    MODBUS_TIMER_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - MODBUS_TIMER_INTR_MASK_TC       - Terminal count mask
*     - MODBUS_TIMER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 MODBUS_TIMER_GetInterruptSourceMasked(void)
{
    return (MODBUS_TIMER_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - MODBUS_TIMER_INTR_MASK_TC       - Terminal count mask
*     - MODBUS_TIMER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 MODBUS_TIMER_GetInterruptSource(void)
{
    return (MODBUS_TIMER_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - MODBUS_TIMER_INTR_MASK_TC       - Terminal count mask
*     - MODBUS_TIMER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_ClearInterrupt(uint32 interruptMask)
{
    MODBUS_TIMER_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: MODBUS_TIMER_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - MODBUS_TIMER_INTR_MASK_TC       - Terminal count mask
*     - MODBUS_TIMER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void MODBUS_TIMER_SetInterrupt(uint32 interruptMask)
{
    MODBUS_TIMER_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
