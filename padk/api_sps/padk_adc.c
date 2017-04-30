
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
 * File: padk_adc.c
 *  Description	: PCM4204 Audio Analog-To-Digital Converter Driver
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:16 $"
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
 *      ADC_Init
 *
 * PURPOSE:
 *		Initialise the analog-to-digital converter.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int ADC_Init( 
 *				ADC_Params *param		// (i) Initialisation parameters
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
far int ADC_Init( ADC_Params *param )
{
	unsigned cfg;

	// read the current configuration
	cfg = FPGA_ADC_DAC_CTRL;

	// clear current configuration bits
	cfg &=  ~(FPGA_ADC_FS1|FPGA_ADC_FS0|FPGA_ADC_FMT0);
	
	// apply new configuration
	switch( param->rate )
	{                
		case ADC_RATE_SINGLE:
		case ADC_RATE_DUAL:
		case ADC_RATE_QUAD:
			cfg |= param->rate;
			break;

		default:
			// Invalid rate 
			return -1;
	}

	switch( param->format )
	{
		case ADC_FMT_24B_LEFT_JUST:
		case ADC_FMT_24B_I2S:
			cfg |= param->format;
			break;
		
		default:
			// Invalid format
			return -1;
	}
	
	// assert reset of the ADCs
	FPGA_MISC_CTRL &= ~NOT_ADC_RESET_BIT;

	// write new configuration
	FPGA_ADC_DAC_CTRL = cfg; 

	// deassert reset of the ADCs
	FPGA_MISC_CTRL |= NOT_ADC_RESET_BIT;
	
	return 0;
}

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
far int ADC_Reset()
{
    ADC_Params params = ADC_DEFAULT_PARAMS;

	return ADC_Init( &params );
}


