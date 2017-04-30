
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
 * File: padk_conv.c
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"
#include "PADK_I2C.h"

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
 *  Modified by MSpencer to use external reference for 0-5V inputP (REF_OFF_AD_ON).
 *  Also needed to do hardware modifications to bypass non-rail-to-rail
 *  op-amps.
 * 		
 ****************************************************************************/   
far int CONV_StartConvertion( int channel )
{
	switch( channel)
	{
		case CONV_CHANNEL_0:
			I2C_WriteVal( ADS7830_DEVICE, CONV_CH0_MAP | REF_OFF_AD_ON );
			break;
		case CONV_CHANNEL_1:
			I2C_WriteVal( ADS7830_DEVICE, CONV_CH1_MAP | REF_OFF_AD_ON );
			break;
		case CONV_CHANNEL_2:
			I2C_WriteVal( ADS7830_DEVICE, CONV_CH2_MAP | REF_OFF_AD_ON );
			break;
		default:
			return -1;
	}
	
	return 0;	// success
}

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
far unsigned char CONV_GetConversionResult()
{
	unsigned char val;

	I2C_ReadVal( ADS7830_DEVICE, &val );
	return val;
}

