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
 *  File        : PADK_CONV.h
 *  Description	: ADS7830 Analog-to-Digital Converter Driver 
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:17 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/
#ifndef __PADK_CONV_H__
#define __PADK_CONV_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

//////////////////////////
//	  device address	//
//////////////////////////
#define ADS7830_DEVICE				(0x48)

//////////////////////////
//     channel ids      //
//////////////////////////
#define CONV_CHANNEL_0              (0x0)   // pin 4 of MIDI connector
#define CONV_CHANNEL_1              (0x1)   // pin 9 of MIDI connector  
#define CONV_CHANNEL_2              (0x2)   // pin 5 of MIDI connector

//////////////////////////
//channel conversion map//
//////////////////////////
#define CONV_CH0_MAP				(0x8<<4)
#define CONV_CH1_MAP				(0xC<<4)
#define CONV_CH2_MAP				(0x9<<4)

//////////////////////////
//   power-down opt     //
//////////////////////////
#define PD_BETWEEN_CONV				(0x0<<2)
#define REF_OFF_AD_ON				(0x1<<2)
#define REF_ON_AD_OFF               (0x2<<2)
#define REF_ON_AD_ON				(0x3<<2)

/**************************************************************************** 
 *
 * NAME  
 *      CONV_StartConvertion
 *
 * PURPOSE:
 *		Start an analog to digital conversion on its specified input. This 
 *		function should always be called before the 
 *		CONV_GetConvertionResult() function is called.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int CONV_StartConvertion( 
 *				int channel		// (i) Input channel to convert
 *		);
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int CONV_StartConvertion( int channel );

/**************************************************************************** 
 *
 * NAME  
 *      CONV_GetConversionResult
 *
 * PURPOSE:
 *		Read the result of a conversion from the ADS7830 device. The 
 *		conversion must be initialized first by calling the 
 *		CONV_StartConversion() function
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int CONV_GetConversionResult();
 *
 * RETURN VALUE
 *		Converted value (8 bits)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far unsigned char CONV_GetConversionResult();

#endif // __PADK_CONV_H__

