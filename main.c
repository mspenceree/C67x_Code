
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
 * File: main.c
 *
 * 
 * 
 * The staring point of the program was the Lyrtech "echo" demo program that demonstrates usage of
 * the McASP drivers that take care of the frame buffers for audio processing.
 *
 * This program project implements the ASACS signal processing functions as commanded by the PC
 * via the USB port.
 * 
 * 
 * Michael Spencer
 * 
 * Revision History:
 *   V0.01 - Slightly modified "echo" demo program. Added the switchesTask.c that reads
 *           the switch states (digital inputs) from ASACS.
 *
 *   V0.02 - Added controlTask.c that reads the commands from the USB port. Added HFP algorithm test
 *           code. Tested RS-232 communications using poling. It's not too CPU intensive if I "sleep"
 *           it for 10msec per loop.
 *
 *   V0.03 - I tested interrupt driven RS-232 communications by bringing in sample code form
 *           uartloopback. It worked but it disabled DSP/BIOS functionality. I didn't get it working
 *           properly. Abandon this version for now... (this version started with an intermediate
 *           version between V0.01 and V0.02)
 *
 *   V0.04 - Started with V0.02 code. Completed the pollTemperatureHumidityTask.c code.
 *
 *   V0.05 - Completed the pollAuxAnalogInTask.c code.
 *
 *   V0.06 - Renamed project to "asacs.pjt" (by starting a new project). Re-arranged directories,
 *           updated compiler include options and modified the command file. (*** Warning: These
 *           dir changes break the previous versions ***).
 *           Stopped using pre-compiled version of PADK Library api. 
 *           I needed to change the voltage reference setting in "padk_conv.c" for the 8-bit A/D to
 *           get 0-5V input range. Plus I think there are errors in "padk_uart" and "padk_midi" for
 *           "wait". I now refer to local copies of the source code. I will comment the files with
 *           any changes I make from the original libraries. Changed: padk_i2c.c lib to keep from
 *           waiting infinitely if interface board not plug in.
 *           Finished: pollSwitchesTask.c
 *           Finished: digitalOutByte() and digitalOutBit() functions.
 *
 *   V0.07 - Dropped poll task header files and moved to main.h. Dropped misc_funcs.c file and moved
 *           functions to end of main.c.
 *           Added initInterfaceBoard() function.
 *           Changed i2c.c lib to avoid the use of reading ticks for timeouts.
 *
 *   V0.08 - Finished up initialization routines. Renamed and reorganized files and functions.
 *           Dropped the "echo" term from the dam project.
 *
 *   V0.09 - Added first DSP Algorithm: "IIR_Filter". Added algCreateBlocks.c code.
 *           Added zzzBackgroundLoopIdle.c code.
 *
 *   V0.10 - Incorporated processing loop. Added GAIN block. Added fade to/from zero code to
 *           to avoid audio "pops". Added "filterDexign.c", Completed processing loops., Updated to
 *           use generalized parameter specifications with DSP algorithms. Updated Gain and IIR.
 *           Many more additions...
 *
 *   V0.11 - Now in WA. Added echo code to pollUsbTask.c for Tim's testing.
 *           Added FIR filter block and tested.
 *
 *   V1.0  - Now at production code to be tested in fully configured ASACS system. Add version
 *           number variable to flash LED with version #: eg. V2.3 Flashes: * *   * * *      repeat.
 *
 *   V1.1  - Fixed the power sequencing problem by periodically writing the mux. values. Removed
 *           the "STANDALONE" Mode and made the software always act as master when the Mic PTT
 *           is pressed (see comments in the pollSwitchesTAsk.c). Bumped up the priority of
 *           pollSwitchesTAsk.c above that of pollUsbTask.c to give fast responsiveness. I put most
 *           of the tasks at priority 7.
 *
 *   V1.2  - Added dynamic range compressor (DRC) header info. This version has USB test code that
 *           conforms to V1.1 of the spec. and ignores the header and echos the data (byte for byte).
 *
 *   V1.3  - Put USB comm. interface and command interpreter into final form. Reconfigured all
 *           DSP handles to use arrays (e.g. GAIN_Handles[0]->gain = 1.0 )
 *
 *   V1.4  - Modified test code to output header and data all together.
 *
 *   V1.5  - Added CONTROLLER, GAIN, DRC, SECLECTOR, DIGITAL_IN, TEMPERATURE, TEMPERATURE_DEW
 *           HUMIDITY, RANGE and ADC to command interpreter.
 *           To make room for new memory buffers I put .text sections into SDRAM (minor speed hit).
 *
 *
 *
 *
 **************************************************************************************************
 * Some program conventions:
 *
 * Global Variables:
 *  Naming: First letter is always capital.
 *  Define at beginning of *.c file where variable is first used. e.g:
 *
 *      int Global_Var = 123;
 *
 *  Reference variable with "extern" keyword at beginning of every *.c file
 *  that uses the variable. E.g:
 *
 *      extern int Global_Var;
 *
 *
 * Global Constants:
 *  Naming: All capital letters
 *  Define in the header file *.h related to the *.c file where first used. e.g
 *
 *      #define IIR_MAX_FILTER_ORDER 20
 *
 *  If a reference is required in a different c-file, add the #include <*.h> to that file.
 *
 *
 */

#include <main.h>
#include <algProcessingTask.h>


// trace LOG buffer is used to print runtime information:
extern far LOG_Obj trace;


//////////////////////////////////////////////////////////
// Define global variables and parameters here
// (if not defined in the pollxxxTask.c files)
//////////////////////////////////////////////////////////
int swVersion = 1;      // Software version int
int swVersionFrac = 5;  // Software version frac. eg. Ver int.frac

unsigned char DSP_Flags;

extern int AsacsInputSelect;
extern int AsacsInputSelectTemp;

extern int AsacsNoiseSourceSelect;
extern int PCLineInLSelect;
extern int PCLineInRSelect;
extern int AuxOut1Select;
extern int AuxOut2Select;

extern int TalkOpStatus;
extern int ListenOpStatus;
extern int MonitorOpStatus;
extern int TestOpStatus;

extern int DSP_Mode;
extern int DSP_ModeTemp;

// Global history variables here:
unsigned char DigitalOut_Copy;
unsigned char AnalogMuxAddrA_Copy;
unsigned char AnalogMuxAddrB_Copy;
unsigned char AnalogMuxAddrC_Copy;


float FadeWindow[FRAMESIZE];


void main(void)
{

    LOG_printf(&trace, "Entered main.c");

    // Mute the DAC outputs while starting up:
    //DAC_Mute(); // this "pops" the output off

    // initialize the Interface Board (if currently powered)
    DSP_Flags = 0;
    if( initInterfaceBoard() )
        DSP_Flags |= DSP_FLAGS_INTERFACE_BOARD_ERROR_BIT;   // Flag error if board not connected or powered,
                                                                // and continue...
    //

    // Create DSP Blocks, load power-up default parameters and compute dependent parameters
    algCreateBlocks();
    
    // initialize remaining global parameters and variables (not initialized in algCreateBlocks() )
    initGlobalVars();

    // Call the CSL Init functions.
    // All these functions always return 'CSL_SOK'
    CSL_chipInit(NULL);     // Initialization function for the CHIP CSL
    CSL_dmaxInit(NULL);     // Initialization function for the DMAX CSL
    CSL_mcaspInit(NULL);    // Initialization function for the McASP CSL

} // main()



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Miscallanious functions that are called throughout the code are here...
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// my_compare_func
//
// Used by qsort to sort 1-D arrays
//
int my_compare_func( const void *ptr1, const void *ptr2 )
{
    return ( *(int*)ptr1 - *(int*)ptr2 );
}



//
// my_median
//
// Performs a length 5 median filter
//
float my_median5( float *inSample, float *history )
{
    float sortArray[5];

    sortArray[ 4 ] = history[ 3 ];  // load the sort array
    sortArray[ 3 ] = history[ 2 ];
    sortArray[ 2 ] = history[ 1 ];
    sortArray[ 1 ] = history[ 0 ];
    sortArray[ 0 ] = *inSample;

    history[ 3 ] = history[ 2 ];    // shift the history
    history[ 2 ] = history[ 1 ];
    history[ 1 ] = history[ 0 ];
    history[ 0 ] = *inSample;

    qsort( (void *) sortArray, 5, sizeof(float), my_compare_func ); // sort the array of five samples

    return( sortArray[ 2 ] );   // return the median

}


//
// my_atof
//
// Converts a string s[] to a float.
// Unlike the standard library atof, this one allows spaces before or after any sign characters.
//
float my_atof( char s[])
{
    float sign, val, power;
//  float temp;
    int i;

    for( i = 0; isspace(s[i]); i++) // skip white space
        ;
    sign = (s[i] == '-') ? -1.0 : 1.0;
    if( s[i] == '+' || s[i] == '-')
        i++;
    for(    ; isspace(s[i]); i++)   // skip white space
        ;
    for( val = 0.0; isdigit(s[i]); i++)
        val = 10.0*val + (s[i] - '0');
    if ( s[i] == '.')
        i++;
    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0*val + (s[i] - '0');
        power *= 10.0;
    }
//temp = sign*val;
//temp = 12.34e0; //temp/power;
//return temp;
    return ( sign*val/power );
}


// This is the error handler for any call to any I2C_WriteVal or I2C_ReadVal calls
// It forces a program restart if I2C can't be setup.
// It sets PC_STATUS_INTERFACE_ERROR_BIT in the PC_Status_Byte and writes a message
// to the trace window if the Interface Board is not powered on. This bit is cleared
// if I2C operation was successfull.
//

int i2c_error( int I2C_result )
{

    if( ( I2C_result == -1 ) )
    {
        LOG_printf( &trace, "I2C (setup?) operation failed. Exiting program.");
        exit (SYS_EALLOC);      // fixthis. For all malacs I should report error to PC and restart DSP
    }

    if( ( I2C_result < -1 ) )
    {
        LOG_printf( &trace, "Error: Interface Board not Powered. I2C_WriteVal/I2C_ReadVal= %d", I2C_result );
        DSP_Flags |= DSP_FLAGS_INTERFACE_BOARD_ERROR_BIT;
        return( -1 );
    }
    else
    {
        DSP_Flags &= ~( (int) DSP_FLAGS_INTERFACE_BOARD_ERROR_BIT );
        return( 0 );
    }

}


//
// digitalOutByte()
//
// Outputs a byte to the digital output port on the Interface Board (at addr: 0x20) and
// updates the global variable: DigitalOut_Copy.
//
// It must be called at least once before calling digitalOutBit().
//
// Returns 0 if OK, -1 if Interface Board is not powered up.
//
int digitalOutByte( unsigned char byteVal )
{
    return ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_OUT, DigitalOut_Copy = byteVal ) ) );
}


//
// digitalOutBit()
//
// Outputs a 0 or 1 (if bitVal != 0) to the specified digital output bit(s)
// on the Interface Board (at addr: 0x20).
//
// At least one call to digitalOutByte() must be made before calling this function.
//
// Several masks may be specified simultainiously. eg.
//  result = digitalOutBit( 0, INPUT_OK_LED_BIT | DSP_OK_LED_BIT );
//
// Returns 0 if OK, -1 if Interface Board is not powered up.
//
int digitalOutBit( int bitVal, unsigned char mask )
{
    if ( bitVal )
        return ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_OUT, DigitalOut_Copy |= mask ) ) );
    else
        return ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_OUT, DigitalOut_Copy &= ~mask ) ) );
}


//
// analogMuxHeadphoneSelect()
//
// Select the headphone signal source by writing the A[67] address lines
// on the ASACS Interface board. (see the "Interface Board: Analog Switch
// Matrix" spreadsheet)
//
// The address parameter: addr67 selects the headphone source as follows:
//
//  addrA67 = 0 -   Monitor PCLineIn (this is DSPOut6 and 7)
//  addrA67 = 1 -   Monitor AuxOut (this is DSPOut4 and 5)
//  addrA67 = 2 -   Monitor PCLineOut (Listen to your MP3 music anybody?)
//  addrA67 = 3 -   Monitor CompressorOut signal on Interface Board
//
// Returns 0 if OK, -1 if Interface Board is not powered up.
//
int analogMuxHeadphoneSelect(int addrA67)
{
    // Write the headphone select bits
    return( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_A, 
            AnalogMuxAddrA_Copy = ( AnalogMuxAddrA_Copy & 0x3F ) | ( (addrA67 & 0x03 ) << 5) ) ) );
}


//
// analogMuxDSPIn78Select()
//
// Select the signal source for the DSPIn7 and DSPIn8 analog inputs
// by writing the same values to the C[0123] and C[4567] address lines
// on the ASACS Interface board. (see the "Interface Board: Analog Switch
// Matrix" spreadsheet)
//
//  addrC0123 = 0 to 15 - see spreadsheet
//
// Returns 0 if OK, -1 if Interface Board is not powered up.
//
int analogMuxDSPIn78Select(int addrC0123)
{
    // Write the DSPIn7 and 8 select bits
    addrC0123 &= 0x0F;  // mask if input out of range
    return( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_C, AnalogMuxAddrC_Copy =  ( addrC0123<<4 ) | addrC0123  ) ) );
}


//
// initInterfaceBoard()
//
// Initializes the ASACS Interface Board with default register values.
//
// Returns 0 if OK, -1 if Interface Board is not powered up.
//
int initInterfaceBoard()
{
    // Write the default analog mux addresses (see the "Interface Board: Analog Switch Matrix" spreadsheet)
    if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_A, AnalogMuxAddrA_Copy = 0x00 ) ) )
        return(-1);
    if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_B, AnalogMuxAddrB_Copy = 0x3B ) ) )
        return(-1);
    if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_C, AnalogMuxAddrC_Copy = 0x44 ) ) )
        return(-1);

    // Write the default LED and digital control bits to the Interface Board
    //
    // The digitalOutByte() write assigns the bits as follows:
    //
    //      INPUT_OK_LED_BIT            = 0
    //      DSP_OK_LED_BIT              = 1
    //      TRANCEDUCER_AMP_OK_LED_BIT  = 0
    //      SPARE1_LED_BIT              = 0
    //      SPARE2_LED_BIT              = 0
    //
    //      AMPSHUTDOWN_F_BIT           = 1 ( amp is operating )
    //      DSP_PUSHTOTALK_F_BIT        = 1 ( talk relays not engaged )
    //      DSP_READY_F_BIT             = 0 ( DSP is ready )
    //
    return( digitalOutByte( DSP_OK_LED_BIT | AMPSHUTDOWN_F_BIT | DSP_PUSHTOTALK_F_BIT ) );
}


//
// initGlobalVars()
//
// Initializes all global parameters as defined in the PC to DSP Interface
//
//
// Returns 0 if OK, -1 if ...
//
void initGlobalVars()
{
    float aa, bb;
    int i;

    // Init Global algorithm parameters:

    // Load up the "fade to zero" window (endpoints not included):

//  aa = ( FADE_WINDOW_STOP - FADE_WINDOW_START )/FRAMESIZE_P1;
//  bb = FADE_WINDOW_START + aa;
//  for( i = 0; i < FRAMESIZE; i++)
//      FadeWindow[i] = powf( 10.0, ( aa*i + bb )/20.0 );   // load up the log fade

    // or do linear fade:
    aa = -1.0/FRAMESIZE_P1;
    bb = 1.0 + aa;
    for( i = 0; i < FRAMESIZE; i++)
        FadeWindow[i] = aa*i + bb;  // load up linear fade


//  AsacsInputSelect = 1;           // default to ASACS_Mic input
    AsacsInputSelect = 8;           // default to PCLineOutSum input
    AsacsInputSelectTemp = AsacsInputSelect;

    AsacsNoiseSourceSelect = 2;     // default to ASACS_NoiseMic input
    PCLineInLSelect = 1;            // default PC Line In to monitor talk/listen
    PCLineInRSelect = 1;
    AuxOut1Select = 1;              // default ASACS_AuxOut (headphones) to monitor talk/listen
    AuxOut2Select = 1;

    TalkOpStatus = DSP_OP_STOPPED;      // Turn off all DSP operations
    ListenOpStatus = DSP_OP_STOPPED;
    MonitorOpStatus = DSP_OP_STOPPED;
    TestOpStatus = DSP_OP_STOPPED;


    DSP_Mode = UNDEFINED;
    ChangeDSP_Mode( LISTEN );  // May want to change to STANDBY until we get $$$$ for this.  Set default DSP_Mode to LISTEN
    DSP_ModeTemp = DSP_Mode;

    return;
}


void ChangeDSP_Mode( int newDSP_Mode )
{

    if( DSP_Mode == newDSP_Mode )
        return;         // return, if no mode change necessary

    DSP_Mode = newDSP_Mode;
    switch( newDSP_Mode )
    {
        case STANDBY:   // Perform a controlled stop of all DSP Operations
            StopOp( &TestOpStatus, 0 );     // Stop Test operations (don't need conformation)
            StopOp( &TalkOpStatus, 0 );     // Stop Talk operations (don't need conformation)
            StopOp( &MonitorOpStatus, 0 );  // Stop Monitor operations (don't need conformation)
            StopOp( &ListenOpStatus, 0 );   // Stop Listen operations (don't need conformation)

            // Disengage PTT relay on the Interface Board (if not):
            digitalOutBit( 1, DSP_PUSHTOTALK_F_BIT );
            break;

        case LISTEN:    // Put the DSP into Listen mode
            StopOp( &TestOpStatus, 1 );     // Stop Test operations and confirm
            StopOp( &TalkOpStatus, 1 );     // Stop Talk operations and confirm

            StartOp( &ListenOpStatus, 0 );  // Start Listen operations (don't need conformation)
            StartOp( &MonitorOpStatus, 0 ); // Start Monitor operations (don't need conformation)

            // Disengage PTT relay on the Interface Board (if not):
            digitalOutBit( 1, DSP_PUSHTOTALK_F_BIT );
            break;

        case TALK:      // Put the DSP into Talk mode
            StopOp( &TestOpStatus, 1 );     // Stop Test operations and confirm
            StopOp( &ListenOpStatus, 1 );   // Stop Listen operations and confirm

            StartOp( &TalkOpStatus, 0 );    // Start Talk operations (don't need conformation)
            StartOp( &MonitorOpStatus, 0 ); // Start Monitor operations (don't need conformation)

            // Engage PTT relay on the Interface Board (if not):
            digitalOutBit( 0, DSP_PUSHTOTALK_F_BIT );
            break;

        case TEST:      // Put the DSP into Test mode
            StopOp( &TalkOpStatus, 1 );     // Stop Talk operations and confirm
            StopOp( &ListenOpStatus, 1 );   // Stop Listen operations and confirm
            StopOp( &MonitorOpStatus, 1 );  // Stop Monitor operations and confirm

            StartOp( &TestOpStatus, 0 );    // Start Test operations (don't need conformation)

            // Engage PTT relay on the Interface Board (if not):
            digitalOutBit( 0, DSP_PUSHTOTALK_F_BIT );
            break;
    }

    return;
}


// Function to stop DSP operation with or without waiting (blocking) to confirm
int StopOp( int *OpStatus, int Confirm )
{
    int result = 0;
    
    HWI_disable();  // Avoid interrupt during testing that could change OpStatus
                    // between initial and subsequent tests.

    // Stop operations if required:
    if( *OpStatus >= DSP_OP_RUNNING )
    {
        if ( *OpStatus == DSP_OP_START )    // If operation was just started...
            *OpStatus = DSP_OP_STOPPED;     // don't let it start
        else
            *OpStatus = DSP_OP_STOP;        // otherwise, intiate a stop

        HWI_enable();   // Restore ints.

        if( Confirm )
        {
            result = 1000000;   // for up to a milion loops:
            while( ( *OpStatus != DSP_OP_STOPPED ) && ( result-- > 0 ) );   // wait for operations to stop
        }
    }
    else
    {
        HWI_enable();   // Restore ints.
    }

    if( result == -1 )
    {
        LOG_printf (&trace, "Algorithm shutdown failed in StopOp(). Exiting program."); // fixthis. For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }

    return( result );   // returns: -1 if operation timmed out (failed).
                        //             This could only happen if the DSP lost it's processing loop ints. Should restart.
                        //           0 if operation was already stopped
                        //    positive if shutdown successful
}


// Function to start DSP operation with or without waiting (blocking) to confirm
int StartOp( int *OpStatus, int Confirm )
{
    int result = 0;
    
    HWI_disable();  // Avoid interrupt during testing that could change OpStatus
                    // between initial and subsequent tests.

    // Start operations if required:
    if( *OpStatus < DSP_OP_RUNNING )    // If operation is stoped or stopping...
    {
        *OpStatus = DSP_OP_START;       // start it.

        HWI_enable();   // Restore ints.

        if( Confirm )
        {
            result = 1000000;   // for up to a milion loops:
            while( ( *OpStatus != DSP_OP_RUNNING ) && ( result-- > 0 ) );   // wait for operations to
                                                                            // transition to running normally.
        }
    }
    else
    {
        HWI_enable();   // Restore ints.
    }

    if( result == -1 )
    {
        LOG_printf (&trace, "Algorithm startup failed in StartOp(). Exiting program."); // For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }

    return( result );   // returns: -1 if operation timmed out (failed)
                        //             This could only happen if the DSP lost it's processing loop ints. Should restart.
                        //           0 if operation was already started
                        //    positive if startup successful
}


