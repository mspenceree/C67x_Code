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
 *  File        : PADK_CLKGEN.h
 *  Description	: Clock Generator Driver
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.8 $"
 * "$Name:  $"
 * "$Date: 2006/02/06 19:15:20 $"
 * "$Author: interne\francois.langlais $"
 *
 ***************************************************************************/
#ifndef __PADK_CLKGEN_H__
#define __PADK_CLKGEN_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct {

	unsigned osc_sel;    // Oscillator Selection
	int adc_dir_clk_src; // Clock source for the ADCs and DIR (ahclkr0-ahclkr1)
	int dac_dit_clk_src; // Clock source for the DACs and DIT (ahclkx0-ahclkx2)
	int adc_scki;        // System clock source for the ADCs
	int dac_scki;        // System clock source for the DACs
} CLKGEN_Params;

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/
//
// Oscillator selection
//
// Oscillator 24.576MHz used to generate sampling frequencies of
// 16, 32, 48, 96, 192 kHz 
#define OSC_24_576MHz		(0<<30)
// Oscillator 22.5792MHz used to generate sampling frequencies of
// 44.1, 88.2, 176.4 kHz
#define OSC_22_5792MHz		(1<<30)

#define OSC_SEL_BIT			(1<<30)

// Clock sources
#define CLK_SRC_OSC			(0)
#define CLK_SRC_OSC_2X		(1)
#define CLK_SRC_AES			(2)
#define CLK_SRC_AES_2X		(3)
#define CLK_SRC_WORD		(4)
#define CLK_SRC_WORD_2X		(5)

// SCKI sources
#define CLK_SRC_OSC_1X      (0)
#define CLK_SRC_OSC_DIV2    (1)

// Default params
#define CLKGEN_DEFAULT_PARAMS {    \
	OSC_24_576MHz,                 \
    CLK_SRC_OSC,                   \
	CLK_SRC_OSC,				   \
	CLK_SRC_OSC_1X,                \
	CLK_SRC_OSC_1X                 \
}

#define GPIO_EN 		(*((volatile unsigned *)0x4300000C))
#define GPIO_DIR1 		(*((volatile unsigned *)0x43000010))
#define GPIO_DAT1 		(*((volatile unsigned *)0x43000014))

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
far int CLKGEN_Init( CLKGEN_Params *param );


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
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int CLKGEN_Reset();

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
far int CLKGEN_OscSel( int osc );


#endif	// __PADK_CLKGEN_H__



