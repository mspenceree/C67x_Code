/***************************************************************************
 *
 *
 *         **   **    **  ******  ********  ********  ********  **    **    
 *        **    **   **  **   ** ********  ********  ********  **    **
 *       **     *****   **   **    **     **        **        **    **
 *      **       **    ******     **     ****      **        ********
 *     **       **    **  **     **     **        **        **    **
 *    *******  **    **   **    **     ********  ********  **    **
 *   *******  **    **    **   **     ********  ********  **    **  
 *
 *            L Y R T E C H   S I G N A L   P R O C E S S I N G              
 *
 ***************************************************************************
 *                                                                          
 *  Project     : PADK
 *  File        : PADK_UART.h
 *  Description	: RS-232 UART Driver
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:18 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/
#ifndef __PADK_UART_H__
#define __PADK_UART_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct {
	
	unsigned baud;			// baud rate
	unsigned ier;			// interrupt enable register
	unsigned fcr;			// fifo control register
	unsigned lcr;			// line control register
	unsigned mcr;			// modem control register
	unsigned scr;			// scratch register

} UART_Params;

/****************************************************************************
 *                             Global Constants                             *
 ****************************************************************************/

// Address Lines A4-A2 are used to know what register is accessed
#define RBR_REG_OFF			0x0000		// [A4-A2] - 000 (DLAB = 0)
#define IER_REG_OFF			0x0010		// [A4-A2] - 001 (DLAB = 0)
#define IIR_REG_OFF			0x0020		// [A4-A2] - 010 (read)   
#define FCR_REG_OFF			0x0020		// [A4-A2] - 010 (write)
#define LCR_REG_OFF			0x0030		// [A4-A2] - 011
#define MCR_REG_OFF			0x0040		// [A4-A2] - 100
#define LSR_REG_OFF			0x0050		// [A4-A2] - 101
#define SCR_REG_OFF			0x0070		// [A4-A2] - 111
#define DL_LSB_REG_OFF		0x0000	  	// [A4-A2] - 000 (DLAB = 1)
#define DL_MSB_REG_OFF		0x0010		// [A4-A2] - 001 (DLAB = 1)

//Uart device registers
#define UART_RBR_REGISTER	 	*((volatile unsigned char *)(UART_BASE_ADDR + RBR_REG_OFF))	//DLAB must be set to 0
#define UART_THR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + RBR_REG_OFF))	//DLAB must be set to 0
#define UART_IER_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + IER_REG_OFF))	//DLAB must be set to 0
#define UART_IIR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + IIR_REG_OFF))
#define UART_FCR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + FCR_REG_OFF))
#define UART_LCR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + LCR_REG_OFF))
#define UART_MCR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + MCR_REG_OFF))
#define UART_LSR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + LSR_REG_OFF))
#define UART_SCR_REGISTER	   	*((volatile unsigned char *)(UART_BASE_ADDR + SCR_REG_OFF))
#define UART_DL_LSB_REGISTER   	*((volatile unsigned char *)(UART_BASE_ADDR + DL_LSB_REG_OFF))	//DLAB must be set to 1
#define UART_DL_MSB_REGISTER   	*((volatile unsigned char *)(UART_BASE_ADDR + DL_MSB_REG_OFF))	//DLAB must be set to 1

// Midi chip registers bit values
//
// LCR
//
#define	UART_LCR_BITS_5							0x00
#define	UART_LCR_BITS_6							0x01
#define	UART_LCR_BITS_7							0x02
#define	UART_LCR_BITS_8							0x03
#define UART_LCR_STOP_1							0x00
#define UART_LCR_STOP_2							0x04
#define UART_LCR_PARITY_NONE					0x00
#define UART_LCR_PARITY_ODD 					0x08
#define UART_LCR_PARITY_EVEN					0x18
#define UART_LCR_STICK_OFF						0x00
#define UART_LCR_STICK_ON 						0x20
#define UART_LCR_BREAK_OFF						0x00
#define UART_LCR_BREAK_ON 						0x40
#define UART_LCR_DIVISOR_LATCH_ACCESS_DISABLED	0x00
#define UART_LCR_DIVISOR_LATCH_ACCESS_ENABLED	0x80

//
// FCR
//
#define  UART_FCR_FIFOEN_FALSE        			0x00
#define  UART_FCR_FIFOEN_TRUE		  			0x01
#define  UART_FCR_RXRST_NONE          			0x00
#define  UART_FCR_RXRST_FIFO		  			0x02
#define  UART_FCR_TXRST_NONE         			0x00
#define  UART_FCR_TXRST_FIFO		   			0x04
#define  UART_FCR_MODE_RDY_0          			0x00
#define  UART_FCR_MODE_RDY_1	   				0x08
#define  UART_FCR_TRIG_01						0x00
#define  UART_FCR_TRIG_04						0x40
#define  UART_FCR_TRIG_08						0x80
#define  UART_FCR_TRIG_14						0xC0

//
// Interrupt Sources (when reading IIR)
//
#define UART_INT_NONE							0x01
#define UART_RCV_LINE_STATUS_INT				0x06
#define UART_CHAR_TIMEOUT_INT					0x04
#define UART_TX_HOLD_REG_EMPTY_INT				0x02
#define UART_MODEM_STATUS_INT					0x00

// Default Params
#define UART_DEFAULT_PARAMS {       								\
	115200,						/* Uart baud rate */				\
	0,							/* Interrupt Enable Register*/		\
	/* FIFO Control Register */										\
	UART_FCR_TRIG_14 |			/* Receiver trig at 1 byte */		\
	UART_FCR_MODE_RDY_0 |		/* DMA mode 0 */					\
	UART_FCR_TXRST_FIFO |		/* Clear transmitter fifo */		\
	UART_FCR_RXRST_FIFO |		/* Clear receiver fifo */			\
	UART_FCR_FIFOEN_TRUE,		/* Fifo enable */					\
	/* Line Control Register */										\
	UART_LCR_BREAK_OFF |		/* no break */						\
	UART_LCR_STICK_OFF |		/* no stick parity */				\
	UART_LCR_PARITY_NONE |		/* no parity */						\
	UART_LCR_STOP_1 |			/* 1 stop bit */					\
	UART_LCR_BITS_8,			/* 8-bit words */					\
	0x02,						/* MODEM Control Register (receive) */ \
	0							/* Scratch Register */				\
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      UART_Init
 *
 * PURPOSE:
 *		Initialise the RS-232 receiver / transmetter.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int UART_Init( 
 *				UART_Params *param		// (i) Initialisation parameters
 *		);
 *
 * RETURN VALUE
 *		Error code
 *		
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int UART_Init( UART_Params *param );

/**************************************************************************** 
 *
 * NAME  
 *      UART_Reset
 *
 * PURPOSE:
 *		Initialise the RS-232 receiver / transmetter with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int UART_Reset(); 
 *
 * RETURN VALUE
 *		Error code
 *		
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int UART_Reset();

/**************************************************************************** 
 *
 * NAME  
 *      UART_Read
 *
 * PURPOSE:
 *		Reads up to N bytes, waiting or not, return the number of bytes read
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int UART_Read( 
 *			unsigned char *data,    // (o) Pointer to the destination buffer
 *			int N, 			        // (i) Number of bytes to read 
 *			int wait 		        // (i) Wait until N words are read or not
 *		)
 *
 * RETURN VALUE
 *		Number of bytes read
 *		
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int UART_Read( unsigned char *data, int N, int wait );

/**************************************************************************** 
 *
 * NAME  
 *      UART_Write
 *
 * PURPOSE:
 *		Writes up to N bytes, waiting or not, return the number of bytes written
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int UART_Write( 
 *			unsigned char *data,    // (i) Pointer to the source buffer
 *			int N, 			        // (i) Number of bytes to write 
 *			int wait 		        // (i) Wait until N words are written or not
 *		)
 *
 * RETURN VALUE
 *		Number of bytes written
 *		
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int UART_Write( unsigned char *data, int N, int wait );

/**************************************************************************** 
 *
 * NAME  
 *      UART_GetInterruptId
 *
 * PURPOSE:
 *		Get the source of the UART interrupt (in the UART register)
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		unsigned UART_GetInterruptId()
 *
 * RETURN VALUE
 *		Source of the UART interrupt
 *		
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far unsigned UART_GetInterruptId();

/**************************************************************************** 
 *
 * NAME  
 *      UART_EnableLed1
 *
 * PURPOSE:
 *		Enable/Disable the UART led #1
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void UART_EnableLed1( 
 *			int enable 			// (i) enable or disable the LED
 *		)
 *
 * RETURN VALUE
 *		
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/
far void UART_EnableLed1( int enable );

/**************************************************************************** 
 *
 * NAME  
 *      UART_EnableLed2
 *
 * PURPOSE:
 *		Enable/Disable the UART led #2
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void UART_EnableLed2( 
 *			int enable 			// (i) enable or disable the LED
 *		)
 *
 * RETURN VALUE
 *		
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/
far void UART_EnableLed2( int enable );

/**************************************************************************** 
 *
 * NAME  
 *      UART_EnableIrq
 *
 * PURPOSE:
 *		Enable the UART interrupt
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void UART_EnableIrq()
 *
 * RETURN VALUE
 *		
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/
far void UART_EnableIrq();

/**************************************************************************** 
 *
 * NAME  
 *      UART_DisableIrq
 *
 * PURPOSE:
 *		Disable the UART interrupt
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void UART_DisableIrq()
 *
 * RETURN VALUE
 *		
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/
far void UART_DisableIrq();


#endif	// __PADK_UART_H__

