
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
 * File: pollRangeFinderTask.c
 *
 *  This functions monitors the UART port for input from the Rense (www.rense.nl)
 *  Model HT-923 Temperature/Hunidity Probe. 
 *
 *  After the task is called initally to set up some parameters, it loops - polling the
 *  UART port for the input stream. When a valid reading is made it is filtered with a
 *  length-5 median filter to eliminate out-liers. Finally, the following global
 *  variables are updated about twice per second:
 *
 *      float   Range       in meters, xx to xxx calibrated, can read from xx to xxx
 *      int     RangeOK     0-reading invalid, 1-reading valid
 *
 * 
 *  M. Spencer
 */

// PADK Library
#include "PADK.h"

#include <main.h>


#define RANGE_FINDER_BUAD_RATE  9600

// For PADK UART Module:
UART_Params uartParams = UART_DEFAULT_PARAMS;
#define UART_WAIT       1
#define UART_NO_WAIT    0

// For trace LOG buffer used to print runtime information:
extern far LOG_Obj trace;


//--------------------------------------------------------
// Declare global parameters here
//--------------------------------------------------------
float Range;
int RangeOK = 0;
float RangeSampleHist[ 4 ];


void pollRangeFinderTask ()
{
    unsigned char datachr, datastr[16];
    int i, num;

    float RangeSample;
    unsigned char Rangestr[10];
    int Rangemsec = 0;
    int Rangeidx = 0;
    int Rangestridx = 0;
    int Rangecount = 0;


 LOG_printf(&trace, "Entered pollRangeFinderTask.c");


    //--------------------------------------------------------
    // Initialize the UART module modifing default values
    //--------------------------------------------------------
    uartParams.baud = RANGE_FINDER_BUAD_RATE;
    UART_Init( &uartParams );

    while(1) { // fixthis the UART and MIDI ports seem to "not get along" together?

        //------------------------------------------------------------------------------------------
        // Read up to 16 characters from the UARTs 16 Byte FIFO. If any more than
        // 16 characters were sent before this loop repeats then characters will be lost.
        //
        // Typical input data examples are:
        //
        //                  "Range= ???, CR(0x0d), LF(0x0a)
        //                  "Range= ???, CR(0x0d), LF(0x0a)
        //                  "Range= ???, CR(0x0d), LF(0x0a)
        //   Position:       012345..   48        49
        //
        //------------------------------------------------------------------------------------------

        num = UART_Read( datastr, 16, UART_NO_WAIT );
        
        if (num) {              // One or more characters have been read.

            UART_EnableLed1( 1 );   // flash LED when something received (this is a very fast routine)


 LOG_printf(&trace, "Number of UART Bytes read = %d", num);

            //for (i = 0; i < num; i++) {
            //  LOG_printf(&trace, "    Byte position:%d   Hex val:%x", i, chardata[i]);
            //}

            //UART_Write( chardata, num, UART_WAIT );   // Echo data to UART port. Must wait because not waiting screws up the UART_Read above.

            for( i = 0; i< num; i++ ) {
                datachr = datastr[ i ];     // Loop for each character receved

                //------------------------------------------------      
                // Sequentially search input stream for "Range=".
                // When found, grab number, convert to float
                // and update the associated global variables.
                //------------------------------------------------      

                switch( Rangeidx ) {

                    case 0 :
                        if( datachr == *"R" )   // I can use 'R' instead.
                            Rangeidx++;
                        break;

                    case 1 :
                        if( datachr == *"H" )
                            Rangeidx++;
                        else
                            Rangeidx = 0;
                        break;

                    case 2 :
                        if( datachr == *"=" )
                            Rangeidx++;
                        else
                            Rangeidx = 0;
                        break;

                    default :   // received the "Range=" OK
                        if( Rangestridx < 10 ) {                                    // Look for up to 10 characters after the "="
                            if( ( Rangestr[ Rangestridx++ ] = datachr ) == *"%" ) { // We now have the number string terminated with the "%"
                                Rangestridx = 0;
                                Rangeidx = 0;
                                //RangeSample = (float) strtod( (char *) Rangestr, &ptr);
                                RangeSample = my_atof( (char *) Rangestr);

                                // Validate Range measurement, perform median filter on sequece
                                // and assign the global variables:

                                if ( ( RangeSample >= 0.0)  && ( RangeSample <= 100.0 ) ) {
                                    Rangemsec = (int) PRD_getticks();   // Flag the sample as valid with the current time (in msec)
                                    Range = my_median5( &RangeSample, RangeSampleHist );

                                    if( Rangecount < 5 )    // Initially wait for 5 updates (to clear history) before validating
                                        Rangecount++;
                                    else
                                        RangeOK = 1;
                                }
// LOG_printf(&trace, "RangeOK = %d ", RangeOK );
// LOG_printf(&trace, "Range * 10 = %d ", (int) (10.0*Range) );

                            }
                        }
                        break;

                } // switch( Rangeidx )     

            } // for( i = 0; i< num; i++ )

        }  // if (num)


        //----------------------------------------------------------------------------
        // Flag any measurements as invalid if not updated in a timely mannor.
        //----------------------------------------------------------------------------
        i = (int) PRD_getticks();

        if( Rangemsec > i )             // if PRD_geticks() rolled over, reset time to avoid 
            Rangemsec = i;          
        if( ( i - Rangemsec ) > 5000 ) {    // Flag global Range invalid if no updates for more than 5 sec.
            RangeOK = 0;
            Rangecount = 0;
// LOG_printf(&trace, "RangeOK = %d ", RangeOK );

        }
        
        
        //--------------------------------------------------------------------------------------
        // Sleep here for 10 msec (9600 baud yields 9600/9 = 1.067 characters per msec)
        // Total poling loop time should be kept well below the 15 msec (9/9600*16); to
        // avoid overflowing the 16 character UART buffer.
        //--------------------------------------------------------------------------------------
        TSK_sleep (10);


        UART_EnableLed1( 0 );


    } // while(1)


}

