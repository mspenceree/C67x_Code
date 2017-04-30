
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
 * File: padk.h
 *  Description	: Evaluation Board Driver Header File
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
#ifndef __PADK_H__
#define __PADK_H__


/****************************************************************************
 *                                Constants                                 *
 ****************************************************************************/

#define SDRAM_CE 0x80000000			// SDRAM
#define ASYNC_CE 0x90000000			// FPGA / FLASH / UART


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK_I2C.h"
#include "PADK_ADC.h"
#include "PADK_DAC.h"
#include "PADK_DIR.h"
#include "PADK_DIT.h"
#include "PADK_USB.h"
#include "PADK_GPIO.h"
#include "PADK_UART.h"
#include "PADK_MIDI.h"
#include "PADK_CONV.h"
#include "PADK_FPGA.h"
#include "PADK_FLASH.h"
#include "PADK_MCASP.h"
#include "PADK_CLKGEN.h"
#include "PADK_I2CMEM.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      PADK_Init
 *
 * PURPOSE:
 *		Reset all external peripherals.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int PADK_Init(); 
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
far int PADK_Init();

#endif	// __PADK_H__

