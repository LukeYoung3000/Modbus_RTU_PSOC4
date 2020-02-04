/***************************************************************************//**
* \file USB_UART_SPI_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USB_UART_PVT.h"
#include "USB_UART_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(USB_UART_INTERNAL_RX_SW_BUFFER_CONST)
    /* Start index to put data into the software receive buffer.*/
    volatile uint32 USB_UART_rxBufferHead;
    /* Start index to get data from the software receive buffer.*/
    volatile uint32 USB_UART_rxBufferTail;
    /**
    * \addtogroup group_globals
    * \{
    */
    /** Sets when internal software receive buffer overflow
    *  was occurred.
    */
    volatile uint8  USB_UART_rxBufferOverflow;
    /** \} globals */
#endif /* (USB_UART_INTERNAL_RX_SW_BUFFER_CONST) */

#if(USB_UART_INTERNAL_TX_SW_BUFFER_CONST)
    /* Start index to put data into the software transmit buffer.*/
    volatile uint32 USB_UART_txBufferHead;
    /* Start index to get data from the software transmit buffer.*/
    volatile uint32 USB_UART_txBufferTail;
#endif /* (USB_UART_INTERNAL_TX_SW_BUFFER_CONST) */

#if(USB_UART_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 USB_UART_rxBufferInternal[USB_UART_INTERNAL_RX_BUFFER_SIZE];
#endif /* (USB_UART_INTERNAL_RX_SW_BUFFER) */

#if(USB_UART_INTERNAL_TX_SW_BUFFER)
    volatile uint8 USB_UART_txBufferInternal[USB_UART_TX_BUFFER_SIZE];
#endif /* (USB_UART_INTERNAL_TX_SW_BUFFER) */


#if(USB_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: USB_UART_SpiUartReadRxData
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * \return
    *  Next data element from the receive buffer. 
    *  The amount of data bits to be received depends on RX data bits selection 
    *  (the data bit counting starts from LSB of return value).
    *
    * \globalvars
    *  USB_UART_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  USB_UART_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 USB_UART_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (USB_UART_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (USB_UART_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (USB_UART_CHECK_RX_SW_BUFFER)
        {
            if (USB_UART_rxBufferHead != USB_UART_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (USB_UART_rxBufferTail + 1u);

                if (USB_UART_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = USB_UART_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                USB_UART_rxBufferTail = locTail;

                #if (USB_UART_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (USB_UART_INTR_RX_MASK_REG & USB_UART_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        USB_UART_INTR_RX_MASK_REG |= USB_UART_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = USB_UART_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: USB_UART_SpiUartGetRxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * \return
    *  Number of received data elements.
    *
    * \globalvars
    *  USB_UART_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  USB_UART_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 USB_UART_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (USB_UART_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (USB_UART_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (USB_UART_CHECK_RX_SW_BUFFER)
        {
            locHead = USB_UART_rxBufferHead;

            if(locHead >= USB_UART_rxBufferTail)
            {
                size = (locHead - USB_UART_rxBufferTail);
            }
            else
            {
                size = (locHead + (USB_UART_INTERNAL_RX_BUFFER_SIZE - USB_UART_rxBufferTail));
            }
        }
        #else
        {
            size = USB_UART_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: USB_UART_SpiUartClearRxBuffer
    ****************************************************************************//**
    *
    *  Clears the receive buffer and RX FIFO.
    *
    * \globalvars
    *  USB_UART_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  USB_UART_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    void USB_UART_SpiUartClearRxBuffer(void)
    {
        #if (USB_UART_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            USB_UART_DisableInt();

            /* Flush RX software buffer */
            USB_UART_rxBufferHead = USB_UART_rxBufferTail;
            USB_UART_rxBufferOverflow = 0u;

            USB_UART_CLEAR_RX_FIFO;
            USB_UART_ClearRxInterruptSource(USB_UART_INTR_RX_ALL);

            #if (USB_UART_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                USB_UART_INTR_RX_MASK_REG |= USB_UART_INTR_RX_NOT_EMPTY;
            }
            #endif
            
            /* Release lock */
            USB_UART_EnableInt();
        }
        #else
        {
            USB_UART_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (USB_UART_RX_DIRECTION) */


#if(USB_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: USB_UART_SpiUartWriteTxData
    ****************************************************************************//**
    *
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *   The amount of data bits to be transmitted depends on TX data bits selection 
    *   (the data bit counting starts from LSB of txDataByte).
    *
    * \globalvars
    *  USB_UART_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  USB_UART_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void USB_UART_SpiUartWriteTxData(uint32 txData)
    {
    #if (USB_UART_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (USB_UART_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (USB_UART_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((USB_UART_txBufferHead == USB_UART_txBufferTail) &&
                (USB_UART_SPI_UART_FIFO_SIZE != USB_UART_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                USB_UART_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (USB_UART_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (USB_UART_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == USB_UART_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                USB_UART_ClearTxInterruptSource(USB_UART_INTR_TX_NOT_FULL);

                USB_UART_PutWordInTxBuffer(locHead, txData);

                USB_UART_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (USB_UART_INTR_TX_MASK_REG & USB_UART_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    USB_UART_INTR_TX_MASK_REG |= (uint32) USB_UART_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (USB_UART_SPI_UART_FIFO_SIZE == USB_UART_GET_TX_FIFO_ENTRIES)
            {
            }

            USB_UART_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: USB_UART_SpiUartPutArray
    ****************************************************************************//**
    *
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * \param wrBuf: pointer to an array of data to be placed in transmit buffer. 
    *  The width of the data to be transmitted depends on TX data width selection 
    *  (the data bit counting starts from LSB for each array element).
    * \param count: number of data elements to be placed in the transmit buffer.
    *
    * \globalvars
    *  USB_UART_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  USB_UART_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void USB_UART_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            USB_UART_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: USB_UART_SpiUartGetTxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of elements currently in the transmit buffer.
    *   - TX software buffer is disabled: returns the number of used entries in
    *     TX FIFO.
    *   - TX software buffer is enabled: returns the number of elements currently
    *     used in the transmit buffer. This number does not include used entries in
    *     the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *     not full.
    *
    * \return
    *  Number of data elements ready to transmit.
    *
    * \globalvars
    *  USB_UART_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  USB_UART_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    uint32 USB_UART_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (USB_UART_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (USB_UART_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (USB_UART_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = USB_UART_txBufferTail;

            if (USB_UART_txBufferHead >= locTail)
            {
                size = (USB_UART_txBufferHead - locTail);
            }
            else
            {
                size = (USB_UART_txBufferHead + (USB_UART_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = USB_UART_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: USB_UART_SpiUartClearTxBuffer
    ****************************************************************************//**
    *
    *  Clears the transmit buffer and TX FIFO.
    *
    * \globalvars
    *  USB_UART_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  USB_UART_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void USB_UART_SpiUartClearTxBuffer(void)
    {
        #if (USB_UART_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            USB_UART_DisableInt();

            /* Flush TX software buffer */
            USB_UART_txBufferHead = USB_UART_txBufferTail;

            USB_UART_INTR_TX_MASK_REG &= (uint32) ~USB_UART_INTR_TX_NOT_FULL;
            USB_UART_CLEAR_TX_FIFO;
            USB_UART_ClearTxInterruptSource(USB_UART_INTR_TX_ALL);

            /* Release lock */
            USB_UART_EnableInt();
        }
        #else
        {
            USB_UART_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (USB_UART_TX_DIRECTION) */


/*******************************************************************************
* Function Name: USB_UART_SpiUartDisableIntRx
****************************************************************************//**
*
*  Disables the RX interrupt sources.
*
*  \return
*   Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 USB_UART_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = USB_UART_GetRxInterruptMode();

    USB_UART_SetRxInterruptMode(USB_UART_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: USB_UART_SpiUartDisableIntTx
****************************************************************************//**
*
*  Disables TX interrupt sources.
*
*  \return
*   Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 USB_UART_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = USB_UART_GetTxInterruptMode();

    USB_UART_SetTxInterruptMode(USB_UART_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(USB_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: USB_UART_PutWordInRxBuffer
    ****************************************************************************//**
    *
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param index:      index to store data byte/word in the RX buffer.
    *  \param rxDataByte: byte/word to store.
    *
    *******************************************************************************/
    void USB_UART_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (USB_UART_ONE_BYTE_WIDTH == USB_UART_rxDataBits)
        {
            USB_UART_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            USB_UART_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            USB_UART_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: USB_UART_GetWordFromRxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \return
    *   Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 USB_UART_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (USB_UART_ONE_BYTE_WIDTH == USB_UART_rxDataBits)
        {
            value = USB_UART_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) USB_UART_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)USB_UART_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: USB_UART_PutWordInTxBuffer
    ****************************************************************************//**
    *
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx:        index to store data byte/word in the TX buffer.
    *  \param txDataByte: byte/word to store.
    *
    *******************************************************************************/
    void USB_UART_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (USB_UART_ONE_BYTE_WIDTH == USB_UART_txDataBits)
        {
            USB_UART_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            USB_UART_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            USB_UART_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: USB_UART_GetWordFromTxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx: index to get data byte/word from the TX buffer.
    *
    *  \return
    *   Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 USB_UART_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (USB_UART_ONE_BYTE_WIDTH == USB_UART_txDataBits)
        {
            value = (uint32) USB_UART_txBuffer[idx];
        }
        else
        {
            value  = (uint32) USB_UART_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) USB_UART_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (USB_UART_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
