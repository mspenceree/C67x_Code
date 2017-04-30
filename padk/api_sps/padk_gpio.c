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
 *  File        : PADK_GPIO.c
 *  Description	: GPIO Interface Driver
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
 *                             Local constants                              *
 ****************************************************************************/
#define LED_I2C_ADDR    0x20
#define DIP_I2C_ADDR    0x21
#define BCD_I2C_ADDR    0x22
static unsigned char led_stat;

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
far int GPIO_Reset()
{
    int res = 0;
	led_stat = 0xFF;
	res |= GPIO_EnableLed( 0, 0 );
	res |= GPIO_EnableLed( 1, 0 );
	res |= GPIO_EnableLed( 2, 0 );
	res |= GPIO_EnableLed( 3, 0 );
	res |= GPIO_EnableLed( 4, 0 );
	res |= GPIO_EnableLed( 5, 0 );
	res |= GPIO_EnableLed( 6, 0 );
	res |= GPIO_EnableLed( 7, 0 );
	res |= GPIO_SetBCD( 0x00 );

    return res;
}

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
far int GPIO_GetDIPSwitch( int swt ) 
{
    unsigned char dip;

    if ( I2C_WriteVal(DIP_I2C_ADDR, 0xFF) < 0 ) return -1;
    if ( I2C_ReadVal(DIP_I2C_ADDR, &dip) < 0 ) return -1;
    return !((dip >> swt) & 1);  
}


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_EnableLed
 *
 * PURPOSE:
 *		Enable/Disable one of the LEDs.
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
far int GPIO_EnableLed( int led, int enable ) 
{
	led_stat |= (1<<led);
    if (enable) led_stat &= ~(1<<led);

    if ( I2C_WriteVal(LED_I2C_ADDR, led_stat) < 0 ) return -1;
    return 0;
}



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
far int GPIO_SetBCD( unsigned char val ) 
{
    if ( I2C_WriteVal(BCD_I2C_ADDR, ~val) < 0 ) return -1;
    return 0;
}


/**************************************************************************** 
 *
 * NAME  
 *      GPIO_GetPushButton
 *
 * PURPOSE:
 *		Get the curent state of one of the push button.
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
far int GPIO_GetPushButton( int btn ) 
{
    return !((FPGA_BUTTON_STATUS >> (1+btn)) & 1);
}

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
far int GPIO_GetDeltaPushButton( int btn ) 
{
    return (FPGA_BUTTON_STATUS & btn);
}


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
far void GPIO_ClearDeltaPushButton( int btn ) 
{
    FPGA_BUTTON_STATUS |= btn;
}

