
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
 * This drive is originaly from V2.0 of theLyrtech PADK Driver collection
 * and may have been modified (repaired) by MSpencer.
 *
 * File: padk.midi.c
 *  Description	: Midi Port Interface Driver
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
 *                             Local constants                              *
 ****************************************************************************/

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_Init
 *
 * PURPOSE:
 *		Initialise the MIDI receiver / transmetter.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int MIDI_Init( 
 *				PADK_Params *param,		// (i) Initialisation parameters
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
far int MIDI_Init( MIDI_Params *param )
{
	unsigned char sData;
	unsigned division;
	volatile unsigned *CFGMCASP0 = (volatile unsigned *)0x40000018;

	*CFGMCASP0 = 0x00000001;		// AMUTEIN0 = AXR0[7]/AXR1[5]/SPI1_SOMI pin	
	
	// Set the wanted baud rate
	sData = MIDI_LCR_REGISTER;
	MIDI_LCR_REGISTER = sData | MIDI_LCR_DIVISOR_LATCH_ACCESS_ENABLED;
	
	division = 11059200 / (param->baud<<4);
	
	MIDI_DL_LSB_REGISTER = ( division & 0x00FF );
	MIDI_DL_MSB_REGISTER = ( division >> 8 );
	
	MIDI_LCR_REGISTER = sData;
	
	// initialize IER
	MIDI_IER_REGISTER = param->ier;
	
	// initialize FCR
	MIDI_FCR_REGISTER = param->fcr;
	
	// initialize LCR
	MIDI_LCR_REGISTER = param->lcr;
	
	// initialize MCR
	MIDI_MCR_REGISTER = param->mcr;
	
	// initialize SCR
	MIDI_SCR_REGISTER = param->scr;
	
	return 0;
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_Reset
 *
 * PURPOSE:
 *		Initialise the MIDI interface with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int MIDI_Reset(); 
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
far int MIDI_Reset()
{
	MIDI_Params params = MIDI_DEFAULT_PARAMS;

	MIDI_Init( &params );
	return 0;
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_Read
 *
 * PURPOSE:
 *		Reads up to N bytes, waiting or not, return the number of bytes read
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int MIDI_Read( 
 *			unsigned char *data, 	// (o) Pointer to the destination buffer
 *			int N, 		            // (i) Number of bytes to read 
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
far int MIDI_Read( unsigned char *data, int N, int wait )
{
	int cnt = 0;                                                                       
	
	if ( wait )
	{
		while ( cnt++ < N )
		{
			while ( !( MIDI_LSR_REGISTER & 1 ) );
			*data++ = (char)MIDI_RBR_REGISTER;
		}
	}
	else
	{   
		while ( ( MIDI_LSR_REGISTER & 1 ) && (cnt < N) )
		{
			cnt++;
			*data++ = (char)MIDI_RBR_REGISTER;
		}		
	}
	
	return cnt;
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_Write
 *
 * PURPOSE:
 *		Writes up to N bytes, waiting or not, return the number of bytes written
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int MIDI_Write( 
 *			unsignedchar *data, // (i) Pointer to the source buffer
 *			int N, 		    	// (i) Number of bytes to write 
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
far int MIDI_Write( unsigned char *data, int N, int wait )
{
	int cnt = 0;
	
	if ( wait )
	{
		while ( cnt++ < N )
		{
			while ( !( MIDI_LSR_REGISTER & (1<<5) ) );
			MIDI_THR_REGISTER = *data++;
		}				
	}
	else
	{   
		while ( ( MIDI_LSR_REGISTER & (1<<5) ) && (cnt < N) )
		{
			cnt++;
			MIDI_THR_REGISTER = *data++;
		}		
	}	
	
	return cnt;
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_SetRTS
 *
 * PURPOSE:
 *		Sets the polarity of RTS (Request To Send) bit.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void MIDI_SetRTS( 
 *			int rts 		// (i) Value for the RTS field
 *		)
 *
 * RETURN VALUE
 *	
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
void MIDI_SetRTS( int rts )
{	
	// Set RTS in MCR
	MIDI_MCR_REGISTER = ( ( MIDI_MCR_REGISTER & ~0x2 ) | ( ( rts & 0x1 ) << 1 ) );	
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_GetInterruptId
 *
 * PURPOSE:
 *		Get the source of the MIDI interrupt (in the MIDI register)
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		unsigned MIDI_GetInterruptId()
 *
 * RETURN VALUE
 *		Source of the MIDI interrupt
 *		
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far unsigned MIDI_GetInterruptId()
{
	// Read IIR
	return 0;
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_EnableLed1
 *
 * PURPOSE:
 *		Enable/Disable the MIDI led #1
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void MIDI_EnableLed1( 
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
far void MIDI_EnableLed1( int enable )
{
	// Set GPO1 in MCR
	if( enable )
		MIDI_MCR_REGISTER |= 0x4;
	else
		MIDI_MCR_REGISTER &= ~0x4;		
}

/**************************************************************************** 
 *
 * NAME  
 *      MIDI_EnableLed2
 *
 * PURPOSE:
 *		Enable/Disable the MIDI led #2
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void MIDI_EnableLed2( 
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
far void MIDI_EnableLed2( int enable )
{
	// Set GPO2 in MCR
	if( enable )
		MIDI_MCR_REGISTER |=  0x8;
	else
		MIDI_MCR_REGISTER &= ~0x8;
}
