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
 *  File        : PADK_DAC.c
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
#ifndef __PADK_DAC_H__
#define __PADK_DAC_H__


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"

/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct {
	
    unsigned fs;
	unsigned char mute;
    unsigned char format;
    unsigned char deemphasis;

} DAC_Params;

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Sampling Mode
#define DAC_FS_SINGLERATE       0x0000      // 32, 44.1, 48 kHz
#define DAC_FS_DUALRATE         0x0100      // 88.2, 96 kHz
#define DAC_FS_QUADRATE         0x0200      // 176.4, 192 kHz
#define DAC_FS_MASK             0x0300

// Audio Data format
#define DAC_FMT_24B_LEFT_JUST   0x0000
#define DAC_FMT_24B_I2S         0x0010
#define DAC_FMT_MASK            0x0010

// Mute Controls
#define DAC_MUTE_OFF            0x0000
#define DAC_MUTE_ON             0x0020
#define DAC_MUTE_MASK           0x0020

// Digital De-Emphasis
#define DAC_DEM_OFF             0x0000
#define DAC_DEM_48KHZ           0x0040
#define DAC_DEM_441KHZ          0x0080
#define DAC_DEM_32KHZ           0x00C0
#define DAC_DEM_MASK            0x00C0


// Default DAC parameters
#define DAC_DEFAULT_PARAMS {       \
	DAC_FS_SINGLERATE,              \
	DAC_MUTE_OFF,                   \
	DAC_FMT_24B_LEFT_JUST,          \
	DAC_DEM_OFF                     \
}


/*
 * Control Register Definitions - Software Mode Only
 * (Not supported)

// Device
#define DAC_DEVICE_1            0x01
#define DAC_DEVICE_2            0x02

// Channel
#define DAC_CHANNEL_1           0x01
#define DAC_CHANNEL_2           0x02
#define DAC_CHANNEL_3           0x03
#define DAC_CHANNEL_4           0x04

// Register 1
#define DAC_REG1_ATT_DB(dB)	        (255-(((unsigned)dB)>>1))
#define DAC_REG1_ATT_MUTE			0x00

// Register 2
#define DAC_REG2_ATT_DB(dB)	        (255-(((unsigned)dB)>>1))
#define DAC_REG2_ATT_MUTE			0x00

// Register 3
#define DAC_REG3_ATT_DB(dB)	        (255-(((unsigned)dB)>>1))
#define DAC_REG3_ATT_MUTE			0x00

// Register 4
#define DAC_REG4_ATT_DB(dB)	        (255-(((unsigned)dB)>>1))
#define DAC_REG4_ATT_MUTE			0x00

// Register 5
#define DAC_REG5_DEM_OFF            0x00
#define DAC_REG5_DEM_48KHZ          0x01
#define DAC_REG5_DEM_441KHZ         0x02
#define DAC_REG5_DEM_32KHZ          0x03
#define DAC_REG5_PHASE_NORMAL       0x00
#define DAC_REG5_PHASE_INVERTED     0x04
#define DAC_REG5_ZDM_OFF            0x00
#define DAC_REG5_ZDM_ON             0x08
#define DAC_REG5_MUTE_CH1           0x10
#define DAC_REG5_MUTE_CH2           0x20
#define DAC_REG5_MUTE_CH3           0x40
#define DAC_REG5_MUTE_CH4           0x80

// Register 6
#define DAC_REG6_FS_SIGLERATE       0x00
#define DAC_REG6_FS_DUALRATE        0x01
#define DAC_REG6_FS_QUADRATE        0x02
#define DAC_REG6_PDN12_OFF          0x00
#define DAC_REG6_PDN12_ON           0x04
#define DAC_REG6_PDN34_OFF          0x00
#define DAC_REG6_PDN34_ON           0x08
#define DAC_REG6_SOFT_RESET         0x80

// Register 7
#define DAC_REG7_FMT_24B_LEFT_JUST  0x00
#define DAC_REG7_FMT_24B_I2S        0x01
#define DAC_REG7_FMT_TDM_0_DELAY    0x02
#define DAC_REG7_FMT_TDM_1_DELAY    0x03
#define DAC_REG7_FMT_24B_RIGHT_JUST 0x04
#define DAC_REG7_FMT_20B_RIGHT_JUST 0x05
#define DAC_REG7_FMT_18B_RIGHT_JUST 0x06
#define DAC_REG7_FMT_16B_RIGHT_JUST 0x07
#define DAC_REG7_FMT_LRCKP_NORMAL   0x00
#define DAC_REG7_FMT_LRCKP_INVERTED 0x10
#define DAC_REG7_FMT_BCKE_RISING    0x00
#define DAC_REG7_FMT_BCKE_FALLING   0x20

*/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      DAC_Init
 *
 * PURPOSE:
 *		Initialise the SPI port to access the digital-to-analog converter and
 *      restore default configuration.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DAC_Init( 
 *              DAC_Params *param,		// (i) Initialisation parameters
 *		);
 *
 * RETURN VALUE
 *		Error code
 *		
 * REFERENCE
 *      Burr-Brown PCM4204 Datasheet (SBAS327A), Texas Instruments Inc.
 *
 * NOTE     
 *
 ****************************************************************************/ 
far int DAC_Init(DAC_Params *param );

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
 *      Burr-Brown PCM4204 Datasheet (SBAS327A), Texas Instruments Inc.
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int DAC_Reset();

/**************************************************************************** 
 *
 * NAME  
 *      DAC_Mute
 *
 * PURPOSE:
 *		Mutes the output of a channel of a DAC device
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
 *      Burr-Brown PCM4204 Datasheet (SBAS327A), Texas Instruments Inc.
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_Mute();

/**************************************************************************** 
 *
 * NAME  
 *      DAC_Unmute
 *
 * PURPOSE:
 *		Unmutes the output of a channel of a DAC device
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
 *      Burr-Brown PCM4204 Datasheet (SBAS327A), Texas Instruments Inc.
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_Unmute();

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
 *      Burr-Brown PCM4204 Datasheet (SBAS327A), Texas Instruments Inc.
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_RelayUnmute();

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
 *      Burr-Brown PCM4204 Datasheet (SBAS327A), Texas Instruments Inc.
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void DAC_RelayMute();


#endif	// __PADK_DAC_H__

