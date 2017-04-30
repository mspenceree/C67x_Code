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
 *  File        : PADK_DIR.h
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
#ifndef __PADK_DIR_H__
#define __PADK_DIR_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct {
	
	// Control 1
	unsigned char swclk;
	unsigned char mute;
	unsigned char hold;
	unsigned char chs;

	// Control 2
	unsigned char deemphasis;	

	// Control 4
	unsigned char run;
	unsigned char rxsel;
	
	// Control 5
	unsigned char soms;
	unsigned char sores;
	unsigned char sodel;
	unsigned char sopol;
	unsigned char solrpol;

	// Control 6
	unsigned char rxerr_mask;

	// Control 7
	unsigned char int_mask;

} DIR_Params;


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

//////////////////////////
//	  device address	//
//////////////////////////
#define CS8416_DEVICE				0x10
#define CS8427_DEVICE				0x14

//////////////////////////
//    data input type   //
//////////////////////////
#define RCA_INPUT 					0
#define OPTO_INPUT 					1

//////////////////////////
//   register address   //
//////////////////////////

#define CONTROL_1					0x01
#define CONTROL_2					0x02
#define CONTROL_3					0x03
#define CONTROL_4					0x04
#define CLOCK_SRC_CTRL				0x04 	// for 8427
#define SERIAL_AUDIO_DATA_FMT		0x05
#define SERIAL_INPUT_FMT			0x05
#define RCV_ERROR_MASK				0x06
#define INT_MASK					0x07
#define WORD_ERROR_MASK				0x11

//////////////////////////
// control register 01h //
//////////////////////////

// SWCLK - RMCK output when PLL looses lock
#define DIR_RMCK_DEFAULT	0x00	// [7] - 0
#define DIR_RMCK_OMCK		0x80	// [7] - 1

// MUTE - Mute control for the serial audio output port
#define DIR_UNMUTE			0x00	// [6] - 0
#define DIR_MUTE			0x40	// [6] - 1

// HOLD - Determine how received audio sample is affected when a receive error occurs
#define DIR_HOLD_LAST		0x00	// [4-3] - 00
#define DIR_REPLACE_W_0		0x04	// [4-3] - 01
#define DIR_DO_NOTHING		0x08	// [4-3] - 10

// CHS - Sets which channel's C data is decoded in the Receiver Channel Status register (0Ah) 
#define DIR_CHS_DECODE_CH_A	0x00	// [0] - 0
#define DIR_CHS_DECODE_CH_B	0x01	// [0] - 1

//////////////////////////
// control register 02h //
//////////////////////////

// EMPH - De-emphasis filter control
#define DIR_DEEMPH_OFF		0x00 	// [6-4] - 000
#define DIR_DEEMPH_32kHz	0x10	// [6-4] - 001
#define DIR_DEEMPH_44kHz	0x20	// [6-4] - 010
#define DIR_DEEMPH_48kHz	0x30	// [6-4] - 011
#define DIR_DEEMPH_50_15us	0x40	// [6-4] - 100

//////////////////////////
// control register 04h //
//////////////////////////

// RUN - Internal clock controls ( allow "power-down" mode ) 
#define DIR_CLOCK_STOP		0x00	// [7] - 0
#define DIR_CLOCK_RUN		0x80	// [7] - 1 

// RXSEL - Select RXP0 to RXP7 for input to the receiver
#define DIR_RXSEL_RCA		0x00	// [5-3] - 000	
#define DIR_RXSEL_OPTO		0x08	// [5-3] - 001

//////////////////////////
// control register 05h //
//////////////////////////

// SOMS - Master or slave of its serial port
#define DIR_IS_SLAVE		0x00
#define DIR_IS_MASTER		0x80

// SORES - Resolution of the output on SDOUT
#define DIR_RES_24_BIT		0x00	// [5-4] - 00
#define DIR_RES_20_BIT		0x10	// [5-4] - 01 
#define DIR_RES_16_BIT		0x20	// [5-4] - 10 
#define DIR_RES_COPY		0x30	// [5-4] - 11 

// SODEL - Delay of SDOUT data relative to OLRCK
#define DIR_DELAY_0			0x00	// [2] - 0
#define DIR_DELAY_1			0x04	// [2] - 1

// SOPOL - OSCLK polarity
#define DIR_POL_RISING		0x00	// [1] - 0
#define DIR_POL_FALLING		0x02	// [1] - 1

// SOLRPOL - OLRCK polarity, channel when OLRCK is high
#define DIR_LRPOL_LEFT		0x00	// [0] - 0
#define DIR_LRPOL_RIGHT		0x01	// [0] - 1

// CHST - Receiver Channel Status
#define DIR_CHST_EMPHASIS	0x01
#define DIR_CHST_ORIG		0x02
#define DIR_CHST_COPY		0x04
#define DIR_CHST_PRO		0x08
#define DIR_CHST_AUX_NONE	0x00
#define DIR_CHST_AUX_1BIT	0x10
#define DIR_CHST_AUX_2BIT	0x20
#define DIR_CHST_AUX_3BIT	0x30
#define DIR_CHST_AUX_4BIT	0x40
#define DIR_CHST_AUX_5BIT	0x50
#define DIR_CHST_AUX_6BIT	0x60
#define DIR_CHST_AUX_7BIT	0x70
#define DIR_CHST_AUX_8BIT	0x80
#define DIR_CHST_AUX_MASK	0xF0

// DFMT - Format detected
#define DIR_DFMT_96kHz		0x01
#define DIR_DFMT_SIL		0x02
#define DIR_DFMT_DTS_CD		0x08
#define DIR_DFMT_DTS_LD		0x10
#define DIR_DFMT_IEC61937	0x20
#define DIR_DFMT_PCM		0x40

// RXERR - Receiver Error
#define DIR_RXERR_PARITY	0x01
#define DIR_RXERR_BI_PHASE	0x02
#define DIR_RXERR_CONF		0x04
#define DIR_RXERR_V_BIT		0x08
#define DIR_RXERR_UNLOCK	0x10
#define DIR_RXERR_CRC		0x20
#define DIR_RXERR_QCRC		0x40
#define DIR_RXERR_ALL		0x7F

// INT - Interrupt Status
#define DIR_INT_FCH			0x01
#define DIR_INT_QCH			0x02
#define DIR_INT_RERR		0x04
#define DIR_INT_CCH			0x08
#define DIR_INT_DETC		0x10
#define DIR_INT_OSLIP		0x20
#define DIR_INT_PCCH		0x40
#define DIR_INT_ALL			0x7F

// Default DIR Params
#define DIR_DEFAULT_PARAMS {       \
	DIR_RMCK_OMCK,				   \
	DIR_UNMUTE,                    \
	DIR_HOLD_LAST,                 \
	DIR_CHS_DECODE_CH_A,           \
	DIR_DEEMPH_OFF,                \
	DIR_CLOCK_RUN,                 \
	DIR_RXSEL_RCA,                 \
	DIR_IS_MASTER,                 \
	DIR_RES_24_BIT,                \
	DIR_DELAY_0,                   \
	DIR_POL_RISING,                \
	DIR_LRPOL_LEFT,                \
	DIR_RXERR_ALL,                 \
	DIR_INT_ALL                    \
}


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
 *				DIR_Params *param,		// (i) Initialisation parameters
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
far int DIR_Init( DIR_Params *param );


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
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int DIR_Reset();

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
far int DIR_Mute();

/**************************************************************************** 
 *
 * NAME  
 *      DIR_Unmute
 *
 * PURPOSE:
 *		Unmutes receiver output
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
far int DIR_Unmute();

/**************************************************************************** 
 *
 * NAME  
 *      DIR_ReadReg
 *
 * PURPOSE:
 *		Read CS4816 register (RAW access). This routine directly calls 
 *		from the I2C module. Do not use this if using DSPBios to handle
 *		the I2C module.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIR_ReadReg( 
 *			unsigned reg, 			// (i) register address to read
 *			unsigned char *reg_val 	// (i) pointer where to store the read
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
far int DIR_ReadReg( unsigned reg, unsigned char *reg_val );

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
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int DIR_WriteReg( unsigned reg, unsigned char reg_val );

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
 *			unsigned cfg 	// (i) New input line
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
far int DIR_CfgRxSel( unsigned sel );


#endif	// __PADK_DIR_H__
