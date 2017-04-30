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
 *  File        : PADK_DIT.h
 *  Description	: Digital Audio Transmitter Driver Header File
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
#ifndef __PADK_DIT_H__
#define __PADK_DIT_H__

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct { 
	
	unsigned char dummy;

} DIT_Params;


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      DIT_Init
 *
 * PURPOSE:
 *		Initialise the transmetter.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIT_Init( 
 *				DIT_Params *param,		// (i) Initialisation parameters
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
far int DIT_Init( DIT_Params *param );


/**************************************************************************** 
 *
 * NAME  
 *      DIT_Reset
 *
 * PURPOSE:
 *		Initialise the transmetter with default values.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DIT_Reset(); 
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
far int DIT_Reset();


#endif	// __PADK_DIT_H__
