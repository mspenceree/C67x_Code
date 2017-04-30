
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
 * File: zzzBackgroundLoopIdle.c

    This is part of the background Idle loop where I can do all low priority prosessing.
    This function is called repeatedly by the DSP/BIOS and must not block (it must return).

    Current functions:

    1.  I toggles the state of the xxxxx LED upon entry (serves as a DSP activity indicator and as a
        means to look at "Idle" condition of the DSP with an osciloscope).

    2.  It computes the FFTs for the amplifier/speaker tests.



    M.Spencer


 **************************************************************************************************
 */
// PADK Library
#include "PADK.h"

#include <main.h>


/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;


//--------------------------------------------------------
// Declare global parameters here
//--------------------------------------------------------
int LEDtoggle = 0;



void zzzBackgroundLoopIdle()
{
//  int i;
//  unsigned char InPort0, InPort1, InPort2;

//  int initializeOK = 0;
//  int readOK;


//  LOG_printf( &trace, "Entered zzzBackgroundLoopIdle.c" );


//  for( i=0; i<1000000; i++) { }   // kill some time


//  UART_EnableLed2( LEDtoggle ^= 1 );  // Toggle LED state (this is a very fast routine)






}
