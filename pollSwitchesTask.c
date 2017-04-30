
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
 * File: pollSwitchesTask.c
 *

    This task poles the state of the ASACS digital inputs once every ~40 msec. ASACS switches
    are connected to these input and are used to update the integer: "SwitchState" with the
    bits as defined below.

    Parameters:
        unsigned int        SwitchState

    All switch inputs have pull-up resistors and the switches and buttons short the input to GND.

    SwithchState bits are assigned as follows:

     Note   Bit   Switch#   ASACS Function                          Connector   Associated Mask Constant
                                                                                (for bit-testing)

            bit 0 -  SW1:   Video On/Off, 0=Off                                 SW1_VIDEO_ON_OFF
            bit 1 -  SW2:   Spare                                       (J2)    SW2_SPARE
     (1)    bit 2 -  SW3:   DEFT On/Off, 0=Off                                  SW3_DEFT_ON_OFF
            bit 3 -  SW4:   Play Mode - Single, 0=Single                        SW3_PLAY_MODE_SINGLE
            bit 4 -  SW5:   Play Mode - Loop, 0=Loop                            SW4_PLAY_MODE_LOOP
            bit 5 -  SW6:   Play Mode - Library, 0=Library                      SW5_PLAY_MODE_LIBRARY
            bit 6 -  SW7:   Light Mode - Spot (not connected)                   SW6_LIGHT_MODE_SPOT
            bit 7 -  SW8:   Light Mode - Flood (not connected)                  SW7_LIGHT_MODE_FLOOD

            bit 8 -  SW9:   Light Mode - Strobe (not connected)                 SW9_LIGHT_MODE_STROBE
            bit 9 -  SW10:  Spare Switch                                        SW10_SPARE
            bit 10 - SW11:  Zoom In Button, 0=Pressed       (J5 Left Handle)    SW11_ZOOM_IN
            bit 11 - SW12:  Zoom Out Button, 0=Pressed      (J5 Left Handle)    SW12_ZOOM_OUT
            bit 12 - SW13:  File Select Button, 0=Pressed   (J6 Right Handle)   SW13_FILE_SELECT
            bit 13 - SW14:  Audio Trigger Button, 0=Pressed (J6 Right Handle)   SW14_AUDIO_TRIGGER
     (1)    bit 14 - SW15:  Volume Switch - Medium, 0=Medium            (J7)    SW15_VOLUME_MEDIUM
     (1)    bit 15 - SW16:  Volume Switch - High, 0=High                (J8)    SW16_VOLUME_HIGH

            bit 16 - SW17:  Spare                                       (J9)    SW17_SPARE
            bit 17 -        Unused
            bit 18 -        Unused
            bit 19 -        Unused
            bit 20 -        Unused
     (1)    bit 21 - SW18:  Audio On/Off, 0=Off                                 SW18_AUDIO_ON_OFF
     (1)    bit 22 - SW19:  KeyLock Switch, 0=Locked                    (J10)   SW19_UNLOCK_SWITCH
     (2)    bit 23 - SW20:  Push-to-Talk Button (on Mic), 0=Pressed     (J11)   SW20_PUSH_TO_TALK_F

            bits 24-31:     Unused


    Note 1: The DSP acts on these bits to control the specified audio function.

    Note 2: Pressing the PTT Button will switch the input Signal Selector to the Microphone input and
            place ASACS into the "TALK" mode. Releasing the PTT Button will return these variables
            to their previous state. The result is: if the PC is playing back a track when PPT is
            pressed then the audio will be temporarily interrupted. The PC may optionally pause the audio
            file during PTT (or stop and reset the track).

            Since the DSP is acting as a master in regards to some audio functions there are some
            rules that are enforced in the command interpreter (in pollUsbTask.c):

            1-  The PC can't take the DSP out of STANDBY if the Audio Enable switch is off and/or
                the KeyLock Switch is locked. These hardware switches have the "last say".
            2-  The PC can't changed the DSP_Mode while the Microphone Push-to-Talk is being pressed.



    M. Spencer


 **************************************************************************************************
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
unsigned int SwitchState = 0;
unsigned int SwitchStateOld = 0;


extern int DSP_Mode;
int DSP_ModeTemp;

extern int AsacsInputSelect;
int AsacsInputSelectTemp;

extern unsigned char DigitalOut_Copy;
extern unsigned char AnalogMuxAddrA_Copy;
extern unsigned char AnalogMuxAddrB_Copy;
extern unsigned char AnalogMuxAddrC_Copy;

extern unsigned char DSP_Flags;

extern pGAIN    GAIN_Handles[ GAIN_BLOCK_MAX_INSTANCE ];


void pollSwitchesTask ()
{

    unsigned char InPort0, InPort1, InPort2;

    int initializeOK = 0;
    int readOK;
    int SwitchStateChangeBits;

    float gDB;

    LOG_printf( &trace, "Entered pollSwitchesTask.c" );


    while(1)
    {
        if( !initializeOK )     // This section of code loops if board is not plugged in.
        {                       // Initialize succeeds if all registers are written without an error.
            initializeOK = 1;

            // This makes sure that all outputs are properly written incase board was not powered during initialization.
            if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_A, AnalogMuxAddrA_Copy ) ) ) {
                initializeOK = 0;
                TSK_sleep( 250 );
            }
            else if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_B, AnalogMuxAddrB_Copy ) ) ) {
                initializeOK = 0;
                TSK_sleep( 250 );
            }
            else if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_ADDR_C, AnalogMuxAddrC_Copy ) ) ) {
                initializeOK = 0;
                TSK_sleep( 250 );
            }

            else if ( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_OUT, DigitalOut_Copy ) ) ) {
                initializeOK = 0;
                TSK_sleep( 250 );
            }

            // Set output registers to all 1's so we can use them as inputs in the "Reads" below.
            else if( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_IN0, 0xFF ) ) ) {     
                initializeOK = 0;
                TSK_sleep( 250 );
            }
            else if( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_IN1, 0xFF ) ) ) {
                initializeOK = 0;
                TSK_sleep( 250 );
            }
            else if( i2c_error( I2C_WriteVal( I2C_ADDR_ASACS_BYTE_IN2, 0xFF ) ) ) {
                initializeOK = 0;
                TSK_sleep( 250 );
            }
        }

        if ( initializeOK ) // now read the switches...
        {
            readOK = 1;
            if( i2c_error( I2C_ReadVal(I2C_ADDR_ASACS_BYTE_IN0, &InPort0) ) ) {
                readOK = 0;
                TSK_sleep( 250 );
            }
            else if( i2c_error( I2C_ReadVal(I2C_ADDR_ASACS_BYTE_IN1, &InPort1) ) ) {
                readOK = 0;
                TSK_sleep( 250 );
            }
            else if( i2c_error( I2C_ReadVal(I2C_ADDR_ASACS_BYTE_IN2, &InPort2) ) ) {
                readOK = 0;
                TSK_sleep( 250 );
            }

            if ( readOK )
            {   
                // Update SwitchState:
                SwitchState = ( ( (unsigned int) InPort2 ) << 16 ) | ( ( (unsigned int) InPort1 ) << 8 ) | ( (unsigned int) InPort0 );
                SwitchStateChangeBits = SwitchState ^ SwitchStateOld;       // Local variable used to detect which switch changed
                SwitchStateOld = SwitchState;                               // Update history
                // At this point SwitchState and SwitchStateChangeBits have been updated (and may or may not have changed).

                if( SwitchStateChangeBits ) // if any bits changed...
                    DSP_Flags |= DSP_FLAGS_SWITCHES_CHANGED_BIT;    // flag the change to the PC


                // Now act on switch state for local audio function control:
                // Act on DEFT switch here:
                if ( SwitchStateChangeBits & SW3_DEFT_ON_OFF )      // if DEFT on/off switch changed then...
                {
                    if( SwitchState & SW3_DEFT_ON_OFF ) // DEFT switch turned "Off"
                    {
    //                      DEFT_H1->params.Bypass = 1;     // Bypass Talk DEFT alg.
    //                  DEFT_H2->params.Bypass = 1;     // Bypass Listen DEFT alg.
                    }
                    else
                    {
    // fixthis remove                       DEFT_H1->params.Bypass = 0;     // Don't Bypass Talk DEFT alg.
    //                  DEFT_H2->params.Bypass = 0;     // Don't Bypass Listen DEFT alg.
                    }
                }


                // Do manual Mode switching here:
                if( ( SwitchStateChangeBits &  SW18_AUDIO_ON_OFF ) ||
                    ( SwitchStateChangeBits &  SW19_UNLOCK_SWITCH ) )   // if audio on/off switch(es) changed then...
                {
                    if( ( SwitchState & SW18_AUDIO_ON_OFF ) && ( SwitchState & SW19_UNLOCK_SWITCH ) ) // if audio is activated
                    {                                                                                 // (Audio and Unlock switches are "ON")
                        if( SwitchState & SW20_PUSH_TO_TALK_F ) // if PTT=0
                        {
                            ChangeDSP_Mode( DSP_ModeTemp );             // Restore DSP_Mode
                            AsacsInputSelect = AsacsInputSelectTemp;    // Restore the input signal selector
                        }
                        else    // PTT=1
                        {
                            ChangeDSP_Mode( TALK );     // Set DSP operations to Talk
                        }
                    }
                    else
                    {
                        if( SwitchState & SW20_PUSH_TO_TALK_F ) // if PTT=0
                        {
                            DSP_ModeTemp = DSP_Mode;                    // Save the current DSP_Mode
                            AsacsInputSelectTemp = AsacsInputSelect;    // Save current state of input signal selector
                        }

                        ChangeDSP_Mode( STANDBY );      // Set DSP operations to Standby
                    }
                }
                else    // else audio on/off has not changed
                {
                    if( ( SwitchStateChangeBits &  SW20_PUSH_TO_TALK_F ) &&
                        ( ( SwitchState & SW18_AUDIO_ON_OFF ) && ( SwitchState & SW19_UNLOCK_SWITCH ) ) )   
                    {                                                   // if PTT switch changed and audio is activated then...
                        if( SwitchState & SW20_PUSH_TO_TALK_F ) // if PTT=0
                        {
                            ChangeDSP_Mode( DSP_ModeTemp );             // Restore DSP_Mode
                            AsacsInputSelect = AsacsInputSelectTemp;    // Restore the input signal selector
                        }
                        else    // PTT=1
                        {
                            DSP_ModeTemp = DSP_Mode;                    // Save the current DSP_Mode
                            AsacsInputSelectTemp = AsacsInputSelect;    // Save current state of input signal selector

                            ChangeDSP_Mode( TALK );     // Set DSP operations to Talk
                        }

                    }
                }


                // Act on Volume control switch here:
                if( !( SwitchState & SW16_VOLUME_HIGH ) )           // if Volume High
                    gDB = 0.0;
                else if( !( SwitchState & SW15_VOLUME_MEDIUM ) )    // if Volume Medium
                    gDB = -26.0;
                else                                                // else Volume Low
                    gDB = -60.0;

                GAIN_Handles[1]->gain = gDB;        // Set the gain control
                GAIN_Handles[1]->slowFlag = 1;
                GAIN_Handles[1]->dbFlag = 1;
                GAIN_update( GAIN_Handles[1] );


//LOG_printf(&trace, "SwitchState =                           0x%X", SwitchState );
//LOG_printf(&trace, "SwitchStateChangeBits = 0x%X\n", SwitchStateChangeBits );

            }
            else
            {
                initializeOK = 0;
            }

        }

        TSK_sleep (40); // Sleep here for 40 msec  */

    }   // while(1) 

//  LOG_printf(&trace, "ticks = %d", PRD_getticks());

}


