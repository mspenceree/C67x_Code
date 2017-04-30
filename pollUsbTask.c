
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
 * File: pollUsbTask.c
 *
 * This function initializes the USB interface then loops... waiting for commands (it currently polls).
 * When a command is recieved ...
 *
 *
 * The Lyrtech sample program usbctrlapp and the file controltask.c were used as a starting point.
 *
 *
 *
 *
 *
 */


// PADK Library
#include "PADK.h"

#include <algorithms.h>

#include <main.h>

#define HEADER_LENGTH 6             // Length of USB comm. header
#define BIGBUF_LENGTH 64*1024 + 6   // 64K  (16K Words + 6 Bytes)

/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;

/* 
    Memory segments defined in the asacs.tcf file.
    IRAM  -> DSP internal RAM
    SDRAM -> External RAM
*/
extern Int IRAM;
extern Int SDRAM;

extern unsigned char DSP_Flags;

extern int AsacsInputSelect;
extern int AsacsInputSelectTemp;

extern int AsacsNoiseSourceSelect;
extern int PCLineInLSelect;
extern int PCLineInRSelect;
extern int AuxOut1Select;
extern int AuxOut2Select;

extern float RelativeHumidity;
extern int RelativeHumidityOK;

extern float Temperature;
extern int TemperatureOK;
 
extern float TemperatureDew;
extern int TemperatureDewOK;

extern float Range;
extern int RangeOK;

extern float AuxAnalogIn1;
extern float AuxAnalogIn2;
extern float AuxAnalogIn3;
extern int AuxAnalogInOK;

extern unsigned int SwitchState;


// DSP Block handles
extern pGAIN    GAIN_Handles[ GAIN_BLOCK_MAX_INSTANCE ];
extern pIIR     IIR_Handles[ IIR_BLOCK_MAX_INSTANCE ];
extern pFIR     FIR_Handles[ FIR_BLOCK_MAX_INSTANCE ];

extern pDRC DRC_Handles[ DRC_BLOCK_MAX_INSTANCE ];
//extern pDEFT  DEFT_Handles[ DEFT_BLOCK_MAX_INSTANCE ];
//extern pAFC   AFC_Handles[ AFC_BLOCK_MAX_INSTANCE ];
//extern pDRE   DRE_Handles[ DRE_BLOCK_MAX_INSTANCE ];
//extern pANC   ANC_Handles[ ANC_BLOCK_MAX_INSTANCE ];
//extern pTEST  TEST_Handles[ TEST_BLOCK_MAX_INSTANCE ];



//forward declorations:
void words2bytes(   void * wordsIn,             // pointer to input words (can be float or int)
                    unsigned char * bytesOut,   // pointer to output bytes
                    int Nwords                  // number of words to write
                );

int bytes2words(    unsigned char * bytesIn,    // pointer to input bytes
                    void * wordsOut,            // pointer to output words (can be float or int)
                    int NbytesIn,               // number of bytes available
                    int Nwords                  // number of words to write
                );

//--------------------------------------------------------
// Declare global parameters here
//--------------------------------------------------------
int DSP_Mode;

// Forward Reference:
int CommandInterpreter( unsigned char BlockTypeCode, 
                        unsigned char BlockInstance, 
                        unsigned char ParameterCode, 
                        unsigned char ReadFlag, 
                        int NbytesIn, 
                        int * NbytesOut, 
                        unsigned char * dataBuff );


void pollUsbTask ()
{

//  int LEDtoggle = 0;
    int CommandCount = 1;
    int receivedCount;
    int sendCount;
    int ErrorCode = NO_ERROR;

    int NbytesIn;
    int NbytesOut;

    //unsigned int temp = BIGBUF_LENGTH;

//int intVar;       // for testing
//float floatVar;

    unsigned char BlockTypeCode, BlockInstance, ParameterCode, ReadFlag;

    unsigned char  *bigBuff;        // Buffer to communicate with PC (Size: BIGBUF_LENGTH, allocated to SDRAM heap)


    LOG_printf(&trace, "Entered pollUsbTask.c");


    // Allocate memory for bigBuff and clear contents
    bigBuff = MEM_calloc ( SDRAM, BIGBUF_LENGTH*sizeof( unsigned char ), 8 );


    if( !bigBuff )
    {
        LOG_printf (&trace, "Algorithm memory allocation failed. Exiting program."); // For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }


    // Init USB Interface:
    USB_Reset();

    while (1)
    {
        /* 
            The protocol from PC to DSP is
                1st byte: commandPacket[0] -> Command Validation Byte (0xAA)
                2nd byte: commandPacket[1] -> BlockTypeCode
                3rd byte: commandPacket[2] -> BlockInstance
                4th byte: commandPacket[3] -> ParameterCode
                5th byte: commandPacket[4] -> ReadFlag
                6th byte: commandPacket[5] -> Spare
                7th+bytes: commandPacket[6+] -> Received Data Words (if any)

            The protocol form DSP to PC is
                1st byte: responsePacket[0] -> Respnse Validation Byte (0x55)
                2nd byte: responsePacket[1] -> DSP_Flags
                3rd byte: responsePacket[2] -> ErrorCode
                4th byte: responsePacket[3] -> Spare
                5th byte: responsePacket[4] -> Spare
                6th byte: responsePacket[5] -> Spare
                7th+bytes: responsePacket[6+] -> Transmitted Data Words (if any)
        */

        while (1)
        {
            // Get all sent data: 
            receivedCount = USB_Read( USB_HOST2DSP_PIPE1, bigBuff, BIGBUF_LENGTH, 0 ); // Receive up to BIGBUF_LENGTH Bytes
                                                                        // Returns 0 if nothing to read (or no USB transmision from host)
                                                                        // or returns number of bytes actually read. (non-blocking)

//          receivedCount = USB_Read_WaitForAWhile( USB_HOST2DSP_PIPE1, bigBuff, BIGBUF_LENGTH, BIGBUF_LENGTH ); // Receive up to BIGBUF_LENGTH Bytes, waiting for up to 2ms
                                                                        // Returns 0 if nothing to read (or no USB transmision from host)
                                                                        // or returns number of bytes actually read.
            if( !receivedCount )
            {
                TSK_sleep(2);   // sleep for x ticks (x ms)
                MIDI_EnableLed2(0);     // turn LED off
            }

            else if( receivedCount >= HEADER_LENGTH && bigBuff[0] == 0xAA ) // if Packet header OK
            {
                BlockTypeCode = bigBuff[1]; // Get header values
                BlockInstance = bigBuff[2];
                ParameterCode = bigBuff[3];
                ReadFlag = bigBuff[4];
                break;  // break out of while loop
            }
            else
            {
                LOG_printf (&trace, "Error in header. 0xAA missing or minimum of HEADER_LENGTH Bytes not received. Resetting USB");
                USB_Reset();
                TSK_sleep(2);   // sleep for 2 ticks (2ms)
            }
        }

 LOG_printf(&trace, "\n\n******* Valid Header Received ******* Current Command Count = %d", CommandCount++);
 LOG_printf(&trace, " # Header and Data Bytes Received from PC = %d", receivedCount);

        // Toggle the MIDI LED when packet received:
//      MIDI_EnableLed2( LEDtoggle ^= 1 );  // this is a very fast function

        MIDI_EnableLed2(1); // turn on the LED
//      TSK_sleep (2);      // sleep for x ms

/*
// For code testing:
    BlockTypeCode = 108;
    BlockInstance = 1;
    ParameterCode = 1;
    ReadFlag = 1;
    receivedCount = 10;

    intVar = 2;
    floatVar = 1234.567;

//  words2bytes(  &intVar, &bigBuff[HEADER_LENGTH], 1 );    // put data into buffer

*/

        // Interpret the command and load bigBuff with output Data (if Read):
        NbytesIn = receivedCount -  HEADER_LENGTH;
        ErrorCode = CommandInterpreter( BlockTypeCode,
                                        BlockInstance,
                                        ParameterCode, 
                                        ReadFlag, 
                                        NbytesIn, 
                                        &NbytesOut, 
                                        &bigBuff[HEADER_LENGTH] );  // work with data after the header

        // Load header into bigBuff:
        bigBuff[0] = 0x55;          // Response Validation Byte
        bigBuff[1] = DSP_Flags;     // Global variable
        bigBuff[2] = ErrorCode;     // Local variable
        bigBuff[3] = 0;             // Send zeros for Spares
        bigBuff[4] = 0;
        bigBuff[5] = 0;

        // Send header and Data to the PC:
        sendCount =  USB_Write( USB_DSP2HOST_PIPE1, bigBuff, HEADER_LENGTH + NbytesOut, 0 );    // This is a non-blocking call
                                                                                    // (Bytes can be successfully written even if PC is not reading -
                                                                                    // they are stored in a buffer somewhere)
                                                                                    // fixthis: Is blocking call required?
 LOG_printf(&trace, " # Header and Data Bytes sent to PC = %d", sendCount);
        
//      TSK_sleep (1); // sleep for x msec

    }


}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  CommandInterpreter()
//
//  This function verifies the header parameters and reads or writes the Block data as detailed in
//  the header file "algorithms.h".
//

/* Recall:
#define NO_ERROR                        0
#define BLOCK_TYPE_CODE_ERROR           1
#define BLOCK_INSTANCE_ERROR            2
#define PARAMETER_CODE_ERROR            3
#define WRITE_NOT_SUPPORTED             4
#define READ_NOT_SUPPORTED              5
#define DATA_VALUES_OUT_OF_RANGE        6   // don't do range check. Just saturate the variable and overwrite
                                            // in ALGX_update() functions. 
#define NUMBER_OF_DATA_WORDS_INCORRECT  7
*/



int CommandInterpreter( unsigned char BlockTypeCode,
                        unsigned char BlockInstance, 
                        unsigned char ParameterCode, 
                        unsigned char ReadFlag,         // 0= write
                        int NbytesIn,                   // number of bytes availble in dataBuff
                        int *NbytesOut,                 // number of bytes to be written to dataBuff
                        unsigned char *dataBuff )       // data buffer (not to include header)
{
    int errorCode;

    int paramReadOK = 1;    // Default the parameters to be read or write
    int paramWriteOK = 1;

    int Nwords = 1;         // Default to single word parameters

    int BIm1 = BlockInstance - 1;   // for index into Handles

    void *paramPtr; // temparary parameter pointer

    int tempInt;        // temp interger
//  float tempFloat;    // temp float

    *NbytesOut = 0; // Default to 0 Bytes to write

    if( ReadFlag == 0 && NbytesIn == 0 )    // if no data available when PC writes, then return error code
    {
        LOG_printf(&trace, "ERROR: No Data available for write");
        return( NUMBER_OF_DATA_WORDS_INCORRECT );
    }


    switch( BlockTypeCode ) {

    case GAIN_BLOCK_TYPE_CODE:

        if( ( BlockInstance > GAIN_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )  // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & GAIN_Handles[ BIm1 ]->mute );   // point to the parameter of interest
                break;
            case 2:
                paramPtr = (void *) ( & GAIN_Handles[ BIm1 ]->bypass ); // point to the parameter of interest
                break;
            case 3:
                paramPtr = (void *) ( & GAIN_Handles[ BIm1 ]->dbFlag ); // point to the parameter of interest
                break;
            case 4:
                paramPtr = (void *) ( & GAIN_Handles[ BIm1 ]->slowFlag );   // point to the parameter of interest
                break;
            case 5:
                paramPtr = (void *) ( & GAIN_Handles[ BIm1 ]->gain );   // point to the parameter of interest
                break;
            case 128:
                paramPtr = (void *) ( & GAIN_Handles[ BIm1 ]->multiplier ); // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;

    case DRC_BLOCK_TYPE_CODE:

        if( ( BlockInstance > DRC_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )   // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->mute );    // point to the parameter of interest
                break;
            case 2:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->bypass );  // point to the parameter of interest
                break;
            case 3:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->Nzones );  // point to the parameter of interest
                break;
            case 4:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->attackTime );  // point to the parameter of interest
                break;
            case 5:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->decayTime );   // point to the parameter of interest
                break;
            case 6:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->LookAheadFlag );   // point to the parameter of interest
                break;
            case 7:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->detectorType );    // point to the parameter of interest
                break;
            case 8:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->usePeakLimiter );  // point to the parameter of interest
                break;
            case 9:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->X0 );  // point to the parameter of interest
                break;
            case 10:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->X1 );  // point to the parameter of interest
                break;
            case 11:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->X2 );  // point to the parameter of interest
                break;
            case 12:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->Y0 );  // point to the parameter of interest
                break;
            case 13:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->s0 );  // point to the parameter of interest
                break;
            case 14:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->s1 );  // point to the parameter of interest
                break;
            case 15:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->s2 );  // point to the parameter of interest
                break;
            case 16:
                paramPtr = (void *) ( & DRC_Handles[ BIm1 ]->s3 );  // point to the parameter of interest
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;




    case CONTROLLER_BLOCK_TYPE_CODE:

        if( ( BlockInstance > CONTROLLER_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )    // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                if( ReadFlag )  // if PC reading
                {
                    paramPtr = (void *) ( & DSP_Mode ); // point to the parameter of interest
                }
                else    // PC writing
                {
                    paramPtr = (void *) ( & tempInt );  // point to tempInt
                    // Copy the input buffer bytes to tempInt (and return if error)
                    if( errorCode = bytes2words( dataBuff, paramPtr, NbytesIn, Nwords ) )
                        return( errorCode );    // will return "NUMBER_OF_DATA_WORDS_INCORRECT" if NbytesIn != 4*Nwords

                    if( tempInt != STANDBY && tempInt != LISTEN && tempInt != TALK )    // if invalid value
                        return( DATA_VALUES_OUT_OF_RANGE );
                    else
                        ChangeDSP_Mode( tempInt );      // Set DSP operations to specified mode
                }
                break;
            case 2:
                if( ReadFlag )  // if PC reading
                {
                    paramReadOK = 0;    // parameter is not readable
                }
                else    // PC writing
                {
                    paramPtr = (void *) ( & tempInt );  // point to tempInt
                    // Copy the input buffer bytes to tempInt (and return if error)
                    if( errorCode = bytes2words( dataBuff, paramPtr, NbytesIn, Nwords ) )
                        return( errorCode );    // will return "NUMBER_OF_DATA_WORDS_INCORRECT" if NbytesIn != 4*Nwords

                    if( tempInt == 1 || tempInt == 99)  // if 1, Reset DSP
                        //resetDSP( tempInt );  // 1= Reset DSP, 99= Write factory default parameter and Reset DSP. fixthis: add a function
                        tempInt = 1234; // temp filler
                    else
                        return( DATA_VALUES_OUT_OF_RANGE );
                }
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;

    case SELECTOR_BLOCK_TYPE_CODE:

        if( ( BlockInstance > SELECTOR_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )  // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                switch( BlockInstance ) {
                case 1:
                    paramPtr = (void *) ( & AsacsInputSelect ); // point to the parameter of interest
                    break;
                case 2:
                    paramPtr = (void *) ( & AsacsNoiseSourceSelect );   // point to the parameter of interest
                    break;
                case 3:
                    paramPtr = (void *) ( & PCLineInLSelect );  // point to the parameter of interest
                    break;
                case 4:
                    paramPtr = (void *) ( & PCLineInRSelect );  // point to the parameter of interest
                    break;
                case 5:
                    paramPtr = (void *) ( & AuxOut1Select );    // point to the parameter of interest
                    break;
                case 6:
                    paramPtr = (void *) ( & AuxOut2Select );    // point to the parameter of interest
                    break;
                }  // switch( BlockInstance )
                break;
            }  // switch( ParameterCode )

        }
        break;

    case DIGITAL_IN_BLOCK_TYPE_CODE:

        if( ( BlockInstance > DIGITAL_IN_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )    // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & SwitchState );  // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );
            }  // switch( ParameterCode )

        }
        break;

    case TEMPERATURE_BLOCK_TYPE_CODE:

        if( ( BlockInstance > TEMPERATURE_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )   // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & Temperature );  // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 2:
                paramPtr = (void *) ( & TemperatureOK );    // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;

    case TEMPERATURE_DEW_BLOCK_TYPE_CODE:

        if( ( BlockInstance > TEMPERATURE_DEW_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )   // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & TemperatureDew );   // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 2:
                paramPtr = (void *) ( & TemperatureDewOK ); // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;

    case HUMIDITY_BLOCK_TYPE_CODE:

        if( ( BlockInstance > HUMIDITY_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )  // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & RelativeHumidity ); // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 2:
                paramPtr = (void *) ( & RelativeHumidityOK );   // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;

    case RANGE_BLOCK_TYPE_CODE:

        if( ( BlockInstance > RANGE_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) ) // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & Range );    // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 2:
                paramPtr = (void *) ( & RangeOK );  // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;

    case ADC_BLOCK_TYPE_CODE:

        if( ( BlockInstance > ADC_BLOCK_MAX_INSTANCE ) || ( BlockInstance < 1 ) )   // if Block Instance is out-of-range
            return( BLOCK_INSTANCE_ERROR );
        
        else
        {
            switch( ParameterCode ) {
            case 1:
                paramPtr = (void *) ( & AuxAnalogIn1 ); // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 2:
                paramPtr = (void *) ( & AuxAnalogIn2 ); // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 3:
                paramPtr = (void *) ( & AuxAnalogIn3 ); // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            case 4:
                paramPtr = (void *) ( & AuxAnalogInOK );    // point to the parameter of interest
                paramWriteOK = 0;   // parameter is not writable
                break;
            default:
                return( PARAMETER_CODE_ERROR );

            }  // switch( ParameterCode )

        }
        break;





    default: // Unknown Block Type
        return( BLOCK_TYPE_CODE_ERROR );

    }  // switch( BlockTypeCode )


    // Now read or write the parameter(s):
    if( ReadFlag )  // if PC reading
    {
        if( !paramReadOK )
            return( READ_NOT_SUPPORTED );

        // Copy the four bytes of the parameter into the output buffer
        words2bytes( paramPtr, dataBuff, Nwords );
        *NbytesOut = Nwords << 2;   // multiply by 4 to get byte count
    }
    else    // PC writing
    {
        if( !paramWriteOK )
            return( WRITE_NOT_SUPPORTED );

        // Copy the input buffer bytes to the parameter (and return if error)
        if( errorCode = bytes2words( dataBuff, paramPtr, NbytesIn, Nwords ) )
            return( errorCode );    // will return "NUMBER_OF_DATA_WORDS_INCORRECT" if NbytesIn != 4*Nwords
    }

    return( NO_ERROR );
}





//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  words2bytes( )
//
//  This function copies 4-Byte words into a number of bytes.
//

void words2bytes(   void *wordsIn,          // pointer to input words (can be float or int)
                    unsigned char *bytesOut,    // pointer to output bytes
                    int Nwords                  // number of words to write
                )
{
    int i;
    unsigned char *bytesInPtr;

    bytesInPtr = (unsigned char *) wordsIn; // cast the wordsIn ptr to unsigned char ptr

    for( i = 0; i < ( Nwords << 2 ); i++ )              // for i = 0 to (4*Nwords - 1)
        *( bytesOut + i ) =  *( bytesInPtr + i );       // copy bytes

    return;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  bytes2words( )
//
//  This function copies bytes into 4-Byte words and checks for an error in the Byte count.
//

int bytes2words(    unsigned char *bytesIn, // pointer to input bytes
                    void *wordsOut,             // pointer to output words (can be float or int)
                    int NbytesIn,               // number of bytes available
                    int Nwords                  // number of words to write
                )
{
    int i;
    unsigned char *bytesOutPtr;

    bytesOutPtr = (unsigned char *) wordsOut;   // cast the wordsOut into unsigned char ptr


    if( NbytesIn != ( Nwords << 2 ) )       // if NbytesIn != 4*Nwords
    {
        return( NUMBER_OF_DATA_WORDS_INCORRECT );
    }
    else
    {
        for( i = 0; i < NbytesIn; i++ )             // for i = 0 to (4*Nwords - 1)
            *( bytesOutPtr + i ) =  *( bytesIn + i );       // copy bytes
    }

    return( NO_ERROR );
}


