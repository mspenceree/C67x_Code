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
 *  File        : PADK_DIR.c
 *  Description	: CS8416, CS8427 Digital Audio Receiver Driver
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
#include <csl_i2c.h>
#include <soc.h>

#include "PADK.h"

/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
//#define USE_I2C_RAW_CONFIG	1

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      DIR_Init
 *
 * PURPOSE:
 *		Initialise the digital audio receivers.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_Init( 
 *				DIR_Params *param		// (i) Initialisation parameters
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
far int DIR_Init( DIR_Params *param )
{
	//
	// Configure the reception device (CS8416)
	//
	I2C_WriteReg(	
					CS8416_DEVICE, 
					CONTROL_1,  
					param->swclk | param->mute | param->hold | param->chs
				);
	I2C_WriteReg(	
					CS8416_DEVICE, 
					CONTROL_2,   
					param->deemphasis	
				);
	I2C_WriteReg(	
					CS8416_DEVICE, 
					CONTROL_4, 
					param->run | param->rxsel
				);
	I2C_WriteReg(	
					CS8416_DEVICE, 
					SERIAL_AUDIO_DATA_FMT, 
					param->soms| param->sores | param->sodel | param->sopol | param->solrpol
				);
	I2C_WriteReg(	
					CS8416_DEVICE, 
					RCV_ERROR_MASK, 
					param->rxerr_mask
				);
	I2C_WriteReg(		
					CS8416_DEVICE, 
					INT_MASK, 
					param->int_mask	
				);
	
	//
	// Configure the word clock device (CS8427)						
	//
	I2C_WriteReg(	
					CS8427_DEVICE, 
					WORD_ERROR_MASK, 
					0x7F
				);
	I2C_WriteReg(	
					CS8427_DEVICE, 
					CLOCK_SRC_CTRL, 
					0x48
				);
	return 0;
}

/**************************************************************************** 
 *
 * NAME  
 *      DIR_Reset
 *
 * PURPOSE:
 *		Initialise the digital audio receivers with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_Reset(); 
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
far int DIR_Reset()
{
	DIR_Params params = DIR_DEFAULT_PARAMS;

	return DIR_Init( &params );	
}


/**************************************************************************** 
 *
 * NAME  
 *      DIR_Mute
 *
 * PURPOSE:
 *		Mutes receiver output
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_Mute(); 
 *
 * RETURN VALUE
 *		Error code
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int DIR_Mute()
{
	unsigned char regval;

	// Set MUTESAO in Control1
	if ( I2C_ReadReg(CS8416_DEVICE, CONTROL_1, &regval) )
	{
		// Failed to read register
		return -1;
	}

	regval |= DIR_MUTE;
	return I2C_WriteReg( CS8416_DEVICE, CONTROL_1, regval );	
}

/**************************************************************************** 
 *
 * NAME  
 *      DIR_Unmute
 *
 * PURPOSE:
 *		Mutes receiver output
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_Unmute(); 
 *
 * RETURN VALUE
 *		Error code
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int DIR_Unmute()
{
	unsigned char regval;

	// Clear MUTESAO in Control1
	if ( I2C_ReadReg(CS8416_DEVICE, CONTROL_1, &regval) )
	{
		// Failed to read register
		return -1;
	}

	regval &= ~DIR_MUTE;	
	return I2C_WriteReg( CS8416_DEVICE, CONTROL_1, regval );
}

/**************************************************************************** 
 *
 * NAME  
 *      DIR_ReadReg
 *
 * PURPOSE:
 *		Reads CS4816 register (RAW access). This routine directly calls 
 *		from the I2C module. Do not use this if using DSPBios to handle
 *		the I2C module.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_ReadReg( 
 *			unsigned reg, 			// (i) register address to read
 *			unsigned char *reg_val 	// (o) pointer where to store the read
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
far int DIR_ReadReg( unsigned reg, unsigned char *reg_val )
{
	return I2C_ReadReg( CS8416_DEVICE, reg, reg_val );
}

/**************************************************************************** 
 *
 * NAME  
 *      DIR_WriteReg
 *
 * PURPOSE:
 *		Writes CS4816 register (RAW access). This routine directly calls 
 *		from the I2C module. Do not use this if using DSPBios to handle
 *		the I2C module.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_WriteReg( 
 *			unsigned reg, 			// (i) register address to read
 *			unsigned char *reg_val 	// (i) pointer to the data to write
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
far int DIR_WriteReg( unsigned reg, unsigned char reg_val )
{
	return I2C_WriteReg( CS8416_DEVICE, reg, reg_val );
}

/**************************************************************************** 
 *
 * NAME  
 *      DIR_CfgRxSel
 *
 * PURPOSE:
 *		Set DIR device input line
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_CfgRxSel( 
 *			unsigned sel 	// (i) New input line
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
far int DIR_CfgRxSel( unsigned sel )
{	
	switch( sel )
	{
		case OPTO_INPUT:
			return I2C_WriteReg( CS8416_DEVICE, CONTROL_4, DIR_CLOCK_RUN|DIR_RXSEL_OPTO );
		case RCA_INPUT:
			return I2C_WriteReg( CS8416_DEVICE, CONTROL_4, DIR_CLOCK_RUN|DIR_RXSEL_RCA );
		default:
			// Invalid input
			return -1;		
	}
}


/****************************************************************************
 *                             Local Functions                              *
 ****************************************************************************/


