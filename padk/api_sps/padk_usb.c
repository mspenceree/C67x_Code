
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
 * File: padk_usb.c
 *  Description	: USB Driver
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
 ***************************************************************************
 *
 * PADK USB Interface
 * ------------------
 * Windows USB Driver Name: \\.\Ltpadk_usb
 * 
 * Interface #: 0
 * Alternate Setting: 1
 *
 * USB micro-controler control pipes (type, direction, pipe#, max pkt size):
 *   BULK, out, 0, 64 (Reserved)
 *   BULK, in,  1, 64 (Reserved)
 *
 * DSP Control pipes (type, direction, pipe#, pkt-size 1.1/2.0, FIFO #):
 *   BULK, out, 2, 64/512 (double buffered), 0
 *   BULK, in,  3, 64/512 (double buffered), 2
 *
 * DSP Data pipes (type, direction, pipe#, max pkt size 1.1/2.0, FIFO #):
 *   BULK, out, 4, 64/512 (double buffered), 1
 *   BULK, in,  5, 64/512 (double buffered), 3
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
 *      USB_Init
 *
 * PURPOSE:
 *		Initialize the USB controller and set the AMUTEIN0 pin to be used as 
 *		external interrupt for the USB IRQs.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Init( 
 *				USB_Params *param,		// (i) Initialisation parameters
 *		);
 *
 * RETURN VALUE
 *		Error code (-1:error, 0:success)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int USB_Init( USB_Params *param )
{
	volatile unsigned *CFGMCASP1 = (volatile unsigned *)0x4000001C;

	*CFGMCASP1 = 0x00000003;		// AMUTEIN1 = AXR0[9]/AXR1[4]/SPI1_SIMO pin	

	// Config Host to DSP Pipe #1 (FIFO 0, End-point 2)
	if ( param->h2d1_en ) USB_ENABLE_FIFO( USB_HOST2DSP_PIPE1 );
	else                  USB_DISABLE_FIFO( USB_HOST2DSP_PIPE1 );

	if ( param->h2d1_irq_en ) USB_ENABLE_FIFO_INT( USB_HOST2DSP_PIPE1 );
	else                      USB_DISABLE_FIFO_INT( USB_HOST2DSP_PIPE1 );

	USB_WRITE_THRESHOLD( USB_HOST2DSP_PIPE1, param->h2d1_thrs );
	
	// Config DSP to Host Pipe #1 (FIFO 2, End-point 3)
	if ( param->d2h1_en ) USB_ENABLE_FIFO( USB_DSP2HOST_PIPE1 );
	else                  USB_DISABLE_FIFO( USB_DSP2HOST_PIPE1 );

	if ( param->d2h1_irq_en ) USB_ENABLE_FIFO_INT( USB_DSP2HOST_PIPE1 );
	else                      USB_DISABLE_FIFO_INT( USB_DSP2HOST_PIPE1 );

	USB_WRITE_THRESHOLD( USB_DSP2HOST_PIPE1, param->d2h1_thrs );

	// Config Host to DSP Pipe #2 (FIFO 1, End-point 4)
	if ( param->h2d2_en ) USB_ENABLE_FIFO( USB_HOST2DSP_PIPE2 );
	else                  USB_DISABLE_FIFO( USB_HOST2DSP_PIPE2 );

	if ( param->h2d2_irq_en ) USB_ENABLE_FIFO_INT( USB_HOST2DSP_PIPE2 );
	else                      USB_DISABLE_FIFO_INT( USB_HOST2DSP_PIPE2 );

	USB_WRITE_THRESHOLD( USB_HOST2DSP_PIPE2, param->h2d2_thrs );
	
	// Config DSP to Host Pipe #2 (FIFO 3, End-point 5)
	if ( param->d2h2_en ) USB_ENABLE_FIFO( USB_DSP2HOST_PIPE2 );
	else                  USB_DISABLE_FIFO( USB_DSP2HOST_PIPE2 );

	if ( param->d2h2_irq_en ) USB_ENABLE_FIFO_INT( USB_DSP2HOST_PIPE2 );
	else                      USB_DISABLE_FIFO_INT( USB_DSP2HOST_PIPE2 );

	USB_WRITE_THRESHOLD( USB_DSP2HOST_PIPE2, param->d2h2_thrs );

	// Config USB up IRQ
	if ( param->usbup_irq_en ) USB_ENABLE_USBUP_INT();
	else                       USB_DISABLE_USBUP_INT();

	// Config Global Interrupt Enable
	if ( param->irq_en ) USB_ENABLE_G_INT();
	else                 USB_DISABLE_G_INT();

	return 0;
}


/**************************************************************************** 
 *
 * NAME  
 *      USB_Reset
 *
 * PURPOSE:
 *		Initialize the USB controller with the default configuration.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Reset(); 
 *
 * RETURN VALUE
 *		Error code (-1:error, 0:success)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int USB_Reset()
{
	USB_Params params = USB_DEFAULT_PARAMS;

	return USB_Init( &params );
}


/**************************************************************************** 
 *
 * NAME  
 *      USB_Read
 *
 * PURPOSE:
 *		This function read up to N bytes from the USB controller. The call can
 *		be blocking or not. 
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Read(
 *			int fifo,				// (i) USB FIFO [0 or 1]
 *			unsigned char *buff,    // (o) Pointer to the destination buffer
 *			int N, 			        // (i) Number of bytes to read 
 *			int wait 		        // (i) Blocking or not 
 *		);
 *
 * RETURN VALUE
 *		Error code or the number of bytes received (-1:error)
 *
 * REFERENCE
 *
 * NOTE     
 *		This function is just a simple example of how to interface the USB
 *		controller. The dMAX controller and the IRQ can be used to achieve
 *		better performance.
 * 		
 ****************************************************************************/ 
far int USB_Read(
			int fifo,				// (i) USB FIFO [0 or 1]
			unsigned char *buff,    // (o) Pointer to the destination buffer
 			int N, 			        // (i) Number of bytes to read 
 			int wait ) 		        // (i) Blocking or not 
{
	int i;

	if ( (fifo != USB_HOST2DSP_PIPE1) && (fifo != USB_HOST2DSP_PIPE2) )
	{
		// Invalid FIFO
		return -1;
	}

	for ( i=0; i<N; i++ )
	{		
		if ( wait )
		{
			// Wait for some data 
			while ( USB_READ_SIZE(fifo) == 0 ) asm( "	nop 5" );
		}
		else
		{
			// Verify if there's data in the FIFO
			if ( USB_READ_SIZE(fifo) == 0 )
			{
				// No more data, return the number of byte read
				return i;
			}
		}

		// Read the data from the FIFO. The data can be read 8 bits 
		// or 16 bits at a time.
		*buff++ = USB_READ_DATA_BYTE( fifo );
	}
	return N;
}


/**************************************************************************** 
 *
 * NAME  
 *      USB_Write
 *
 * PURPOSE:
 *		This function write up to N bytes to the USB controller. The call can
 *		be blocking or not. If the call is blocking, a packet end will be 
 *		generated at the end of the data. In non-blocking mode, the caller
 *		is reponsible to generate the packet end if needed.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Write(
 *			int fifo,				// (i) USB FIFO [2 or 3]
 *			unsigned char *buff,    // (o) Pointer to the source buffer
 *			int N, 			        // (i) Number of bytes to write
 *			int wait 		        // (i) Blocking or not 
 *		);
 *
 * RETURN VALUE
 *		Error code or the number of byte transmitted (-1:error)
 *
 * REFERENCE
 *
 * NOTE     
 *		This function is just a simple example of how to interface the USB
 *		controller. The dMAX controller and the IRQ can be used to achieve
 *		better performance.
 * 		
 ****************************************************************************/ 
far int USB_Write(
			int fifo,				// (i) USB FIFO [2 or 3]
			unsigned char *buff,    // (o) Pointer to the source buffer
 			int N, 			        // (i) Number of bytes to write
 			int wait ) 		        // (i) Blocking or not 
{
	int i;

	if ( (fifo != USB_DSP2HOST_PIPE1) && (fifo != USB_DSP2HOST_PIPE2) )
	{
		// Invalid FIFO
		return -1;
	}

	for ( i=0; i<N; i++ )
	{		
		if ( wait )
		{
			// Wait for free space in the transmit FIFO 
			while ( USB_READ_SIZE(fifo) == 0 ) asm( "	nop 5" );
		}
		else
		{
			// Verify if there's free space in the FIFO
			if ( USB_READ_SIZE(fifo) == 0 )
			{
				// No more free space, return the number of byte transmitted
				return i;
			}
		}

		// Write data to the FIFO. The data can be written 8 bits 
		// or 16 bits at a time.
		USB_WRITE_DATA_BYTE( fifo, *buff++ );
	}

	if ( wait )
	{
		// Do a packet end if in blocking mode. This will force the 
		// transmission of the data in the USB controller to the Host.
		// Note that the data is also transmit when "max packet size" bytes
		// are in the USB FIFO. The maximum packet size is 64 bytes in USB 1 
		// and 512 bytes in USB 2.
		USB_WRITE_PKTEND( fifo );
	}
	return N;
}
