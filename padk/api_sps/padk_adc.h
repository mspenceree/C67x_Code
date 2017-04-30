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
 *  File        : PADK_ADC.h
 *  Description	: PCM4204 Audio Analog-To-Digital Converter Driver
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
#ifndef __PADK_ADC_H__
#define __PADK_ADC_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"

/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct
{	
	unsigned char format;
	unsigned char rate;
} ADC_Params;

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Data format
#define ADC_FMT_24B_LEFT_JUST	(0)
#define ADC_FMT_24B_I2S			(0|FPGA_ADC_FMT0)

// Clock Rate
#define ADC_RATE_SINGLE			(0)                             // 32, 44.1, 48 kHz
#define ADC_RATE_DUAL 			(0|FPGA_ADC_FS0)                // 88.2, 96 kHz
#define ADC_RATE_QUAD	 		(0|FPGA_ADC_FS1|FPGA_ADC_FS0)   // 176.4, 192 kHz

// Default Params
#define ADC_DEFAULT_PARAMS {       \
    ADC_FMT_24B_LEFT_JUST,          \
    ADC_RATE_SINGLE                 \
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      ADC_Init
 *
 * PURPOSE:
 *		Initialise the analog-to-digital converter.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int ADC_Init( 
 *				ADC_Params *param,		// (i) Initialisation parameters
 *		);
 *
 * RETURN VALUE
 *		Error code
 *		
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int ADC_Init( ADC_Params *param );

/**************************************************************************** 
 *
 * NAME  
 *      ADC_Reset
 *
 * PURPOSE:
 *		Initialise the analog-to-digital converter with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int ADC_Reset(); 
 *
 * RETURN VALUE
 *		Error code
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int ADC_Reset();

#endif	// __PADK_ADC_H__

