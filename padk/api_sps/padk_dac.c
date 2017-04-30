
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
 * File: padk_dac.c
 *  Description	: PCM4104 Audio Stereo Digital-To-Analog Converter Driver
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
 *      DAC_Init
 *
 * PURPOSE:
 *		Initialise the digital-to-analog converter.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DAC_Init( 
 *				DAC_Params *param,		// (i) Initialisation parameters
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
far int DAC_Init( DAC_Params *param )
{
	unsigned cfg;

	// Read current configuration
	cfg = FPGA_ADC_DAC_CTRL;

	// Clear old DAC configuration
	cfg &= ~(DAC_FS_MASK|DAC_FMT_MASK|DAC_MUTE_MASK|DAC_DEM_MASK);

	// Apply new DAC configuration

	// Sampling Mode
	switch ( param->fs )
	{
		case DAC_FS_SINGLERATE:
		case DAC_FS_DUALRATE:
		case DAC_FS_QUADRATE:
			cfg |= param->fs;
			break;

		default:
			// Invalid configuration
			return -1;
	}

	// Mute Controls
	switch ( param->mute )
	{
		case DAC_MUTE_OFF:
		case DAC_MUTE_ON:
			cfg |= param->mute;
			break;

		default:
			// Invalid configuration
			return -1;
	}
		
	// Audio Data format
	switch ( param->format )
	{
		case DAC_FMT_24B_LEFT_JUST:
		case DAC_FMT_24B_I2S:
			cfg |= param->format;
			break;

		default:
			// Invalid configuration
			return -1;
	}
		
	// Digital De-Emphasis
	switch ( param->deemphasis )
	{
		case DAC_DEM_OFF:
		case DAC_DEM_48KHZ:
		case DAC_DEM_441KHZ:
		case DAC_DEM_32KHZ:
			cfg |= param->deemphasis;
			break;

		default:
			// Invalid configuration
			return -1;
	}

	// Write new configuration
	FPGA_ADC_DAC_CTRL = cfg;

	return 0;
}


/**************************************************************************** 
 *
 * NAME  
 *      DAC_Reset
 *
 * PURPOSE:
 *		Initialise the digital-to-analog converter with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DAC_Reset(); 
 *
 * RETURN VALUE
 *		Error code
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int DAC_Reset()
{
    DAC_Params params = DAC_DEFAULT_PARAMS;

	return DAC_Init( &params );
}

/**************************************************************************** 
 *
 * NAME  
 *      DAC_Mute
 *
 * PURPOSE:
 *		Mutes the output of the DAC devices
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void DAC_Mute(); 
 *
 * RETURN VALUE
 *		none
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_Mute()
{
	FPGA_ADC_DAC_CTRL |= DAC_MUTE_ON;
}

/**************************************************************************** 
 *
 * NAME  
 *      DAC_Unmute
 *
 * PURPOSE:
 *		Unmutes the output of the DAC devices
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void DAC_Unmute(); 
 *
 * RETURN VALUE
 *		none
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_Unmute()
{
	FPGA_ADC_DAC_CTRL &= ~DAC_MUTE_ON;
}

/**************************************************************************** 
 *
 * NAME  
 *      DAC_RelayUnmute
 *
 * PURPOSE:
 *		Unmutes the output hardware relays of the DAC devices
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void DAC_RelayUnmute(); 
 *
 * RETURN VALUE
 *		none
 *		
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_RelayUnmute()
{
	FPGA_ADC_DAC_CTRL |= FPGA_AUDIO_MUTE;
}

/**************************************************************************** 
 *
 * NAME  
 *      DAC_RelayMute
 *
 * PURPOSE:
 *		Mutes the output hardware relays of the DAC devices
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void DAC_RelayMute(); 
 *
 * RETURN VALUE
 *		none
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_RelayMute()
{
	FPGA_ADC_DAC_CTRL &= ~FPGA_AUDIO_MUTE;
}

