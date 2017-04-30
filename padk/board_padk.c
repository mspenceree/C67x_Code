
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
 * File: board_padk.c
 *
 * PADK H/W specific source file.
 * This file contains functions to initialize and configure the PADK hardware.
 *
 * This is a modified version from the "echo" demo.
 *
 *
 */

 
/* PADK board header file */
#include <board_padk.h>
#include <mem.h>

/*
    PADK_bord_control () is used to control and configure
    the PADK H/W. This function gets as argument the handle
    to the board instance and also the CMD.
    The commands supported by the control function are defined 
    in the header file board_padk.h, enum PADK_brdCmd
*/


Int32 PADK_board_control (PADK_brdHdl hPADK, Uns cmd)
{
    /* The PADK_brdCfg structure is defined in board_padk.h */
    PADK_brdCfg * pBrdCfg;
    Int32 status;

    /*
        Initialize the status to reflect NoError. The board
        specific errors are defined in the header file 
        board_padk.h, enum PADK_err
    */
    status = PADK_NOERR;
    pBrdCfg = hPADK;

    switch (cmd) {
    /* Start the ADC circuitry */
    case PADK_START_INPUT:
        pBrdCfg->padkInt->adcParams.format = ADC_FMT_24B_LEFT_JUST;
        pBrdCfg->padkInt->adcParams.rate = ADC_RATE_SINGLE;
        ADC_Init(&pBrdCfg->padkInt->adcParams);
        break;
    /* Start the DAC circuitry */
    case PADK_START_OUTPUT:
        pBrdCfg->padkInt->dacParams.mute = DAC_MUTE_OFF;
        pBrdCfg->padkInt->dacParams.format = DAC_FMT_24B_LEFT_JUST;
        pBrdCfg->padkInt->dacParams.deemphasis = DAC_DEM_OFF;
        pBrdCfg->padkInt->dacParams.fs = DAC_FS_SINGLERATE;
        DAC_Init(&pBrdCfg->padkInt->dacParams);
        break;
    /* Mute the PADK output */
    case PADK_MUTE_OUTPUT:
        DAC_RelayMute();
        break;        
    /* Un-mute the PADK output */
    case PADK_UNMUTE_OUTPUT:
        DAC_RelayUnmute();
        break;        
    /* If here, return with error */
    default:
        status = PADK_ENOTIMPL;
        break;
    }

    return (status);
}

/*
    PADK_bord_open () is used to opne an instance of PADK
    H/W. This function creates an handle to allow the F/W
    to interface with the PADK H/W.
    This function gets as argument memory segment ID from
    which to allocate memory to create the handle.
*/


PADK_brdHdl PADK_board_open (Int32 segid)
{
    /* The PADK_brdCfg structure is defined in board_padk.h */
    PADK_brdCfg * pBrdCfg;

    /* Allocate the PADK_brdCfg structure */
    pBrdCfg = (Void *)MEM_calloc (segid, sizeof(PADK_brdCfg), 8);
    pBrdCfg->padkInt = (Void *)MEM_calloc (segid, sizeof(PADK_internal), 8);
    
    if (!pBrdCfg || !pBrdCfg->padkInt) {
        return (NULL);
    }

    /* 
        PADK_board_control function is the interaface
        for the F/W to the PADK H/W
    */
    pBrdCfg->brdCtrl = PADK_board_control;

// commented out by MSpencer    pBrdCfg->padkInt->clkgenParams.osc_sel = OSC_22_5792MHz; 
// Added by MSpencer fixthis shit:
// 	For 44.1KHz use OSC_22_5792MHz
//	For 48.0KHz use OSC_24_576MHz 
    pBrdCfg->padkInt->clkgenParams.osc_sel = OSC_22_5792MHz; 

    pBrdCfg->padkInt->clkgenParams.adc_dir_clk_src = CLK_SRC_OSC_1X;
    pBrdCfg->padkInt->clkgenParams.dac_dit_clk_src = CLK_SRC_OSC_1X;

	// This LOWERS the sampling rate to 24KHz (for 24 osc_sel)!!! ???:
	// Changing it back does nothing unless I cycle power.
	//pBrdCfg->padkInt->clkgenParams.adc_dir_clk_src = CLK_SRC_OSC_2X;
    //pBrdCfg->padkInt->clkgenParams.dac_dit_clk_src = CLK_SRC_OSC_2X;

	// And this has no effect!!! ???:
	//pBrdCfg->padkInt->clkgenParams.adc_scki = CLK_SRC_OSC_DIV2;
    //pBrdCfg->padkInt->clkgenParams.dac_scki = CLK_SRC_OSC_DIV2;

    /* Initialize the clock generation circuitry */
    CLKGEN_Init(&pBrdCfg->padkInt->clkgenParams);

    /* Return with the board instance */    
    return ((PADK_brdHdl) pBrdCfg);
}


