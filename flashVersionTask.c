
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
 * File: flashVersionTask.c
 *
 *  This task flashes the Code version number out of the UART_EnableLed2.
 *
 *  eg. V2.3 Flashes: * *   * * *      repeat
 *
 * 
 *  M. Spencer
 */

// PADK Library
#include "PADK.h"

//#include <algorithms.h>

#include <main.h>


/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;

extern int swVersion;       // Software version int
extern int swVersionFrac;   // Software version frac. eg. Ver int.frac


void flashVersionTask ()
{
    int i;

    LOG_printf(&trace, "Entered flashVersionTask.c");

    TSK_sleep (1000);   // Sleep here for x msec 

    while(1){

        for( i=0; i<swVersion; i++)
        {
            UART_EnableLed2(1);     // LED on (this is a very fast routine)
            TSK_sleep (80);     // Sleep here for x msec
            UART_EnableLed2(0);     // LED off
            TSK_sleep (300);        // Sleep here for x msec
        }

        TSK_sleep (80); // Sleep here for x  msec

        // Quick flash for decimal point?:
        //UART_EnableLed2(1);       // LED on
        TSK_sleep (8);          // Sleep here for x msec
        UART_EnableLed2(0);     // LED off
        TSK_sleep (400);        // Sleep here for x msec


        for( i=0; i<swVersionFrac; i++)
        {
            UART_EnableLed2(1);     // LED on
            TSK_sleep (80);     // Sleep here for x msec
            UART_EnableLed2(0);     // LED off
            TSK_sleep (300);        // Sleep here for x msec
        }

        TSK_sleep (1800);   // Sleep here for x msec 

    }   


}
