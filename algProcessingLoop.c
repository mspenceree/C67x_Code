
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
 * File: algProcessingLoop.c
 *
 * This is the main processing loop.
 *
    The processingLoop code performs data RX and TX between the input and
    output driver instances. The data received from the McASP in
    the driver buffer is converted to floating point in the frame
    buffer. The frame buffer data is then converted to fixed point
    and provided to the McASP for transmit.
    The float<->fixed conversion is done to demonstrate how audio
    processing code can be inserted b/w the actual RX and TX.
    The processingLoop function accepts as an argument the processingLoop task handle.

 *
 */

#include <algProcessingTask.h>
#include <algorithms.h>
#include <main.h>


/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;


// Global variables
extern float *inBuff[ IO_MAX_CHAN ];
extern float *outBuff[ IO_MAX_CHAN ];
extern float *Buff[ BUFF_MAX_P1 ];

extern float FadeWindow[FRAMESIZE];


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



int AsacsInputSelect;
int AsacsNoiseSourceSelect;
int PCLineInLSelect;
int PCLineInRSelect;
int AuxOut1Select;
int AuxOut2Select;

int TalkOpStatus;
int ListenOpStatus;
int MonitorOpStatus;
int TestOpStatus;

float *muxOutPtr;

// Forward Reference:
float* signalSelector( int select, float *ptr0,
                       float *ptr1, float *ptr2, float *ptr3, float *ptr4,
                       float *ptr5, float *ptr6, float *ptr7, float *ptr8);


Int32 processingLoop (pIOstreamHdl IOhdl)// , Float *inBuff[] )
{
    Int32 error, status;
    Void *input, *output;
    Int32 nmadus;
    Int32 NinSamples, NoutSamples;

    Int32 i, j;
//  Uint32 ipMask, opMask;
    Float ipScale, opScale;

    float *restrict fPtr;
    int *restrict iPtr;
    int *restrict oPtr;

   // Precompute input (A/D) mask and scaling:
//  ipMask = (Int)0xffffffff << ( 32- IOhdl->pParams->ipPrecision );
    ipScale = 1.0/MVAL32BIT;
        
   // Precompute output (D/A) mask and scaling:
//  opMask = (Int)0xffffffff << ( 32- IOhdl->pParams->opPrecision );
    opScale = MVAL32BIT;

    NinSamples = FRAMESIZE*IOhdl->pParams->maxIPChannels;
    NoutSamples = FRAMESIZE*IOhdl->pParams->maxOPChannels;

    /* Initialize the error flag to 'no error' */
    error = SYS_OK;

    while (1) {

        /*
            Reclaim the filled buffer from the RX section. The 
            driver configures the dMAX to performs data sorting.
            The received data is seperated out into various audio
            channels.
            The reclaim call returns with the number of MADUS that
            are available in the input buffer. If there is an error
            in the reclaim, the error is returned as a negative 
            error code.
        */         
        nmadus = SIO_reclaim (IOhdl->sioIP, (void *) &input, NULL);
        if (nmadus < 0)    {
            error = - nmadus;
            break;
        }

        /*
            Convert the fixed point data received in the 
            driver buffer to floating point data in the frame buffer.
            The ipPrecision indicates the precision of the input fixed point
            data. For ex, if the precision is 24, it is required that the 
            lower 32-24 = 8bits be cleared before the input data is converted 
            to floating point.

            To convert the data to floating point, divide the updated data by
            the maximum signed numeric value that can be represented by 32bits.
        */
        
        // Convert the fixed point data to floating point:
        iPtr = (int *)input;    // Point to fixed point input array
        fPtr = inBuff[0];       // Pointer to input frame buffer
        for (i = 0; i < NinSamples; i++)
//          fPtr[i] = (Float)( iPtr[i] & (ipMask) ) * ipScale;
            fPtr[i] = (Float)( iPtr[i] ) * ipScale; // No mask required. The lsb's are already zero


        /* Issue the empty buffer back to the RX section. */
        status = SIO_issue (IOhdl->sioIP, input, 
                            IOhdl->pParams->maxIPChannels*FRAMESIZE*
                            sizeof(float), NULL);
        if (status != SYS_OK) {
            error = status;
            break;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        //
        //  This is the portion of the processing loop that ASACS DSP operations are performed!
        //
        //
        //  We now have pointers to the 8 analog input channel buffers (Sources) in:
        //
        //      inBuff[ch1], inBuff[ch2], inBuff[ch3], inBuff[ch4],
        //      inBuff[ch5], inBuff[ch6], inBuff[ch7], inBuff[ch8]
        //
        //  We have working buffers available:
        //
        //      Buff[1],... Buff[ BUFF_MAX ]
        //
        //  We also have a zero-filled buffer available: Buff[0]
        //
        //  The DSP operations for ASACS are then performed.
        //
        //  Outputs are written to one of the 8 analog output channel buffers (Sinks) in:
        //
        //      outBuff[ch1], outBuff[ch2], outBuff[ch3], outBuff[ch4],
        //      outBuff[ch5], outBuff[ch6], outBuff[ch7], outBuff[ch8]
        //
        //  


//      HWI_disable(); // commented out by MSpencer
        

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Talk Processing Here:
        ///////////////////////////////////////////////////////////////////////////////////////////

        if( TalkOpStatus >= DSP_OP_RUNNING )    // Process Talk Blocks
        {

//for( i=0; i<10000; i++ ) { }  // delay

            // Compute a sum if required:
            if( AsacsInputSelect == 5 )
            {
                i = FRAMESIZE_M1;
                do {    // Sum the Left and Right and divide by 2:
                    *( Buff[1] + i ) = 0.5*( *( inBuff[ch1] + i ) + *( inBuff[ch2] + i ) ); 
                } while( i-- );
            }
            else if( AsacsInputSelect == 8 )
            {
                i = FRAMESIZE_M1;
                do {    // Sum the Left and Right and divide by 2:
                    *( Buff[1] + i ) = 0.5*( *( inBuff[ch3] + i ) + *( inBuff[ch4] + i ) ); 
                } while( i-- );
            }

            // Input signal selector:
            muxOutPtr = signalSelector( AsacsInputSelect,
                            Buff[0],
                            inBuff[ch5], inBuff[ch6], inBuff[ch1], inBuff[ch2],
                            Buff[1], inBuff[ch3], inBuff[ch4], Buff[1] );


            IIR_apply( IIR_Handles[0], muxOutPtr, Buff[1] );        // HighPass Filter

            IIR_apply( IIR_Handles[1], Buff[1], Buff[1] );          // LowPass Filter

            //DRC_apply( DRC_Handles[0], Buff[1], Buff[1] );            // Dynamic Range Compressor

            //AFC_apply( AFC_Handles[0], Buff[1], Buff[1] );            // Accoustic Feedback Canceller

            GAIN_apply( GAIN_Handles[0], Buff[1], Buff[2] );        // Gain Block (AsacsSpeakerMonitorLevel)

            GAIN_apply( GAIN_Handles[1], Buff[1], Buff[1] );        // Gain Block (Volume Switch)

            GAIN_apply( GAIN_Handles[2], Buff[1], Buff[1] );        // Gain Block (AsacsSpeakerVolume)

            //DEFT_apply( DEFT_Handles[0], Buff[1], Buff[1] );          // Distant Energy Frequency Tailoring (DEFT Alg.)

            //FIR_apply( FIR_Handles[0], Buff[1], Buff[1] );            // Speaker/Driver Equalization Filter

            //FIR_apply( FIR_Handles[1], Buff[1], Buff[3] );            // Fixed Beamformer is just a pair of FIR filters
            //FIR_apply( FIR_Handles[2], Buff[1], Buff[4] );

            //DRC_apply( DRC_Handles[1], Buff[3], Buff[3] );            // Dynamic Range Compressor
            //DRC_apply( DRC_Handles[2], Buff[4], Buff[4] );            // Dynamic Range Compressor

    // Copy Buff[1] to outBuff[ch1], outBuff[ch2] and outBuff[ch3]:
    i = FRAMESIZE_M1;
    do {
        *( outBuff[ch1] + i ) =
        *( outBuff[ch2] + i ) =
        *( outBuff[ch3] + i ) = *( Buff[1] + i );
    } while( i-- );

            // Copy outBuff[ch1] to outBuff[ch3]:
            //i = FRAMESIZE_M1;
            //do {
            //  *( outBuff[ch3] + i ) = *( outBuff[ch1] + i );
            //} while( i-- );


            // Check if the Talk operation was just turned on/off:
            if( TalkOpStatus == DSP_OP_START )  // Fade output buffers FROM zero to avoid startup "pop"
            {
                i = FRAMESIZE_M1;
                j = 0;
                do {
                    *( outBuff[ch1] + i ) = FadeWindow[j]* ( *( outBuff[ch1] + i ) );   // Un-Fade speaker outputs
                    *( outBuff[ch2] + i ) = FadeWindow[j]* ( *( outBuff[ch2] + i ) );
                    *( outBuff[ch3] + i ) = FadeWindow[j]* ( *( outBuff[ch3] + i ) );
                    *( Buff[2] + i ) = FadeWindow[j++]* ( *( Buff[2] + i ) );           // Un-Fade Headphone monitor
                } while( i-- );

                TalkOpStatus = DSP_OP_RUNNING;  // Flag operation as running
            }
            else if( TalkOpStatus == DSP_OP_STOP )  // Fade output buffers TO zero to avoid turn-off "pop"
            {
                i = FRAMESIZE_M1;
                do {
                    *( outBuff[ch1] + i ) = FadeWindow[i]* ( *( outBuff[ch1] + i ) );   // Fade speaker outputs
                    *( outBuff[ch2] + i ) = FadeWindow[i]* ( *( outBuff[ch2] + i ) );
                    *( outBuff[ch3] + i ) = FadeWindow[i]* ( *( outBuff[ch3] + i ) );
                    *( Buff[2] + i ) = FadeWindow[i]* ( *( Buff[2] + i ) );         // Fade Headphone monitor
                } while( i-- );

                TalkOpStatus = DSP_OP_STOPPING; // Flag operation as stopping
            }

        }   // if( TalkOpStatus >= DSP_OP_RUNNING )

        else if(  TalkOpStatus == DSP_OP_STOPPING ) // Last pass was a DSP_OP_STOP command...
                                                    // now clear states and zero output buffers:
        {
            IIR_clearState( IIR_Handles[0] );       // Clear state history of HighPass Filter

            IIR_clearState( IIR_Handles[1] );       // Clear state history of LowPass Filter

            //DRC_clearState( DRC_Handles[0] );     // Clear state history of Dynamic Range Compressor

            //AFC_clearState( AFC_Handles[0] );     // Clear state history of Accoustic Feedback Canceller

            //DEFT_clearState( DEFT_Handles[0] );       // Clear state history of Distant Energy Frequency Tailoring (DEFT Alg.)

            //FIR_clearState( FIR_Handles[0] );     // Clear state history of Speaker/Driver Equalization Filter

            //FIR_clearState( FIR_Handles[1] );     // Clear state history of Fixed Beamformer is just a pair of FIR filters
            //FIR_clearState( FIR_Handles[2] );

            //DRC_clearState( DRC_Handles[1] );         // Clear state history of Dynamic Range Compressor
            //DRC_clearState( DRC_Handles[2] );         // Clear state history of Dynamic Range Compressor


            // Zero out the output buffers (they still have the fade TO zero data in them):
            i = FRAMESIZE_M1;
            do {
                *( outBuff[ch1] + i ) = 0.0;        // Zero the buffers
                *( outBuff[ch2] + i ) = 0.0;
                *( outBuff[ch3] + i ) = 0.0;
                *( Buff[2] + i ) = 0.0;
            } while( i-- );

            TalkOpStatus = DSP_OP_STOPPED;  // Flag operation as stopped
        }



        ///////////////////////////////////////////////////////////////////////////////////////////
        // Listen Processing Here:
        ///////////////////////////////////////////////////////////////////////////////////////////

        if( ListenOpStatus >= DSP_OP_RUNNING )  // Process Listen Blocks
        {

            IIR_apply( IIR_Handles[2], inBuff[ch7], Buff[1] );      // HighPass Filter

            IIR_apply( IIR_Handles[3], inBuff[ch8], Buff[2] );      // HighPass Filter

            //DRE_apply( DRE_Handles[0], Buff[1], Buff[2], Buff[1] );   // Dynamic Range Extender

            IIR_apply( IIR_Handles[4], Buff[1], Buff[1] );          // LowPass Filter

            //FIR_apply( FIR_Handles[3], Buff[1], Buff[1] );            // Speaker/Driver Equalization Filter

            // Compute a sum if required:
            if( AsacsNoiseSourceSelect == 5 )
            {
                i = FRAMESIZE_M1;
                do {    // Sum the Left and Right and divide by 2:
                    *( Buff[2] + i ) = 0.5*( *( inBuff[ch1] + i ) + *( inBuff[ch2] + i ) ); 
                } while( i-- );
            }

            // Noise source selector for ANC alg.:
            muxOutPtr = signalSelector( AsacsNoiseSourceSelect,
                            Buff[0],
                            inBuff[ch5], inBuff[ch6], inBuff[ch1], inBuff[ch2],
                            Buff[2], Buff[0], Buff[0], Buff[0] );

            IIR_apply( IIR_Handles[5], muxOutPtr, Buff[2] );        // HighPass Filter

            IIR_apply( IIR_Handles[6], Buff[2], Buff[2] );          // LowPass Filter

            //ANC_apply( ANC_Handles[0],  Buff[1], Buff[2], Buff[1] );  // Adaptive Noise Caneller

            //DEFT_apply( DEFT_Handles[1], Buff[1], Buff[1] );      // Distant Energy Frequency Tailoring (DEFT Alg.)

            //DRC_apply( DRC_Handles[3], Buff[1], Buff[1] );            // Dynamic Range Compressor

            GAIN_apply( GAIN_Handles[3], Buff[1], Buff[2] );        // Gain Block (AsacsSpeakerMonitorLevel)


            // Check if the Listen operation was just turned on:
            if( ListenOpStatus == DSP_OP_START )    // Fade output buffers FROM zero to avoid startup "pop"
            {
                i = FRAMESIZE_M1;
                j = 0;
                do {
                    *( Buff[2] + i ) = FadeWindow[j++]* ( *( Buff[2] + i ) );           // Un-Fade Headphone monitor
                } while( i-- );

                ListenOpStatus = DSP_OP_RUNNING;    // Flag operation as running
            }
            else if( ListenOpStatus == DSP_OP_STOP )    // Fade output buffers TO zero to avoid turn-off "pop"
            {
                i = FRAMESIZE_M1;
                do {
                    *( Buff[2] + i ) = FadeWindow[i]* ( *( Buff[2] + i ) );         // Fade Headphone monitor
                } while( i-- );

                ListenOpStatus = DSP_OP_STOPPING;   // Flag operation as stopping
            }

        }   // if( ListenOpStatus >= DSP_OP_RUNNING )

        else if(  ListenOpStatus == DSP_OP_STOPPING )   // Last pass was a DSP_OP_STOP command...
                                                        // now clear states and zero output buffers:
        {
            IIR_clearState( IIR_Handles[2] );       // Clear state history of HighPass Filter

            IIR_clearState( IIR_Handles[3] );       // Clear state history of HighPass Filter

            //DRE_clearState( DRE_Handles[0] );     // Clear state history of Dynamic Range Extender

            IIR_clearState( IIR_Handles[4] );       // Clear state history of LowPass Filter

            //FIR_clearState( FIR_Handles[3] );     // Clear state history of Speaker/Driver Equalization Filter

            IIR_clearState( IIR_Handles[5] );       // Clear state history of HighPass Filter

            IIR_clearState( IIR_Handles[6] );       // Clear state history of LowPass Filter

            //ANC_clearState( ANC_Handles[0] );     // Clear state history of Adaptive Noise Caneller

            //DEFT_clearState( DEFT_Handles[1] );       // Clear state history of Distant Energy Frequency Tailoring (DEFT Alg.)

            //DRC_clearState( DRC_Handles[3] );         // Clear state history of Dynamic Range Compressor

            // Zero out the output buffers (they still have the fade TO zero data in them):
            i = FRAMESIZE_M1;
            do {
                *( Buff[2] + i ) = 0.0;     // Zero the buffers
            } while( i-- );

            ListenOpStatus = DSP_OP_STOPPED;    // Flag operation as stopped
        }



        ///////////////////////////////////////////////////////////////////////////////////////////
        // Monitor Processing Here:
        ///////////////////////////////////////////////////////////////////////////////////////////

        if( MonitorOpStatus >= DSP_OP_RUNNING ) // Process Monitor Blocks
        {
            // Signal Selector:
            muxOutPtr = signalSelector( PCLineInLSelect,
                            Buff[0],
                            Buff[2], Buff[0], Buff[0], Buff[0],
                            Buff[0], Buff[0], Buff[0], Buff[0] );

            GAIN_apply( GAIN_Handles[4], muxOutPtr, outBuff[ch6] );     // Gain Block

            // Signal Selector:
            muxOutPtr = signalSelector( PCLineInRSelect,
                            Buff[0],
                            Buff[2], Buff[0], Buff[0], Buff[0],
                            Buff[0], Buff[0], Buff[0], Buff[0] );

            GAIN_apply( GAIN_Handles[5], muxOutPtr, outBuff[ch7] );     // Gain Block

            // Signal Selector:
            muxOutPtr = signalSelector( AuxOut1Select,
                            Buff[0],
                            Buff[2], Buff[0], Buff[0], Buff[0],
                            Buff[0], Buff[0], Buff[0], Buff[0] );

            GAIN_apply( GAIN_Handles[6], muxOutPtr, outBuff[ch4] );     // Gain Block

            // Signal Selector:
            muxOutPtr = signalSelector( AuxOut2Select,
                            Buff[0],
                            Buff[2], Buff[0], Buff[0], Buff[0],
                            Buff[0], Buff[0], Buff[0], Buff[0] );

            GAIN_apply( GAIN_Handles[7], muxOutPtr, outBuff[ch5] );     // Gain Block


            // Check if the Monitor operation was just turned on/off:
            if( MonitorOpStatus == DSP_OP_START )   // Fade output buffers FROM zero to avoid startup "pop"
            {
                i = FRAMESIZE_M1;
                j = 0;
                do {
                    *( outBuff[ch6] + i ) = FadeWindow[j]* ( *( outBuff[ch6] + i ) );   // Un-Fade outputs
                    *( outBuff[ch7] + i ) = FadeWindow[j]* ( *( outBuff[ch7] + i ) );
                    *( outBuff[ch4] + i ) = FadeWindow[j]* ( *( outBuff[ch4] + i ) );
                    *( outBuff[ch5] + i ) = FadeWindow[j++]* ( *( outBuff[ch5] + i ) );
                } while( i-- );

                MonitorOpStatus = DSP_OP_RUNNING;   // Flag operation as running
            }
            else if( MonitorOpStatus == DSP_OP_STOP )   // Fade output buffers TO zero to avoid turn-off "pop"
            {
                i = FRAMESIZE_M1;
                do {
                    *( outBuff[ch6] + i ) = FadeWindow[i]* ( *( outBuff[ch6] + i ) );   // Un-Fade outputs
                    *( outBuff[ch7] + i ) = FadeWindow[i]* ( *( outBuff[ch7] + i ) );
                    *( outBuff[ch4] + i ) = FadeWindow[i]* ( *( outBuff[ch4] + i ) );
                    *( outBuff[ch5] + i ) = FadeWindow[i]* ( *( outBuff[ch5] + i ) );
                } while( i-- );

                MonitorOpStatus = DSP_OP_STOPPING;  // Flag operation as stopping
            }

        }   // if( MonitorOpStatus >= DSP_OP_RUNNING )

        else if(  MonitorOpStatus == DSP_OP_STOPPING )  // Last pass was a DSP_OP_STOP command...
                                                    // now clear states and zero output buffers:
        {
            //not required- GAIN_clearState( GAIN_Handles[4] );     // Clear state history of Gain Block

            //not required- GAIN_clearState( GAIN_Handles[5] );     // Clear state history of Gain Block

            //not required- GAIN_clearState( GAIN_Handles[6] );     // Clear state history of Gain Block

            //not required- GAIN_clearState( GAIN_Handles[7] );     // Clear state history of Gain Block

            // Zero out the output buffers (they still have the fade TO zero data in them):
            i = FRAMESIZE_M1;
            do {
                *( outBuff[ch6] + i ) = 0.0;        // Zero the buffers
                *( outBuff[ch7] + i ) = 0.0;
                *( outBuff[ch4] + i ) = 0.0;
                *( outBuff[ch5] + i ) = 0.0;
            } while( i-- );

            MonitorOpStatus = DSP_OP_STOPPED;   // Flag operation as stopped
        }



        ///////////////////////////////////////////////////////////////////////////////////////////
        // Test Processing Here: add code...
        ///////////////////////////////////////////////////////////////////////////////////////////

        if( TestOpStatus >= DSP_OP_RUNNING )    // Process Test Blocks
        {

            //GAIN_apply( GAIN_Handles[8]9, muxOutPtr, outBuff[ch5] );      // Gain Block

            // Check if the Test operation was just turned on:
            if( TestOpStatus == DSP_OP_START )  // Fade output buffers FROM zero to avoid startup "pop"
            {
                i = FRAMESIZE_M1;
                j = 0;
                do {
                    //*( outBuff[ch6] + i ) = FadeWindow[j]* ( *( outBuff[ch6] + i ) ); // Un-Fade outputs
                } while( i-- );

                TestOpStatus = DSP_OP_RUNNING;  // Flag operation as running
            }
            else if( TestOpStatus == DSP_OP_STOP )  // Fade output buffers TO zero to avoid turn-off "pop"
            {
                i = FRAMESIZE_M1;
                do {
                    //*( outBuff[ch6] + i ) = FadeWindow[i]* ( *( outBuff[ch6] + i ) ); // Un-Fade outputs
                } while( i-- );

                TestOpStatus = DSP_OP_STOPPING; // Flag operation as stopping
            }

        }   // if( TestOpStatus >= DSP_OP_RUNNING )

        else if(  TestOpStatus == DSP_OP_STOPPING ) // Last pass was a DSP_OP_STOP command...
                                                    // now clear states and zero output buffers:
        {


            //not required- GAIN_clearState( GAIN_Handles[6] );     // Clear state history of Gain Block


            // Zero out the output buffers (they still have the fade TO zero data in them):
            i = FRAMESIZE_M1;
            do {
                //*( outBuff[ch6] + i ) = 0.0;      // Zero the buffers
            } while( i-- );

            TestOpStatus = DSP_OP_STOPPED;  // Flag operation as stopped
        }










//      HWI_enable();

        //
        //
        //  End of DSP operations for ASACS.
        //
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////



        /*
            Reclaim the empty buffer from the TX section. The 
            driver configures the dMAX to performs data sorting.
            Thus the data provided to the driver is provided as
            seperate channel blocks.
        */         
        nmadus = SIO_reclaim (IOhdl->sioOP, (void *) &output, NULL);
        if (nmadus < 0)    {
            error = - nmadus;
            break;
        }

        /*
            Convert the floating point data present in the 
            frame buffer to fixed point data in the output buffer.
            The opPrecision indicates the precision of the output fixed point
            data. For ex, if the precision is 24, it is required that the 
            lower 32-24 = 8bits be cleared before the output data is transmitted
            by McASP.

            To convert the data to fixed point, multiply the floating point data
            with the maximum signed numeric value that can be represented by 
            32bits.
        */

//outputFormat ( IOhdl, (Int *)output);

        // Convert the floating point data to fixed point:
        oPtr = (int *)output;   // Point to fixed point output array
        fPtr = outBuff[0];      // Point to the output frame buffer
        for (i = 0; i < NoutSamples; i++)
//            oPtr[i] = _spint(fPtr[i] * opScale) & opMask;
            oPtr[i] = _spint(fPtr[i] * opScale);    // What's the point of masking?


        /* Issue the filled buffer back to the TX section. */
        status = SIO_issue (IOhdl->sioOP, output, 
                            IOhdl->pParams->maxIPChannels*FRAMESIZE
                            *sizeof(float), NULL);
        if (status != SYS_OK) {
            error = status;
            break;
        }
    }

    /* Return with the error code */
    return( error );
}



// Signal multiplexer returns copy of the selected pointer (ptr0 to ptr8):
float * signalSelector( int select, float *ptr0,
                                float *ptr1, float *ptr2, float *ptr3, float *ptr4,
                                float *ptr5, float *ptr6, float *ptr7, float *ptr8)
{
    float *out_ptr;

    switch( select )
    {
        case 0:
            out_ptr = ptr0;
            break;
        case 1:
            out_ptr = ptr1;
            break;
        case 2:
            out_ptr = ptr2;
            break;
        case 3:
            out_ptr = ptr3;
            break;
        case 4:
            out_ptr = ptr4;
            break;
        case 5:
            out_ptr = ptr5;
            break;
        case 6:
            out_ptr = ptr6;
            break;
        case 7:
            out_ptr = ptr7;
            break;
        case 8:
            out_ptr = ptr8;
            break;
        default: // point to the zero buffer otherwise:
            out_ptr = Buff[0];
            break;
    }  // switch( select )

    return( out_ptr );
}
