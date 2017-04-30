
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
 * File: pollTemperatureHumidityTask.c
 *
 *  This functions monitors the MIDI port for input from the Rense (www.rense.nl)
 *  Model HT-923 Temperature/Hunidity Probe. A modified DB-9 male connector is required
 *  on the output of the probe to adapt it to the op-isolator input.
 *
 *  After the task is called initally to set up some parameters, it loops - polling the
 *  MIDI port for the input stream. When a valid reading is made it is filtered with a
 *  length-5 median filter to eliminate out-liers. Finally, the following global
 *  variables are updated about twice per second:
 *
 *      float   RelativeHumidity        in %, 1 to 95% calibrated, can read from 0.0 to 100%.
 *      int     RelativeHumidityOK      0-reading invalid, 1-reading valid (readings are flaged invalid if not updated for 5 seconds)
 *
 *      float   Temperature             in deg. C, -20 to 100 C calibrated, can posibly read -100 to 200 C
 *      int     TemperatureOK           0-reading invalid, 1-reading valid (readings are flaged invalid if not updated for 5 seconds)
 *
 *      float   TemperatureDew          in deg. C, This is the temperature of an object that will condense moisture
 *      int     TemperatureDewOK        0-reading invalid, 1-reading valid (readings are flaged invalid if not updated for 5 seconds)
 *
 * 
 *  M. Spencer
 *
 */

// PADK Library
#include "PADK.h"

#include <main.h>


#define TEMP_HUMIDITY_SENSOR_BUAD_RATE  9600
#define INVALID_AFTER   5000                        //  readings are flaged invalid if not updated for 5000 msec

// For PADK MIDI Module:
MIDI_Params midiParams = MIDI_DEFAULT_PARAMS;
#define MIDI_WAIT       1
#define MIDI_NO_WAIT    0

// For trace LOG buffer used to print runtime information:
extern far LOG_Obj trace;


//--------------------------------------------------------
// Declare global parameters here
//--------------------------------------------------------
float RelativeHumidity;
int RelativeHumidityOK = 0;
float RHSampleHist[ 4 ];

float Temperature;
int TemperatureOK = 0;
float TSampleHist[ 4 ];

float TemperatureDew;
int TemperatureDewOK = 0;
float TDEWSampleHist[ 4 ];


void pollTemperatureHumidityTask ()
{
    unsigned char datachr, datastr[16];
    int i, num;

    float RHSample;
    unsigned char RHstr[10];
    int RHmsec = 0;
    int RHidx = 0;
    int RHstridx = 0;
    int RHcount = 0;
    int RHchrCount;

    float TSample;
    unsigned char   Tstr[10];
    int Tmsec = 0;
    int Tidx = 0;
    int Tstridx = 0;
    int Tcount = 0;
    int TchrCount;

    float TDEWSample;
    unsigned char TDEWstr[10];
    int TDEWmsec = 0;
    int TDEWidx = 0;
    int TDEWstridx = 0;
    int TDEWcount = 0;
    int TDEWchrCount;



 LOG_printf(&trace, "Entered pollTemperatureHumidityTask.c");


    //--------------------------------------------------------
    // Initialize the MIDI module modifing default values
    //--------------------------------------------------------

    midiParams.baud = TEMP_HUMIDITY_SENSOR_BUAD_RATE;
    MIDI_Init( &midiParams );

    while(1) {

        //------------------------------------------------------------------------------------------
        // Read up to 16 characters from the UARTs 16 Byte FIFO. If any more than
        // 16 characters were sent before this loop repeats then characters will be lost.
        //
        // Typical input string examples are:
        //
        //                  "RH= 34.0%,T= 25.9C,Tdew=  8.5C,AbsH=  8.0gr/m3  ", CR(0x0d), LF(0x0a)
        //                  "RH= 51.1%,T= 28.3C,Tdew= 14.7C,AbsH= 12.0gr/m3  ", CR(0x0d), LF(0x0a)
        //                  "RH= 70.6%,T=  3.6C,Tdew=- 1.0C,AbsH=  4.3gr/m3  ", CR(0x0d), LF(0x0a)
        //   Position:       012345678901234567890123456789012345678901234567   48        49
        //
        //------------------------------------------------------------------------------------------

        num = MIDI_Read( datastr, 16, MIDI_NO_WAIT );
        
        if (num) {              // One or more characters have been read.

            MIDI_EnableLed1( 1 );   // flash LED when something received (this is a very fast routine)


// LOG_printf(&trace, "Number of MIDI Bytes read = %d", num);

//for (i = 0; i < num; i++) {
//  LOG_printf(&trace, "    Byte position:%d   Hex val:%x", i, chardata[i]);
//}

//UART_Write( chardata, num, MIDI_WAIT );   // Echo data to UART port. Must wait because not waiting screws up the MIDI_Read above.

            for( i = 0; i< num; i++ ) {
                datachr = datastr[ i ];     // Loop for each character receved

                //------------------------------------------------      
                // Sequentially search input stream for "RH=".
                // When found, grab number, convert to float
                // and update the associated global variables.
                //------------------------------------------------      

                switch( RHidx ) {

                    case 0 :
                        if( datachr == 'R' ) {  // find first char of interest
                            RHidx = 1;
                            RHchrCount = 1;
                        }
                        break;

                    case 1 :
                        if( ( datachr == 'H' ) && ( RHchrCount == 1 ) ) {   // verify second char of interest followed first
                            RHidx = 2;
                            RHchrCount = 2;
                        }
                        else
                            RHidx = 0;
                        break;

                    case 2 :
                        if( ( datachr == '=' ) && ( RHchrCount == 2 ) ) {   // and so on...
                            RHidx = 3;
                            RHchrCount = 3;
                        }
                        else
                            RHidx = 0;
                        break;

                    default :   // received the "RH=" OK
                        if( ( RHstridx < 10 ) && ( RHchrCount == 3 ) ) {        // Look for up to 10 characters after the "="
                            if( ( RHstr[ RHstridx++ ] = datachr ) == '%' ) {    // We now have the number string terminated with the "%"
                                RHstridx = 0;
                                RHidx = 0;
                                RHSample = my_atof( (char *) RHstr);    // Convert characters to float

                                RHmsec = (int) PRD_getticks();  // Flag the sample as received using the current time (in msec)

                                // Clip out-of-bound values, perform median filter on sequece
                                // and assign the global variables:

                                RHSample = ( RHSample >= 0.0 ) ? RHSample : 0.0;
                                RHSample = ( RHSample <= 100.0 ) ? RHSample : 100.0;
                                RelativeHumidity = my_median5( &RHSample, RHSampleHist );   // 5-sample median filter

                                if( RHcount < 4 )   // Initially wait for 4 updates (to clear history) before validating
                                    RHcount++;
                                else
                                    RelativeHumidityOK = 1;

//LOG_printf(&trace, "RelativeHumidity * 10 = %d ", (int) (10.0*RelativeHumidity) );

                            }

                        }
                        else {
                            RHidx = 0;
                            RHstridx = 0;
                        }
                        break;

                } // switch( RHidx )        



                //------------------------------------------------      
                // Sequentially search input stream for "T=".
                // When found, grab number, convert to float
                // and update the associated global variables.
                //------------------------------------------------      

                switch( Tidx ) {

                    case 0 :
                        if( datachr == 'T' ) {  // find first char of interest
                            Tidx = 1;
                            TchrCount = 1;
                        }
                        break;

                    case 1 :
                        if( ( datachr == '=' ) && ( TchrCount == 1 ) ) {    // verify second char of interest followed first
                            Tidx = 2;
                            TchrCount = 2;
                        }
                        else
                            Tidx = 0;
                        break;

                    default :   // received the "T=" OK
                        if( ( Tstridx < 10 )  && ( TchrCount == 2 ) ) {         // Look for up to 10 characters after the "="
                            if( ( Tstr[ Tstridx++ ] = datachr ) == 'C' ) {      // We now have the number string terminated with the "C"
                                Tstridx = 0;
                                Tidx = 0;
                                TSample = my_atof( (char *) Tstr );     // Convert characters to float

                                Tmsec = (int) PRD_getticks();   // Flag the sample as received using the current time (in msec)

                                // Clip out-of-bound values, perform median filter on sequece
                                // and assign the global variables:

                                TSample = ( TSample >= -100.0 ) ? TSample : -100.0;
                                TSample = ( TSample <= 200.0 ) ? TSample : 200.0;
                                Temperature = my_median5( &TSample, TSampleHist );  // 5-sample median filter

                                if( Tcount < 4 )    // Initially wait for 4 updates (to clear history) before validating
                                    Tcount++;
                                else
                                    TemperatureOK = 1;

//LOG_printf(&trace, "Temperature * 10 = %d ", (int) (10.0*Temperature) );

                            }
                        }
                        else {
                            Tidx = 0;
                            Tstridx = 0;
                        }
                        break;

                } // switch( Tidx )     



                //------------------------------------------------      
                // Sequentially search input stream for "Tdew=".
                // When found, grab number, convert to float
                // and update the associated global variables.
                //------------------------------------------------      

                switch( TDEWidx ) {

                    case 0 :
                        if( datachr == 'T' ) {  // find first char of interest
                            TDEWidx = 1;
                            TDEWchrCount = 1;
                        }
                        break;

                    case 1 :
                        if( ( datachr == 'd' ) && ( TDEWchrCount == 1 ) ) { // verify second char of interest followed first
                            TDEWidx = 2;
                            TDEWchrCount = 2;
                        }
                        else
                            TDEWidx = 0;
                        break;

                    case 2 :
                        if( ( datachr == 'e' ) && ( TDEWchrCount == 2 ) ) { // and so on...
                            TDEWidx = 3;
                            TDEWchrCount = 3;
                        }
                        else
                            TDEWidx = 0;
                        break;

                    case 3 :
                        if( ( datachr == 'w' ) && ( TDEWchrCount == 3 ) ) { // and so on...
                            TDEWidx = 4;
                            TDEWchrCount = 4;
                        }
                        else
                            TDEWidx = 0;
                        break;

                    case 4 :
                        if( ( datachr == '=' ) && ( TDEWchrCount == 4 ) ) { // and so on...
                            TDEWidx = 5;
                            TDEWchrCount = 5;
                        }
                        else
                            TDEWidx = 0;
                        break;

                    default :   // received the "Tdew=" OK
                        if( ( TDEWstridx < 10 ) && ( TDEWchrCount == 5 ) ) {            // Look for up to 10 characters after the "="
                            if( ( TDEWstr[ TDEWstridx++ ] = datachr ) == 'C' ) {    // We now have the number string terminated with the "C"
                                TDEWstridx = 0;
                                TDEWidx = 0;
                                TDEWSample = my_atof( (char *) TDEWstr);    // Convert characters to float

                                TDEWmsec = (int) PRD_getticks();    // Flag the sample as received using the current time (in msec)

                                // Clip out-of-bound values, perform median filter on sequece
                                // and assign the global variables:

                                TDEWSample = ( TDEWSample >= -100.0 ) ? TDEWSample : -100.0;
                                TDEWSample = ( TDEWSample <= 200.0 ) ? TDEWSample : 200.0;
                                TemperatureDew = my_median5( &TDEWSample, TDEWSampleHist ); // 5-sample median filter

                                if( TDEWcount < 4 ) // Initially wait for 4 updates (to clear history) before validating
                                    TDEWcount++;
                                else
                                    TemperatureDewOK = 1;

//LOG_printf(&trace, "TemperatureDew * 10 = %d \n", (int) (10.0*TemperatureDew) );

                            }
                        }
                        else {
                            TDEWidx = 0;
                            TDEWstridx = 0;
                        }
                        break;

                } // switch( TDEWidx )      





            } // for( i = 0; i< num; i++ )

        }  // if (num)


        //----------------------------------------------------------------------------
        // Flag any measurements as invalid if not updated in a timely mannor.
        //----------------------------------------------------------------------------
        i = (int) PRD_getticks();

        // RelativeHumidity:
        if( RHmsec > i )                        // if PRD_geticks() rolled over, reset time to avoid trouble
            RHmsec = i;         
        if( ( i - RHmsec ) > INVALID_AFTER ) {  // Flag global RelativeHumidity invalid if no updates for more than 5 sec.
            RelativeHumidityOK = 0;
            RHcount = 0;
// LOG_printf(&trace, "RelativeHumidityOK             = %d ", RelativeHumidityOK );
        }

        // Teperature:
        if( Tmsec > i )                         // if PRD_geticks() rolled over, reset time to avoid trouble
            Tmsec = i;          
        if( ( i - Tmsec ) > INVALID_AFTER ) {   // Flag global Temperature invalid if no updates for more than 5 sec.
            TemperatureOK = 0;
            Tcount = 0;
// LOG_printf(&trace, "TemperatureOK                     = %d ", TemperatureOK );
        }

        // TeperatureDew:
        if( TDEWmsec > i )                          // if PRD_geticks() rolled over, reset time to avoid trouble
            TDEWmsec = i;           
        if( ( i - TDEWmsec ) > INVALID_AFTER ) {    // Flag global TemperatureDew invalid if no updates for more than 5 sec.
            TemperatureDewOK = 0;
            TDEWcount = 0;
// LOG_printf(&trace, "TemperatureDewOK                   = %d ", TemperatureDewOK );
        }

        
        
        //--------------------------------------------------------------------------------------
        // Sleep here for 10 msec (9600 baud yields 9600/9 = 1.067 characters per msec)
        // Total poling loop time should be kept well below the 15 msec (9/9600*16); to
        // avoid overflowing the 16 character UART buffer.
        //--------------------------------------------------------------------------------------
        TSK_sleep (10);


        MIDI_EnableLed1( 0 );


    } // while(1)


}




