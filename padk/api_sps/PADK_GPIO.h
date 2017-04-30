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
 *  File        : PADK_GPIO.h
 *  Description	: GPIO Interface Driver header file
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
#ifndef __PADK_GPIO_H__
#define __PADK_GPIO_H__

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      GPIO_Reset
 *
 * PURPOSE:
 *		Initialises all the GPIO pins needed to access the PADK peripherals.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int GPIO_Reset() 
 *
 * RETURN VALUE
 *		status
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int GPIO_Reset();


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_GetDIPSwitch
 *
 * PURPOSE:
 *		Get the state of one of the DIP switches.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int GPIO_GetDIPSwitch( int swt ) 
 *
 *      swt - (i) DIP switch number [0-7]
 *
 * RETURN VALUE
 *		state (0 or 1) or error (-1)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int GPIO_GetDIPSwitch( int swt );


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_EnableLed
 *
 * PURPOSE:
 *		Enanle/Disable one of the LEDs.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int GPIO_EnableLed( int led, int enable ) 
 *
 *      led    - (i) led number [0-7]
 *      enable - (i) 1->enable, 0->disable
 *
 * RETURN VALUE
 *		status
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int GPIO_EnableLed( int led, int enable );


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_SetBCD
 *
 * PURPOSE:
 *		Set the BCD value.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int GPIO_SetBCD( unsigned char val ) 
 *
 *      val    - (i) bcd value [0-255]
 *
 * RETURN VALUE
 *		status
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int GPIO_SetBCD( unsigned char val );


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_GetPushButton
 *
 * PURPOSE:
 *		Get the state of one of the push button.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int GPIO_GetPushButton( int btn ) 
 *
 *      btn - (i) Push button number [1,2]
 *
 * RETURN VALUE
 *		state (0 or 1) or error (-1)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int GPIO_GetPushButton( int btn );


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_GetDeltaPushButton
 *
 * PURPOSE:
 *		Verify if the button has been pressed.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int GPIO_GetDeltaPushButton( int btn ) 
 *
 *      btn - (i) Push button number [1,2]
 *
 * RETURN VALUE
 *		state (0 or 1) or error (-1)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int GPIO_GetDeltaPushButton( int btn );


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_ClearDeltaPushButton
 *
 * PURPOSE:
 *		Clear the state of the push button.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void GPIO_ClearDeltaPushButton( int btn ) 
 *
 *      btn - (i) Push button number [1,2]
 *
 * RETURN VALUE
 *		none
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far void GPIO_ClearDeltaPushButton( int btn );

#endif // __PADK_GPIO_H__
