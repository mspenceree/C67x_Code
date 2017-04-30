
/**************************************************************************************************
 *                                                                                                *
 *                                      SSSSS    PPPPPP    SSSSS                                  * 
 *                                    SS   SS   PP   PP  SS   SS                                  * 
 *                                   SS        PP   PP  SS                                        *
 *                                   SSSSS    PPPPPP    SSSSS                                     *
 *                                      SS   PP            SS                                     *
 *                                SS   SS   PP       SS   SS                                      *
 *                                SSSSS    PP        SSSSS                                        *
 *                                                                                                *
 *                               SIGNAL  PROCESSING  SOLUTIONS                                    *
 *                                                                                                *
 **************************************************************************************************
 *                                                                                                *
 *                  Project:                                  *
 *                                                                                                *
 *                  Author:     Michael E. Spencer, Ph.D.                                         *
 *                                                                                                *
 *                  Copyright(c) Signal Processing Solutions, 2007                                *
 *                                                                                                *
 **************************************************************************************************
 *
 * File: pollAuxAnalogInTask.c
 *
 *  This task reads the first three 8-bit values from the ADS7830 analog to digital converer.
 *  The input signals are available on the MIDI port. The gloabal variable below are updated
 *  approximatly every 50 msec.
 *
 *      float   AuxAnalogIn1    A/D input value from 0.0 to 255.0 (pin 4 of MIDI connector)
 *      float   AuxAnalogIn2    A/D input value from 0.0 to 255.0 (pin 9 of MIDI connector)
 *      float   AuxAnalogIn3    A/D input value from 0.0 to 255.0 (pin 5 of MIDI connector)
 *      int     AuxAnalogInOK   0-reading invalid, 1-reading valid
 *
 *  I can add first order RC filter to smooth chatter.
 * 
 *  M. Spencer
 */

// PADK Library
#include "PADK.h"

#include <algorithms.h>

#include <main.h>


/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;


//--------------------------------------------------------
// Declare global parameters here
//--------------------------------------------------------
float AuxAnalogIn1;
float AuxAnalogIn2;
float AuxAnalogIn3;
int AuxAnalogInOK = 0;

//extern pGAIN  GAIN_Handles[ GAIN_BLOCK_MAX_INSTANCE ];


void pollAuxAnalogInTask ()
{

//  float mindB, m, g;

//  mindB = -50.0;
//  m = -mindB/255.0;


    LOG_printf(&trace, "Entered pollAuxAnalogInTask.c");

    while(1){

// i = (int) PRD_getticks();

//      UART_EnableLed2(1); // flash LED while converting (this is a very fast routine)


        //--------------------------------------------------------
        // Start A/D conversion and get result for all channels:
        //--------------------------------------------------------

        CONV_StartConvertion(0);    // Switch to the specified input (0 to 2) and start conversion. (Hopefully if I2C is busy this command will wait. )
//      TSK_sleep (1);
        AuxAnalogIn1 = (float) CONV_GetConversionResult();  // Read the result of a conversion.

        CONV_StartConvertion(1);    // Switch to the specified input (0 to 2) and start conversion. (Hopefully if I2C is busy this command will wait. )
//      TSK_sleep (1);
        AuxAnalogIn2 = (float) CONV_GetConversionResult();  // Read the result of a conversion.

        CONV_StartConvertion(2);    // Switch to the specified input (0 to 2) and start conversion. (Hopefully if I2C is busy this command will wait. )
//      TSK_sleep (1);
        AuxAnalogIn3 = (float) CONV_GetConversionResult();  // Read the result of a conversion.

        AuxAnalogInOK = 1;

//LOG_printf(&trace, "AuxAnalogIn1 = %d ", (int) AuxAnalogIn1 );
//LOG_printf(&trace, "AuxAnalogIn2 = %d ", (int) AuxAnalogIn2 );
//LOG_printf(&trace, "AuxAnalogIn3 = %d ", (int) AuxAnalogIn3 );




/* Disable POTs now:

//  test gain control:
GAIN_Handles[2]->slowFlag = 1;
GAIN_Handles[2]->dbFlag = 1;
g = m*AuxAnalogIn1 + mindB; // -40 to 0dB, AsacsSpeakerVolume, Pot#1
if( AuxAnalogIn1 < 5 )
    g = -100.0;
GAIN_Handles[2]->gain = g;
GAIN_update( GAIN_Handles[2] );

GAIN_Handles[0]->slowFlag = 1;
GAIN_Handles[0]->dbFlag = 1;
g = m*AuxAnalogIn2 + mindB; // -40 to 0dB, AsacsSpeakerMonitorLevel, Pot#2
if( AuxAnalogIn2 < 5 )
    g = -100.0;
GAIN_Handles[0]->gain = g;
GAIN_update( GAIN_Handles[0] );

GAIN_Handles[3]->slowFlag = 1;
GAIN_Handles[3]->dbFlag = 1;
g = m*AuxAnalogIn3 + mindB + 12.0;  // (-40 to 0dB) + 12, AsacsListenBackMonitorLevel, Pot#3
if( AuxAnalogIn3 < 5 )
    g = -100.0;
GAIN_Handles[3]->gain = g;
GAIN_update( GAIN_Handles[3] );

*/


//      UART_EnableLed2(0);

        TSK_sleep (40); // Sleep here for  msec 

    }   


}
