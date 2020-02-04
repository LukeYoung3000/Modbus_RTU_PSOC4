/*******************************************************************************
* File Name: RS485.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_RS485_H)
#define CY_UART_RS485_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define RS485_RX_ENABLED                     (1u)
#define RS485_TX_ENABLED                     (1u)
#define RS485_HD_ENABLED                     (0u)
#define RS485_RX_INTERRUPT_ENABLED           (0u)
#define RS485_TX_INTERRUPT_ENABLED           (0u)
#define RS485_INTERNAL_CLOCK_USED            (1u)
#define RS485_RXHW_ADDRESS_ENABLED           (0u)
#define RS485_OVER_SAMPLE_COUNT              (8u)
#define RS485_PARITY_TYPE                    (0u)
#define RS485_PARITY_TYPE_SW                 (0u)
#define RS485_BREAK_DETECT                   (0u)
#define RS485_BREAK_BITS_TX                  (13u)
#define RS485_BREAK_BITS_RX                  (13u)
#define RS485_TXCLKGEN_DP                    (1u)
#define RS485_USE23POLLING                   (1u)
#define RS485_FLOW_CONTROL                   (0u)
#define RS485_CLK_FREQ                       (0u)
#define RS485_TX_BUFFER_SIZE                 (4u)
#define RS485_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define RS485_CONTROL_REG_REMOVED            (0u)
#else
    #define RS485_CONTROL_REG_REMOVED            (1u)
#endif /* End RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct RS485_backupStruct_
{
    uint8 enableState;

    #if(RS485_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End RS485_CONTROL_REG_REMOVED */

} RS485_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void RS485_Start(void) ;
void RS485_Stop(void) ;
uint8 RS485_ReadControlRegister(void) ;
void RS485_WriteControlRegister(uint8 control) ;

void RS485_Init(void) ;
void RS485_Enable(void) ;
void RS485_SaveConfig(void) ;
void RS485_RestoreConfig(void) ;
void RS485_Sleep(void) ;
void RS485_Wakeup(void) ;

/* Only if RX is enabled */
#if( (RS485_RX_ENABLED) || (RS485_HD_ENABLED) )

    #if (RS485_RX_INTERRUPT_ENABLED)
        #define RS485_EnableRxInt()  CyIntEnable (RS485_RX_VECT_NUM)
        #define RS485_DisableRxInt() CyIntDisable(RS485_RX_VECT_NUM)
        CY_ISR_PROTO(RS485_RXISR);
    #endif /* RS485_RX_INTERRUPT_ENABLED */

    void RS485_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void RS485_SetRxAddress1(uint8 address) ;
    void RS485_SetRxAddress2(uint8 address) ;

    void  RS485_SetRxInterruptMode(uint8 intSrc) ;
    uint8 RS485_ReadRxData(void) ;
    uint8 RS485_ReadRxStatus(void) ;
    uint8 RS485_GetChar(void) ;
    uint16 RS485_GetByte(void) ;
    uint8 RS485_GetRxBufferSize(void)
                                                            ;
    void RS485_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define RS485_GetRxInterruptSource   RS485_ReadRxStatus

#endif /* End (RS485_RX_ENABLED) || (RS485_HD_ENABLED) */

/* Only if TX is enabled */
#if(RS485_TX_ENABLED || RS485_HD_ENABLED)

    #if(RS485_TX_INTERRUPT_ENABLED)
        #define RS485_EnableTxInt()  CyIntEnable (RS485_TX_VECT_NUM)
        #define RS485_DisableTxInt() CyIntDisable(RS485_TX_VECT_NUM)
        #define RS485_SetPendingTxInt() CyIntSetPending(RS485_TX_VECT_NUM)
        #define RS485_ClearPendingTxInt() CyIntClearPending(RS485_TX_VECT_NUM)
        CY_ISR_PROTO(RS485_TXISR);
    #endif /* RS485_TX_INTERRUPT_ENABLED */

    void RS485_SetTxInterruptMode(uint8 intSrc) ;
    void RS485_WriteTxData(uint8 txDataByte) ;
    uint8 RS485_ReadTxStatus(void) ;
    void RS485_PutChar(uint8 txDataByte) ;
    void RS485_PutString(const char8 string[]) ;
    void RS485_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void RS485_PutCRLF(uint8 txDataByte) ;
    void RS485_ClearTxBuffer(void) ;
    void RS485_SetTxAddressMode(uint8 addressMode) ;
    void RS485_SendBreak(uint8 retMode) ;
    uint8 RS485_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define RS485_PutStringConst         RS485_PutString
    #define RS485_PutArrayConst          RS485_PutArray
    #define RS485_GetTxInterruptSource   RS485_ReadTxStatus

#endif /* End RS485_TX_ENABLED || RS485_HD_ENABLED */

#if(RS485_HD_ENABLED)
    void RS485_LoadRxConfig(void) ;
    void RS485_LoadTxConfig(void) ;
#endif /* End RS485_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RS485) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    RS485_CyBtldrCommStart(void) CYSMALL ;
    void    RS485_CyBtldrCommStop(void) CYSMALL ;
    void    RS485_CyBtldrCommReset(void) CYSMALL ;
    cystatus RS485_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus RS485_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RS485)
        #define CyBtldrCommStart    RS485_CyBtldrCommStart
        #define CyBtldrCommStop     RS485_CyBtldrCommStop
        #define CyBtldrCommReset    RS485_CyBtldrCommReset
        #define CyBtldrCommWrite    RS485_CyBtldrCommWrite
        #define CyBtldrCommRead     RS485_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RS485) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define RS485_BYTE2BYTE_TIME_OUT (25u)
    #define RS485_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define RS485_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define RS485_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define RS485_SET_SPACE      (0x00u)
#define RS485_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (RS485_TX_ENABLED) || (RS485_HD_ENABLED) )
    #if(RS485_TX_INTERRUPT_ENABLED)
        #define RS485_TX_VECT_NUM            (uint8)RS485_TXInternalInterrupt__INTC_NUMBER
        #define RS485_TX_PRIOR_NUM           (uint8)RS485_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RS485_TX_INTERRUPT_ENABLED */

    #define RS485_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define RS485_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define RS485_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(RS485_TX_ENABLED)
        #define RS485_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (RS485_HD_ENABLED) */
        #define RS485_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (RS485_TX_ENABLED) */

    #define RS485_TX_STS_COMPLETE            (uint8)(0x01u << RS485_TX_STS_COMPLETE_SHIFT)
    #define RS485_TX_STS_FIFO_EMPTY          (uint8)(0x01u << RS485_TX_STS_FIFO_EMPTY_SHIFT)
    #define RS485_TX_STS_FIFO_FULL           (uint8)(0x01u << RS485_TX_STS_FIFO_FULL_SHIFT)
    #define RS485_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << RS485_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (RS485_TX_ENABLED) || (RS485_HD_ENABLED)*/

#if( (RS485_RX_ENABLED) || (RS485_HD_ENABLED) )
    #if(RS485_RX_INTERRUPT_ENABLED)
        #define RS485_RX_VECT_NUM            (uint8)RS485_RXInternalInterrupt__INTC_NUMBER
        #define RS485_RX_PRIOR_NUM           (uint8)RS485_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RS485_RX_INTERRUPT_ENABLED */
    #define RS485_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define RS485_RX_STS_BREAK_SHIFT             (0x01u)
    #define RS485_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define RS485_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define RS485_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define RS485_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define RS485_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define RS485_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define RS485_RX_STS_MRKSPC           (uint8)(0x01u << RS485_RX_STS_MRKSPC_SHIFT)
    #define RS485_RX_STS_BREAK            (uint8)(0x01u << RS485_RX_STS_BREAK_SHIFT)
    #define RS485_RX_STS_PAR_ERROR        (uint8)(0x01u << RS485_RX_STS_PAR_ERROR_SHIFT)
    #define RS485_RX_STS_STOP_ERROR       (uint8)(0x01u << RS485_RX_STS_STOP_ERROR_SHIFT)
    #define RS485_RX_STS_OVERRUN          (uint8)(0x01u << RS485_RX_STS_OVERRUN_SHIFT)
    #define RS485_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << RS485_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define RS485_RX_STS_ADDR_MATCH       (uint8)(0x01u << RS485_RX_STS_ADDR_MATCH_SHIFT)
    #define RS485_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << RS485_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define RS485_RX_HW_MASK                     (0x7Fu)
#endif /* End (RS485_RX_ENABLED) || (RS485_HD_ENABLED) */

/* Control Register definitions */
#define RS485_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define RS485_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define RS485_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define RS485_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define RS485_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define RS485_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define RS485_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define RS485_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define RS485_CTRL_HD_SEND               (uint8)(0x01u << RS485_CTRL_HD_SEND_SHIFT)
#define RS485_CTRL_HD_SEND_BREAK         (uint8)(0x01u << RS485_CTRL_HD_SEND_BREAK_SHIFT)
#define RS485_CTRL_MARK                  (uint8)(0x01u << RS485_CTRL_MARK_SHIFT)
#define RS485_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << RS485_CTRL_PARITY_TYPE0_SHIFT)
#define RS485_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << RS485_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define RS485_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define RS485_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define RS485_SEND_BREAK                         (0x00u)
#define RS485_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define RS485_REINIT                             (0x02u)
#define RS485_SEND_WAIT_REINIT                   (0x03u)

#define RS485_OVER_SAMPLE_8                      (8u)
#define RS485_OVER_SAMPLE_16                     (16u)

#define RS485_BIT_CENTER                         (RS485_OVER_SAMPLE_COUNT - 2u)

#define RS485_FIFO_LENGTH                        (4u)
#define RS485_NUMBER_OF_START_BIT                (1u)
#define RS485_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define RS485_TXBITCTR_BREAKBITS8X   ((RS485_BREAK_BITS_TX * RS485_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define RS485_TXBITCTR_BREAKBITS ((RS485_BREAK_BITS_TX * RS485_OVER_SAMPLE_COUNT) - 1u)

#define RS485_HALF_BIT_COUNT   \
                            (((RS485_OVER_SAMPLE_COUNT / 2u) + (RS485_USE23POLLING * 1u)) - 2u)
#if (RS485_OVER_SAMPLE_COUNT == RS485_OVER_SAMPLE_8)
    #define RS485_HD_TXBITCTR_INIT   (((RS485_BREAK_BITS_TX + \
                            RS485_NUMBER_OF_START_BIT) * RS485_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define RS485_RXBITCTR_INIT  ((((RS485_BREAK_BITS_RX + RS485_NUMBER_OF_START_BIT) \
                            * RS485_OVER_SAMPLE_COUNT) + RS485_HALF_BIT_COUNT) - 1u)

#else /* RS485_OVER_SAMPLE_COUNT == RS485_OVER_SAMPLE_16 */
    #define RS485_HD_TXBITCTR_INIT   ((8u * RS485_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define RS485_RXBITCTR_INIT      (((7u * RS485_OVER_SAMPLE_COUNT) - 1u) + \
                                                      RS485_HALF_BIT_COUNT)
#endif /* End RS485_OVER_SAMPLE_COUNT */

#define RS485_HD_RXBITCTR_INIT                   RS485_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 RS485_initVar;
#if (RS485_TX_INTERRUPT_ENABLED && RS485_TX_ENABLED)
    extern volatile uint8 RS485_txBuffer[RS485_TX_BUFFER_SIZE];
    extern volatile uint8 RS485_txBufferRead;
    extern uint8 RS485_txBufferWrite;
#endif /* (RS485_TX_INTERRUPT_ENABLED && RS485_TX_ENABLED) */
#if (RS485_RX_INTERRUPT_ENABLED && (RS485_RX_ENABLED || RS485_HD_ENABLED))
    extern uint8 RS485_errorStatus;
    extern volatile uint8 RS485_rxBuffer[RS485_RX_BUFFER_SIZE];
    extern volatile uint8 RS485_rxBufferRead;
    extern volatile uint8 RS485_rxBufferWrite;
    extern volatile uint8 RS485_rxBufferLoopDetect;
    extern volatile uint8 RS485_rxBufferOverflow;
    #if (RS485_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 RS485_rxAddressMode;
        extern volatile uint8 RS485_rxAddressDetected;
    #endif /* (RS485_RXHW_ADDRESS_ENABLED) */
#endif /* (RS485_RX_INTERRUPT_ENABLED && (RS485_RX_ENABLED || RS485_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define RS485__B_UART__AM_SW_BYTE_BYTE 1
#define RS485__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define RS485__B_UART__AM_HW_BYTE_BY_BYTE 3
#define RS485__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define RS485__B_UART__AM_NONE 0

#define RS485__B_UART__NONE_REVB 0
#define RS485__B_UART__EVEN_REVB 1
#define RS485__B_UART__ODD_REVB 2
#define RS485__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define RS485_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define RS485_NUMBER_OF_STOP_BITS    (1u)

#if (RS485_RXHW_ADDRESS_ENABLED)
    #define RS485_RX_ADDRESS_MODE    (0u)
    #define RS485_RX_HW_ADDRESS1     (0u)
    #define RS485_RX_HW_ADDRESS2     (0u)
#endif /* (RS485_RXHW_ADDRESS_ENABLED) */

#define RS485_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << RS485_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << RS485_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << RS485_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << RS485_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << RS485_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << RS485_RX_STS_BREAK_SHIFT) \
                                        | (0 << RS485_RX_STS_OVERRUN_SHIFT))

#define RS485_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << RS485_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << RS485_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << RS485_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << RS485_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RS485_CONTROL_REG \
                            (* (reg8 *) RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define RS485_CONTROL_PTR \
                            (  (reg8 *) RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RS485_TX_ENABLED)
    #define RS485_TXDATA_REG          (* (reg8 *) RS485_BUART_sTX_TxShifter_u0__F0_REG)
    #define RS485_TXDATA_PTR          (  (reg8 *) RS485_BUART_sTX_TxShifter_u0__F0_REG)
    #define RS485_TXDATA_AUX_CTL_REG  (* (reg8 *) RS485_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RS485_TXDATA_AUX_CTL_PTR  (  (reg8 *) RS485_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RS485_TXSTATUS_REG        (* (reg8 *) RS485_BUART_sTX_TxSts__STATUS_REG)
    #define RS485_TXSTATUS_PTR        (  (reg8 *) RS485_BUART_sTX_TxSts__STATUS_REG)
    #define RS485_TXSTATUS_MASK_REG   (* (reg8 *) RS485_BUART_sTX_TxSts__MASK_REG)
    #define RS485_TXSTATUS_MASK_PTR   (  (reg8 *) RS485_BUART_sTX_TxSts__MASK_REG)
    #define RS485_TXSTATUS_ACTL_REG   (* (reg8 *) RS485_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define RS485_TXSTATUS_ACTL_PTR   (  (reg8 *) RS485_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(RS485_TXCLKGEN_DP)
        #define RS485_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RS485_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RS485_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define RS485_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define RS485_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RS485_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RS485_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RS485_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RS485_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define RS485_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) RS485_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* RS485_TXCLKGEN_DP */

#endif /* End RS485_TX_ENABLED */

#if(RS485_HD_ENABLED)

    #define RS485_TXDATA_REG             (* (reg8 *) RS485_BUART_sRX_RxShifter_u0__F1_REG )
    #define RS485_TXDATA_PTR             (  (reg8 *) RS485_BUART_sRX_RxShifter_u0__F1_REG )
    #define RS485_TXDATA_AUX_CTL_REG     (* (reg8 *) RS485_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define RS485_TXDATA_AUX_CTL_PTR     (  (reg8 *) RS485_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RS485_TXSTATUS_REG           (* (reg8 *) RS485_BUART_sRX_RxSts__STATUS_REG )
    #define RS485_TXSTATUS_PTR           (  (reg8 *) RS485_BUART_sRX_RxSts__STATUS_REG )
    #define RS485_TXSTATUS_MASK_REG      (* (reg8 *) RS485_BUART_sRX_RxSts__MASK_REG )
    #define RS485_TXSTATUS_MASK_PTR      (  (reg8 *) RS485_BUART_sRX_RxSts__MASK_REG )
    #define RS485_TXSTATUS_ACTL_REG      (* (reg8 *) RS485_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RS485_TXSTATUS_ACTL_PTR      (  (reg8 *) RS485_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End RS485_HD_ENABLED */

#if( (RS485_RX_ENABLED) || (RS485_HD_ENABLED) )
    #define RS485_RXDATA_REG             (* (reg8 *) RS485_BUART_sRX_RxShifter_u0__F0_REG )
    #define RS485_RXDATA_PTR             (  (reg8 *) RS485_BUART_sRX_RxShifter_u0__F0_REG )
    #define RS485_RXADDRESS1_REG         (* (reg8 *) RS485_BUART_sRX_RxShifter_u0__D0_REG )
    #define RS485_RXADDRESS1_PTR         (  (reg8 *) RS485_BUART_sRX_RxShifter_u0__D0_REG )
    #define RS485_RXADDRESS2_REG         (* (reg8 *) RS485_BUART_sRX_RxShifter_u0__D1_REG )
    #define RS485_RXADDRESS2_PTR         (  (reg8 *) RS485_BUART_sRX_RxShifter_u0__D1_REG )
    #define RS485_RXDATA_AUX_CTL_REG     (* (reg8 *) RS485_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RS485_RXBITCTR_PERIOD_REG    (* (reg8 *) RS485_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RS485_RXBITCTR_PERIOD_PTR    (  (reg8 *) RS485_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RS485_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RS485_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RS485_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RS485_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RS485_RXBITCTR_COUNTER_REG   (* (reg8 *) RS485_BUART_sRX_RxBitCounter__COUNT_REG )
    #define RS485_RXBITCTR_COUNTER_PTR   (  (reg8 *) RS485_BUART_sRX_RxBitCounter__COUNT_REG )

    #define RS485_RXSTATUS_REG           (* (reg8 *) RS485_BUART_sRX_RxSts__STATUS_REG )
    #define RS485_RXSTATUS_PTR           (  (reg8 *) RS485_BUART_sRX_RxSts__STATUS_REG )
    #define RS485_RXSTATUS_MASK_REG      (* (reg8 *) RS485_BUART_sRX_RxSts__MASK_REG )
    #define RS485_RXSTATUS_MASK_PTR      (  (reg8 *) RS485_BUART_sRX_RxSts__MASK_REG )
    #define RS485_RXSTATUS_ACTL_REG      (* (reg8 *) RS485_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RS485_RXSTATUS_ACTL_PTR      (  (reg8 *) RS485_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (RS485_RX_ENABLED) || (RS485_HD_ENABLED) */

#if(RS485_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define RS485_INTCLOCK_CLKEN_REG     (* (reg8 *) RS485_IntClock__PM_ACT_CFG)
    #define RS485_INTCLOCK_CLKEN_PTR     (  (reg8 *) RS485_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define RS485_INTCLOCK_CLKEN_MASK    RS485_IntClock__PM_ACT_MSK
#endif /* End RS485_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(RS485_TX_ENABLED)
    #define RS485_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End RS485_TX_ENABLED */

#if(RS485_HD_ENABLED)
    #define RS485_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End RS485_HD_ENABLED */

#if( (RS485_RX_ENABLED) || (RS485_HD_ENABLED) )
    #define RS485_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (RS485_RX_ENABLED) || (RS485_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define RS485_WAIT_1_MS      RS485_BL_CHK_DELAY_MS   

#define RS485_TXBUFFERSIZE   RS485_TX_BUFFER_SIZE
#define RS485_RXBUFFERSIZE   RS485_RX_BUFFER_SIZE

#if (RS485_RXHW_ADDRESS_ENABLED)
    #define RS485_RXADDRESSMODE  RS485_RX_ADDRESS_MODE
    #define RS485_RXHWADDRESS1   RS485_RX_HW_ADDRESS1
    #define RS485_RXHWADDRESS2   RS485_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define RS485_RXAddressMode  RS485_RXADDRESSMODE
#endif /* (RS485_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define RS485_initvar                    RS485_initVar

#define RS485_RX_Enabled                 RS485_RX_ENABLED
#define RS485_TX_Enabled                 RS485_TX_ENABLED
#define RS485_HD_Enabled                 RS485_HD_ENABLED
#define RS485_RX_IntInterruptEnabled     RS485_RX_INTERRUPT_ENABLED
#define RS485_TX_IntInterruptEnabled     RS485_TX_INTERRUPT_ENABLED
#define RS485_InternalClockUsed          RS485_INTERNAL_CLOCK_USED
#define RS485_RXHW_Address_Enabled       RS485_RXHW_ADDRESS_ENABLED
#define RS485_OverSampleCount            RS485_OVER_SAMPLE_COUNT
#define RS485_ParityType                 RS485_PARITY_TYPE

#if( RS485_TX_ENABLED && (RS485_TXBUFFERSIZE > RS485_FIFO_LENGTH))
    #define RS485_TXBUFFER               RS485_txBuffer
    #define RS485_TXBUFFERREAD           RS485_txBufferRead
    #define RS485_TXBUFFERWRITE          RS485_txBufferWrite
#endif /* End RS485_TX_ENABLED */
#if( ( RS485_RX_ENABLED || RS485_HD_ENABLED ) && \
     (RS485_RXBUFFERSIZE > RS485_FIFO_LENGTH) )
    #define RS485_RXBUFFER               RS485_rxBuffer
    #define RS485_RXBUFFERREAD           RS485_rxBufferRead
    #define RS485_RXBUFFERWRITE          RS485_rxBufferWrite
    #define RS485_RXBUFFERLOOPDETECT     RS485_rxBufferLoopDetect
    #define RS485_RXBUFFER_OVERFLOW      RS485_rxBufferOverflow
#endif /* End RS485_RX_ENABLED */

#ifdef RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RS485_CONTROL                RS485_CONTROL_REG
#endif /* End RS485_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RS485_TX_ENABLED)
    #define RS485_TXDATA                 RS485_TXDATA_REG
    #define RS485_TXSTATUS               RS485_TXSTATUS_REG
    #define RS485_TXSTATUS_MASK          RS485_TXSTATUS_MASK_REG
    #define RS485_TXSTATUS_ACTL          RS485_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(RS485_TXCLKGEN_DP)
        #define RS485_TXBITCLKGEN_CTR        RS485_TXBITCLKGEN_CTR_REG
        #define RS485_TXBITCLKTX_COMPLETE    RS485_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define RS485_TXBITCTR_PERIOD        RS485_TXBITCTR_PERIOD_REG
        #define RS485_TXBITCTR_CONTROL       RS485_TXBITCTR_CONTROL_REG
        #define RS485_TXBITCTR_COUNTER       RS485_TXBITCTR_COUNTER_REG
    #endif /* RS485_TXCLKGEN_DP */
#endif /* End RS485_TX_ENABLED */

#if(RS485_HD_ENABLED)
    #define RS485_TXDATA                 RS485_TXDATA_REG
    #define RS485_TXSTATUS               RS485_TXSTATUS_REG
    #define RS485_TXSTATUS_MASK          RS485_TXSTATUS_MASK_REG
    #define RS485_TXSTATUS_ACTL          RS485_TXSTATUS_ACTL_REG
#endif /* End RS485_HD_ENABLED */

#if( (RS485_RX_ENABLED) || (RS485_HD_ENABLED) )
    #define RS485_RXDATA                 RS485_RXDATA_REG
    #define RS485_RXADDRESS1             RS485_RXADDRESS1_REG
    #define RS485_RXADDRESS2             RS485_RXADDRESS2_REG
    #define RS485_RXBITCTR_PERIOD        RS485_RXBITCTR_PERIOD_REG
    #define RS485_RXBITCTR_CONTROL       RS485_RXBITCTR_CONTROL_REG
    #define RS485_RXBITCTR_COUNTER       RS485_RXBITCTR_COUNTER_REG
    #define RS485_RXSTATUS               RS485_RXSTATUS_REG
    #define RS485_RXSTATUS_MASK          RS485_RXSTATUS_MASK_REG
    #define RS485_RXSTATUS_ACTL          RS485_RXSTATUS_ACTL_REG
#endif /* End  (RS485_RX_ENABLED) || (RS485_HD_ENABLED) */

#if(RS485_INTERNAL_CLOCK_USED)
    #define RS485_INTCLOCK_CLKEN         RS485_INTCLOCK_CLKEN_REG
#endif /* End RS485_INTERNAL_CLOCK_USED */

#define RS485_WAIT_FOR_COMLETE_REINIT    RS485_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_RS485_H */


/* [] END OF FILE */
