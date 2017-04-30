
/**************************************************************************************************
 *																								  *
 *							            SSSSS    PPPPPP    SSSSS								  * 
 *							          SS   SS   PP   PP  SS   SS								  * 
 *							         SS        PP   PP  SS										  *
 *							         SSSSS    PPPPPP    SSSSS									  *
 *							            SS   PP            SS									  *
 *							      SS   SS   PP       SS   SS									  *
 *							      SSSSS    PP        SSSSS										  *
 *						 																		  *
 *							     SIGNAL  PROCESSING  SOLUTIONS									  *
 *																								  *
 **************************************************************************************************
 *																								  *
 *					Project:							  *
 *																								  *
 *					Author:		Michael E. Spencer, Ph.D.										  *
 *																								  *
 *					Copyright(c) Signal Processing Solutions, 2007								  *
 *																								  *
 **************************************************************************************************
 *
 * This driver is originaly from V2.0 of theLyrtech PADK Driver collection
 * and may have been modified (repaired) by MSpencer.
 *
 * File: padk_uart.c
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/

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
 *				UART_Params *param,		// (i) Initialisation parameters
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
far int UART_Init( UART_Params *param )
{
	unsigned char sData;
	unsigned division;
    volatile unsigned *CFGMCASP2 = (volatile unsigned *)0x40000020;

	*CFGMCASP2 = 0x00000002;		// AMUTEIN2 = AXR0[8]/SPI1_CLK pin	
	
	// Set the wanted baud rate
	sData = UART_LCR_REGISTER;
	UART_LCR_REGISTER = sData | UART_LCR_DIVISOR_LATCH_ACCESS_ENABLED;
	
	division = 11059200 / (param->baud<<4);
	
	UART_DL_LSB_REGISTER = ( division & 0xFF );
	UART_DL_MSB_REGISTER = ( ( division & 0xFF00 ) >>8 );
	
	UART_LCR_REGISTER = sData;
	
	// initialize IER
	UART_IER_REGISTER = param->ier;
	
	// initialize FCR
	UART_FCR_REGISTER = param->fcr;
	
	// initialize LCR
	UART_LCR_REGISTER = param->lcr;
	
	// initialize MCR
	UART_MCR_REGISTER = param->mcr;
	
	// initialize SCR
	UART_SCR_REGISTER = param->scr;
	
	return 0;
}

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
far int UART_Reset()
{
	UART_Params params = UART_DEFAULT_PARAMS;

	UART_Init( &params );
	return 0;
}

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
far int UART_Read( unsigned char *data, int N, int wait )
{
	int cnt = 0; 
	
	if ( wait )
	{
		while ( cnt++ < N )
		{
			while ( !( UART_LSR_REGISTER & 1 ) );
			*data++ = (char)UART_RBR_REGISTER;
		}
	}
	else
	{   
		while ( ( UART_LSR_REGISTER & 1 ) && (cnt < N) )
		{
			cnt++;
			*data++ = (char)UART_RBR_REGISTER;
		}		
	}
	
	return cnt;
}

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
 *			unsignedchar *data, // (i) Pointer to the source buffer
 *			int N, 			    // (i) Number of bytes to write 
 *			int wait 		    // (i) Wait until N words are written or not
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
far int UART_Write( unsigned char *data, int N, int wait )
{
	int cnt = 0;
	
	if ( wait )
	{
		while ( cnt++ < N )
		{
			while ( !( UART_LSR_REGISTER & (1<<5) ) );
			UART_THR_REGISTER = (unsigned)*data++;
		}
		while( !( UART_LSR_REGISTER & (1<<6) ) );				
	}
	else
	{   
		while ( ( UART_LSR_REGISTER & (1<<5) ) && (cnt < N) )
		{
			cnt++;
			UART_THR_REGISTER = (unsigned)*data++;
		}		
	}	
	
	return cnt;
}


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
far unsigned UART_GetInterruptId()
{
	// Read IIR
	return ( UART_IIR_REGISTER & 0x0F );
}


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
far void UART_EnableLed1( int enable )
{
	// Set GPO1 in MCR
	if( enable )
		UART_MCR_REGISTER |= 0x4;
	else
		UART_MCR_REGISTER &= ~0x4;
}

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
far void UART_EnableLed2( int enable )
{
	// Set GPO2 in MCR
	if( enable )
		UART_MCR_REGISTER |= 0x8;
	else
		UART_MCR_REGISTER &= ~0x8;
}
