
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
 * File: padk_clkgen.c
 *  Description	: Clock Generator Driver
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
 *      CLKGEN_Init
 *
 * PURPOSE:
 *		Initialise the sample rate generator and the clock source for all devices.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int CLKGEN_Init( 
 *				CLKGEN_Params *param,		// (i) Initialisation parameters
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
far int CLKGEN_Init( CLKGEN_Params *param )
{
    unsigned cfg;	
	
	// Enable HD[24:31] as GPIOs
	GPIO_EN |= (1<<10);

	// HD[30] in output
	GPIO_DIR1 |= OSC_SEL_BIT;

	// Select the desired oscillator
	GPIO_DAT1 = (GPIO_DAT1 &~ OSC_SEL_BIT) | param->osc_sel;

	// Get old configuration
	cfg = FPGA_CLOCKS_CTRL;

	// Clear old configuration bits
	cfg &= (FPGA_AHCLKR2|FPGA_AHCLKX1|FPGA_AHCLKR0|FPGA_AHCLKX0);
	
	// AHCLKR0 - AHCLKR1
	switch( param->adc_dir_clk_src )
	{
		case CLK_SRC_OSC	: break;
		case CLK_SRC_OSC_2X	: cfg |= FPGA_AHCLKR0_S2; break;
		case CLK_SRC_AES	: cfg |= FPGA_AHCLKR0_S1; break;
		case CLK_SRC_AES_2X	: cfg |= FPGA_AHCLKR0_S2|FPGA_AHCLKR0_S1; break;
		case CLK_SRC_WORD	: cfg |= FPGA_AHCLKR0_S1|FPGA_AHCLKR0_S0; break;
		case CLK_SRC_WORD_2X: cfg |= FPGA_AHCLKR0_S2|FPGA_AHCLKR0_S1|FPGA_AHCLKR0_S0; break;
		default: return -1;
	}

	// AHCLKX0 - AHCLKX2
	switch( param->dac_dit_clk_src )
	{
		case CLK_SRC_OSC	: break;
		case CLK_SRC_OSC_2X	: cfg |= FPGA_AHCLKX0_S2; break;
		case CLK_SRC_AES	: cfg |= FPGA_AHCLKX0_S1; break;
		case CLK_SRC_AES_2X	: cfg |= FPGA_AHCLKX0_S2|FPGA_AHCLKX0_S1; break;
		case CLK_SRC_WORD	: cfg |= FPGA_AHCLKX0_S1|FPGA_AHCLKX0_S0; break;
		case CLK_SRC_WORD_2X: cfg |= FPGA_AHCLKX0_S2|FPGA_AHCLKX0_S1|FPGA_AHCLKX0_S0; break;
		default: return -1;
	}
	

	// The following was added to this version of the file by: MSpencer
	// ADC_SCKI
	switch( param->adc_scki )
	{
		case CLK_SRC_OSC_1X  : break;
		case CLK_SRC_OSC_DIV2: cfg |= FPGA_AHCLKR2_S2; break;
		default              : return -1;
	}

	// DAC_SCKI
	switch( param->dac_scki )
	{
		case CLK_SRC_OSC_1X  : break;
		case CLK_SRC_OSC_DIV2: cfg |= FPGA_AHCLKX1_S2; break;
		default              : return -1;
	}



	// Write new configuration
	FPGA_CLOCKS_CTRL = cfg;	
	return 0;
}

/**************************************************************************** 
 *
 * NAME  
 *      CLKGEN_Reset
 *
 * PURPOSE:
 *		Initialise the sample rate generator with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int CLKGEN_Reset(); 
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
far int CLKGEN_Reset()
{
    CLKGEN_Params params = CLKGEN_DEFAULT_PARAMS;

	// Set clock genrator to default values
	return CLKGEN_Init( &params );
}

/**************************************************************************** 
 *
 * NAME  
 *      CLKGEN_OscSel
 *
 * PURPOSE:
 *		Select the oscillator to use as audio clock
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int CLKGEN_OscSel( 
 *				int osc,		// (i) oscillator id
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
far int CLKGEN_OscSel( int osc )
{
	// Select the desired oscillator
	GPIO_DAT1 = (GPIO_DAT1 &~ OSC_SEL_BIT) | osc;

	return 0;
}

