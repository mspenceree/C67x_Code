
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
 * File: algorithms.c
 *
 * This file contains the DSP Block functions that execute the real-time algorithms in the DSP.
 * Several functions (some optional) are included that create, execute and update the parameters
 * for the DSP Blocks as detailed here for a generic algorithm; ALGX:
 *
 *  ALGX_init() -   DSP Block initialization function. Optional function that is used to allocate and
 *  (optional)      initalize any global variables that are used by all instances of this DSP Block 
 *                  (e.g. coefs for an FFT). It is only to be called once before intances of the
 *                  Block are created.
 *
 *  ALGX_exit() -   DSP Block finalization function. Optional function (run once) called to
 *  (optional)      de-allocate any memory alocated by the _init() function before setting up a new
 *                  block diagram.
 *
 *  ALGX_create() - Creates an instance of the DSP Block and returns a pointer to the instance handle
 *                  structure. Also: 1- Memory is allocated for the structure, 2- Memory is allocated
 *                  for parameters (with pointers), 3- Memory is allocated for the state vector and it
 *                  is cleared, 4- Some parameters are loaded with values to make the DSP Block
 *                  functional. These functions are called from the "algCreateBlock.c" file (that is
 *                  called from "main.c").
 *
 *  ALGX_update() - Updates the "dependent" parameters using any "user" parameters that have an influence
 *                  (eg. the filter is re-designed using the filter order and cutoff frequency to give
 *                  the new filter coeficients). There are two copies of all "dependent" parameters;
 *                  one that is being used in the real-time processing loop, ALGX_apply() and the other
 *                  that is written by this update function. After the update is complete the ping-pong
 *                  flag is toggled to swap in the newly computed "dependent" parameters (in one
 *                  instruction). These functions are called from a que in the order that the parameter
 *                  changes are made from the command interpreter.
 *
 *  ALGX_apply() -  Apply or execute the DSP Block's function. This is the real-time part that processes
 *                  input buffer(s) to output buffer(s) and updates any output parameters in the handle
 *                  structure. These functions are called from the main signal processing loop in the
 *                  "algProcessingLoop.c" file.
 *
 *  ALGX_clearState() - Clears the state (or history) portion in the algorithm handle (usually used to
 *                  avoid an audible "pop" when the alorihtm is turned on again). Called in the
 *                  "algProcessingLoop.c" file when a given algorithm is turned off.
 *  
 *  ALGX_delete() - Delete a DSP Block instance, freeing up memory allocated with the _create() call.
 *  (optional)      Use the MEM_free() function.
 *                  (not used at this point because I'm not dynically rebuilding block diagrams.)
 *
 *
 */

// Algorithm header file defines the structures and other definitions.
#include <algorithms.h>
#include <algProcessingTask.h>
#include <main.h>

/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;

extern int IRAM;
extern int SDRAM;

extern float FadeWindow[FRAMESIZE];




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  GAIN functions
//
//  The GAIN DSP Block is a fancy volume control
//
//
pGAIN GAIN_create()
{
    pGAIN returnHdl;

    // Allocate memory for handle and clear contents
    returnHdl = MEM_calloc ( IRAM, sizeof( GAIN_Handle ), 8 );

    if ( !returnHdl ) {
        LOG_printf (&trace, "Algorithm memory allocation failed. Exiting program."); // fixthis. For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }

    // Un-mute:
    returnHdl->mute = 0;
    returnHdl->muteOld = 0;

    // Un-bypass:
    returnHdl->bypass = 0;
    returnHdl->bypassOld = 0;


    // Load up zeros gain with slow ramping:
    returnHdl->dbFlag = 0;
    returnHdl->slowFlag = 1;
    returnHdl->slowFlagOld = 0;

    returnHdl->multiplier =
        returnHdl->multiplierOld =
            returnHdl->gain =       0.0;        // 1.0 for testing. fixed. Normally set to 0.0

    // Set some states:
//  returnHdl->multiplierOld = 1.0;     // same as gain
//  returnHdl->multiplier = 1.0;

    GAIN_update( returnHdl );

    return (returnHdl);
}


void GAIN_clearState( pGAIN GAIN_H )
{
    // There are no state variables in the processing path
    return;
}


int GAIN_update( pGAIN GAIN_H )
{
    float multiplier = GAIN_H->multiplier;   // get local copies of stuff
    float gain = GAIN_H->gain;                  
    int slowFlag = GAIN_H->slowFlag;


    if( slowFlag && !GAIN_H->slowFlagOld )  // "slow" was just turned on
    {
        // Set state of the "slow" filter so that it's output will equal "multiplier"
        // when computed next time in the GAIN_apply() function. This will ramp the
        // gain slowly from the current value to the new value just specified.
        GAIN_H->d0 = ( 1 - GAIN_BLOCK_B0 )*multiplier;
        GAIN_H->d1 = GAIN_H->d0 + ( GAIN_BLOCK_A1 - GAIN_BLOCK_B1 )*multiplier;
    }
    GAIN_H->slowFlagOld = slowFlag;     // update old value

    // Re-compute multiplier:
    if( GAIN_H->dbFlag )    // Gain specified in dB 
    {
        if( gain > 500.0 )
            multiplier = 1.0e25;        // return 10^(500/20)
        else if( gain < -500.0 )
            multiplier = 0.0;           // return zero gain
        else
            multiplier = powf( 10.0, gain/20.0 );   // convert to dB (fixthis: Why does this only work with 'math.h' )
    }
    else                                // Absolute gain is specifed
        multiplier = gain;


    GAIN_H->multiplier = multiplier;    // update the dependent parameter

    return(0);
}


void GAIN_apply( pGAIN GAIN_H, float *in, float *out )
{
    int i;
    float iirOut;

    float d0;
    float d1;

    //Get local copies of stuff:
    int mute = GAIN_H->mute;
    float firstWeight = GAIN_H->multiplierOld;  // The first and last weights are the 
    float lastWeight = GAIN_H->multiplier;      // old and new multiplier values, respectively.

    GAIN_H->multiplierOld = lastWeight; // update the old multiplier state

    if( GAIN_H->slowFlag )  // Compute and use the slow filter if specified
    {
        // Get filter history:
        d0 = GAIN_H->d0;
        d1 = GAIN_H->d1;

        // Compute the "slow" filter (IIR sos in Direct Form II):
        // Filter lastWeight to get iirOut:
        iirOut = GAIN_BLOCK_B0*lastWeight + d0;
        d0 = GAIN_BLOCK_B1*lastWeight - GAIN_BLOCK_A1*iirOut + d1;
        d1 = GAIN_BLOCK_B2*lastWeight - GAIN_BLOCK_A2*iirOut;

        GAIN_H->d0 = d0;    // save the state of the slow filter
        GAIN_H->d1 = d1;

        firstWeight = GAIN_H->iirOutOld;            // The first and last weights are the
        lastWeight = GAIN_H->iirOutOld = iirOut;    // old and new filter outputs, respectively.
    }

    // Fade to/from ZERO if mute changed
    if( mute != GAIN_H->muteOld )
    {
        GAIN_H->muteOld = mute; // update old mute
        if( mute )              // mute just activated:
            lastWeight = 0.0;   // set lastWeight to 0
        else                    // mute just de-activated:
            firstWeight = 0.0;  // set firstWeight to 0
    }

    // Perform a linear weighting of the input frame and copy to output frame:
    lastWeight = ONE_DIV_FRAMESIZE_M1*( lastWeight - firstWeight ); // compute -slope
                                                            // (lastWeight is now a temp var)
                                                            // ONE_DIV_FRAMESIZE_M1 = 1/(FRAMESIZE - 1)
    // Ramp the volume over the frame:
    i = FRAMESIZE_M1;
    do {
        out[i] = ( lastWeight*i + firstWeight )*in[i];
    } while( i-- );

    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  IIR Filter Block functions
//
//  The IIR Filter DSP Block is an Nth order lowpass, highpass, bandpass, or bandstop filter
//  implemented as a cascade of second order sections (sos's). See algorithms.h header file for
//  parameter descriptions.
//
//
pIIR IIR_create()
{
    pIIR returnHdl;

    // Allocate memory for handle and clear contents
    returnHdl = MEM_calloc ( IRAM, sizeof( IIR_Handle ), 8 );

    // Allocate memory for sos filter coefficients (ping) and clear contents
    returnHdl->sos[0] = MEM_calloc (    IRAM,
                                    ( 5*IIR_MAX_NSOS )*sizeof(float),
                                    8 // fixthis check if 4 byte boundry is OK
                                   );

    // Allocate memory for sos filter coefficients (pong) and clear contents
    returnHdl->sos[1] = MEM_calloc (    IRAM,
                                    ( 5*IIR_MAX_NSOS )*sizeof(float),
                                    8
                                   );

    // Allocate memory for state and clear contents
    returnHdl->state = MEM_calloc ( IRAM,
                                    ( 2*IIR_MAX_NSOS )*sizeof(float),
                                    8
                                  );

// Fixthis: we also need to add MEM_calloc's for *z and *p etc.


    if (    !returnHdl ||
            !returnHdl->sos[0] ||
            !returnHdl->sos[1] ||
            !returnHdl->state ) {
        LOG_printf (&trace, "Algorithm memory allocation failed. Exiting program."); // For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }


    // Un-mute:
    returnHdl->mute = 0;
    returnHdl->muteOld = 0;

    // Un-bypass:
    returnHdl->bypass = 0;
    returnHdl->bypassOld = 0;

    // Load up an allpass filter into the sos to make this block functional:
    returnHdl->IIR_Spec = SOS;
    *( returnHdl->sos[0] ) = 1.0;       // set b01 to 1.0
    returnHdl->Nsos[0] = 1;             // one sos section

    // Set pingPong to known state:
    returnHdl->pingPong = 0;

    return (returnHdl);
}


void IIR_clearState( pIIR IIR_H )
{
    // Set the sos delays lines to 0
    memset( IIR_H->state, 0, ( 2*IIR_MAX_NSOS )*sizeof(float) );

    return;
}


int IIR_update( pIIR IIR_H )
{
    int pp;
    int result = 0;
    int Ntrue;

    pp = IIR_H->pingPong ^ 1;   // Get pingPong and toggle

    Ntrue =  IIR_H->Nin;    // get local copy of filter order because it could be increased
                            // by a filter design function (e.g. bandpass will double the order of result)

    switch( IIR_H->IIR_Spec )
    {
        case SOS:
            goto ToggleAndReturn;       // no design is required

        case BUTTER:
            // Design IIR filter and get new filter order, zeros, poles and gain as return values:
            result = butter( SAMPLING_RATE,                                     // Global fs
                             IIR_H->Nin, IIR_H->passType, IIR_H->f1, IIR_H->f2, // Input params
                             &Ntrue, IIR_H->z, IIR_H->p, &(IIR_H->k) ); // zpk params are loaded into sturcture
            break;


        default:    // Error in parameter
            result = -10;   // error code for bad IIR_Spec
            break;

    }

    if( result < 0 )
        return( result );

    // Derive sos parameters based on pole-zero locations:
    zp2sos( Ntrue, IIR_H->z, IIR_H->p, IIR_H->k,    // Inputs are true filter order and zpk's
            IIR_H->sos[pp], IIR_H->Nsos[pp] );      // Output sos params are load into the
                                                    // non-working copy of the "Dependent" params.

    ToggleAndReturn:
    
    // Now that all the paramters are stable, toggling pingPong
    // forces the _apply function in the processing loop to use the just updated params:
    IIR_H->pingPong =  pp;

    return( 0 );    // return successful

}


void IIR_apply( pIIR IIR_H, float *in, float *out )
{
    int i, j;
    float a1, a2, b0, b1, b2, d0, d1;
    float sosIn, sosOut;

    int mute = IIR_H->mute;         // get local copies of some params
    int pp = IIR_H->pingPong;
    int nsos = IIR_H->Nsos[pp];

    float *sosParamPtr = IIR_H->sos[pp];    // Init pointer to coefs
    float *stateParamPtr = IIR_H->state;    // Init pointer to state memory
    float *tempPtr0;
    float *tempPtr1;

    // The second order sections are stored as follows:
    //
    // sos = [ b01 b11 b21   a11 a21 
    //         b02 b12 b22   a12 a22
    //         ...
    //         b0Nsos b1Nsos b2Nsos   a1Nsos a2Nsos ]
    //

    // First pass: process *in to *out
    b0 = *sosParamPtr++;
    b1 = *sosParamPtr++;
    b2 = *sosParamPtr++;

    a1 = *sosParamPtr++;
    a2 = *sosParamPtr++;

    d0 = *(tempPtr0 = stateParamPtr++ );
    d1 = *(tempPtr1 = stateParamPtr++ );

    for( i = 0; i < FRAMESIZE; i++ )
    {
        // Compute the IIR sos in Direct Form II:
        out[i] = sosOut = b0*( sosIn = in[i] ) + d0;
        d0 = b1*sosIn - a1*sosOut + d1;
        d1 = b2*sosIn - a2*sosOut;
    }

    *tempPtr0 = d0; // save the state of this first sos for next call to IIR__apply()
    *tempPtr1 = d1;
    
    // The next (Nsos - 1) passes process *out to *out
    while( --nsos )
    {
        b0 = *sosParamPtr++;
        b1 = *sosParamPtr++;
        b2 = *sosParamPtr++;

        a1 = *sosParamPtr++;
        a2 = *sosParamPtr++;

        d0 = *(tempPtr0 = stateParamPtr++ );
        d1 = *(tempPtr1 = stateParamPtr++ );

        //#pragma UNROLL( 8 );
        //#pragma MUST_ITERATE (16, 1024, 8);
        for( i = 0; i < FRAMESIZE; i++ )
        {
            // Compute the IIR sos in Direct Form II:
            out[i] = sosOut= b0*( sosIn = out[i] ) + d0;
            d0 = b1*sosIn - a1*sosOut + d1;
            d1 = b2*sosIn - a2*sosOut;
        }

        *tempPtr0 = d0; // save the state of each sos for next call to IIR__apply()
        *tempPtr1 = d1;
    }


    // Perform the fade to/from zero operation:
    if( mute != IIR_H->muteOld )    // if mute changed
    {
        IIR_H->muteOld = mute;      // update old mute

        i = FRAMESIZE_M1;
        if( mute )  // Mute just activated; fade to zero
        {
            do {
                out[i] = FadeWindow[i]*out[i];
            } while( i-- );
        }
        else        // Mute just de-activated; fade from zero
        {
            j = 0;
            do {
                out[i--] = FadeWindow[ j++ ]*out[i];
            } while( i );
        }
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FIR Filter Block functions
//
//  The FIR Filter DSP Block is an Nth filter. See algorithms.h header file for
//  parameter descriptions.
//
//
pFIR FIR_create()
{
    pFIR returnHdl;

    // Allocate memory for handle and clear contents
    returnHdl = MEM_calloc ( IRAM, sizeof( FIR_Handle ), 8 );

    // Allocate memory for handle and clear contents
    returnHdl->freqs = MEM_calloc ( IRAM, sizeof( 2*FIR_MAX_FREQS )*sizeof(float), 8 );

    // Allocate memory for handle and clear contents
    returnHdl->mags = MEM_calloc ( IRAM, sizeof( 2*FIR_MAX_FREQS )*sizeof(float), 8 );

    // Allocate memory for handle and clear contents
    returnHdl->weights = MEM_calloc ( IRAM, sizeof( 2*FIR_MAX_WEIGHTS )*sizeof(float), 8 );

    // Allocate memory for FIR filter coefficients (ping) and clear contents
    returnHdl->coefs[0] = MEM_calloc (  IRAM,
                                        ( FIR_MAX_COEFS )*sizeof(float),
                                        8 
                                      );

    // Allocate memory for FIR filter coefficients (pong) and clear contents
    returnHdl->coefs[1] = MEM_calloc (  IRAM,
                                        ( FIR_MAX_COEFS )*sizeof(float),
                                        8
                                       );

    // Allocate memory for state and clear contents
    returnHdl->state = MEM_calloc ( IRAM,
                                    ( FIR_MAX_COEFS - 1 )*sizeof(float),
                                    8
                                  );

    if (    !returnHdl ||
            !returnHdl->freqs ||
            !returnHdl->mags ||
            !returnHdl->weights ||
            !returnHdl->coefs[0] ||
            !returnHdl->coefs[1] ||
            !returnHdl->state ) {
        LOG_printf (&trace, "Algorithm memory allocation failed. Exiting program."); // For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }


    // Un-mute:
    returnHdl->mute = 0;
    returnHdl->muteOld = 0;

    // Un-bypass:
    returnHdl->bypass = 0;
    returnHdl->bypassOld = 0;

    // Load up an allpass filter into the FIR to make this block functional:
    returnHdl->FIR_Spec = COEFS;
    returnHdl->symmetry = 0;            // not symmetric
    returnHdl->k = 0;                   // Set first index into state to 0
    *( returnHdl->state ) = 1.0;        // set w0 to 1.0
    returnHdl->N[0] = 1;                // one tap

    // Set pingPong to known state:
    returnHdl->pingPong = 0;

    return (returnHdl);
}


void FIR_clearState( pFIR FIR_H )
{
    FIR_H->k = 0;       // Set first index into state to 0
            
    // Set the FIR delay line to 0:
    memset( FIR_H->state, 0, ( FIR_MAX_COEFS - 1 )*sizeof(float) );

    return;
}


int FIR_update( pFIR FIR_H )
{
    int pp;
    int result = 0;

    pp = FIR_H->pingPong ^ 1;   // Get pingPong and toggle


    switch( FIR_H->FIR_Spec )
    {
        case COEFS:
            goto ToggleAndReturn;       // no design is required

        case WINDOW:
            // Design FIR filter and get new filter order, zeros, poles and gain as return values:
            //result = butter( SAMPLING_RATE,                                       // Global fs
            //               FIR_H->N, FIR_H->passType, FIR_H->f1, FIR_H->f2,   // Input params
            //               &Ntrue, FIR_H->z, FIR_H->p, &(FIR_H->k) ); // zpk params are loaded into sturcture

            break;

        case WINDOW_ARB:
            // Design FIR filter and get new filter order, zeros, poles and gain as return values:
            //result = butter( SAMPLING_RATE,                                       // Global fs
            //               FIR_H->N, FIR_H->passType, FIR_H->f1, FIR_H->f2,   // Input params
            //               &Ntrue, FIR_H->z, FIR_H->p, &(FIR_H->k) ); // zpk params are loaded into sturcture

            break;

        case EQUIRIPPLE:
            // Design FIR filter and get new filter order, zeros, poles and gain as return values:
            //result = butter( SAMPLING_RATE,                                       // Global fs
            //               FIR_H->N, FIR_H->passType, FIR_H->f1, FIR_H->f2,   // Input params
            //               &Ntrue, FIR_H->z, FIR_H->p, &(FIR_H->k) ); // zpk params are loaded into sturcture

            break;


        default:    // Error in parameter
            result = -10;   // error code for bad FIR_Spec
            break;

    }

    if( result < 0 )
        return( result );



    ToggleAndReturn:
    
    // Now that all the paramters are stable, toggling pingPong
    // forces the _apply function in the processing loop to use the just updated params:
    FIR_H->pingPong =  pp;

    return( 0 );    // return successful

}


void FIR_apply( pFIR FIR_H, float *in, float *out )
{
    int i, j;
    float sum;

    int mute = FIR_H->mute;         // get local copies of some params
    int pp = FIR_H->pingPong;
    int Nfir = FIR_H->N[pp];     // N

    float *coefsParamPtr = FIR_H->coefs[pp];    // Init pointer to coefs
    float *stateParamPtr = FIR_H->state;        // Init pointer to state memory
    int k = FIR_H->k;                           // get local copy of pointer variable, k for speed

    i = 0;
    do{
        sum = 0.0;
        j = Nfir;        
        stateParamPtr[k] = in[i];   // overwrite oldest sample in circular buffer with new sample 
        k = (k + 1)%Nfir;           // update pointer to oldest sample in history state buffer
        
        do{
            sum += coefsParamPtr[--j]*stateParamPtr[k]; // FIR filter
            k = (k + 1)%Nfir;
        } while(j);
        out[i++] = sum;  // load output array
        
    } while(i < FRAMESIZE);
    
    FIR_H->k = k;   // save the pointer into the history state buffer

    
    // Perform the fade to/from zero operation:
    if( mute != FIR_H->muteOld )    // if mute changed
    {
        FIR_H->muteOld = mute;      // update old mute

        i = FRAMESIZE_M1;
        if( mute )  // Mute just activated; fade to zero
        {
            do {
                out[i] = FadeWindow[i]*out[i];
            } while( i-- );
        }
        else        // Mute just de-activated; fade from zero
        {
            j = 0;
            do {
                out[i--] = FadeWindow[ j++ ]*out[i];
            } while( i );
        }
    }

    return;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DRC Block functions
//
//  Dynamic Range Controller/Compressor (DRC). See algorithms.h header file for
//  parameter descriptions.
//
//
pDRC DRC_create()
{
    pDRC returnHdl;

    // Allocate memory for handle and clear contents
    returnHdl = MEM_calloc ( IRAM, sizeof( DRC_Handle ), 8 );

    // Allocate memory for delay line
    returnHdl->delayLine = MEM_calloc ( IRAM, DRC_MAX_DELAY_LINE_LENGTH*sizeof(float), 8 );

    if (    !returnHdl ||
            !returnHdl->delayLine ) {
        LOG_printf (&trace, "Algorithm memory allocation failed. Exiting program."); // For all malacs I should report error to PC and restart DSP
        exit (SYS_EALLOC);
    }


    // Un-mute:
    returnHdl->mute = 0;
    returnHdl->muteOld = 0;

    // Un-bypass:
    returnHdl->bypass = 0;
    returnHdl->bypassOld = 0;

    // Load up a 4-to-1 compressor into the DRC to make this block functional:
    returnHdl->Nzones = 1;
    returnHdl->attackTime = 5e-3;
    returnHdl->decayTime = 25e-3;
    returnHdl->LookAheadFlag = 1;
    returnHdl->detectorType = 1;
    returnHdl->usePeakLimiter = 1;
    returnHdl->X0 = 0.0;
    returnHdl->Y0 = 0.0;
    returnHdl->s0 = 1/4.0;


    // Set pingPong to known state:
    returnHdl->pingPong = 0;

    return (returnHdl);
}


void DRC_clearState( pDRC DRC_H )
{
            
    // Set the DRC delay line to 0:
    memset( DRC_H->delayLine , 0, DRC_MAX_DELAY_LINE_LENGTH*sizeof(float) );

    return;
}


int DRC_update( pDRC DRC_H )
{
    int pp;
    int L;
//  int result = 0;

    float B0, B1, B2, B3;
    float k, r;

    pp = DRC_H->pingPong ^ 1;   // Get pingPong and toggle

    
    if( DRC_H->detectorType == 1 )
        r = 2.0;                        // RMS detector
    else if( DRC_H->detectorType == 2 )
        r = 2.0;                        // RMS detector
    else
        r = 1.0;                        // Power detector


    // Compute parameters used by the compressor:
    k = logf( 10.0 )/20.0;      // k_const = loge( 10.0 )/20.0

    B0 = DRC_H->Y0 - DRC_H->s0 * DRC_H->X0;         // Compute intermediate values
    B1 = ( DRC_H->s0 - DRC_H->s1 ) * DRC_H->X0 + B0;
    B2 = ( DRC_H->s1 - DRC_H->s2 ) * DRC_H->X1 + B1;
    B3 = ( DRC_H->s2 - DRC_H->s3 ) * DRC_H->X2 + B2;


    DRC_H->k_const = k;

    DRC_H->m0p[pp] = DRC_H->s0 - 1.0;       // Compute slopes
    DRC_H->m0pp[pp] = DRC_H->m0p[pp]/r;

    DRC_H->m1p[pp] = DRC_H->s1 - 1.0;
    DRC_H->m1pp[pp] = DRC_H->m1p[pp]/r;

    DRC_H->m2p[pp] = DRC_H->s2 - 1.0;
    DRC_H->m2pp[pp] = DRC_H->m2p[pp]/r;

    DRC_H->m3p[pp] = DRC_H->s3 - 1.0;
    DRC_H->m3pp[pp] = DRC_H->m3p[pp]/r;

    DRC_H->b0[pp] = k * B0;                 // Compute intercepts
    DRC_H->b1[pp] = k * B1;
    DRC_H->b3[pp] = k * B2;
    DRC_H->b4[pp] = k * B3;

    DRC_H->D0p[pp] = k * DRC_H->X0;         // Compute limits
    DRC_H->D0pp[pp] = DRC_H->D0p[pp] * r;

    DRC_H->D1p[pp] = k * DRC_H->X1;
    DRC_H->D1pp[pp] = DRC_H->D1p[pp] * r;

    DRC_H->D2p[pp] = k * DRC_H->X2;
    DRC_H->D2pp[pp] = DRC_H->D2p[pp] * r;


    // Calulate delay line Length:
    L = (int) ( SAMPLING_RATE * DRC_H->attackTime );    // Calculate requested delay line length
    if( L > DRC_MAX_DELAY_LINE_LENGTH ) // if over
        L = DRC_MAX_DELAY_LINE_LENGTH;  // Use max delay length
    else if ( !DRC_H->LookAheadFlag )   // else
        L = 1;                          // only single delay if no Look Ahead

    DRC_H->LdelayLine[pp] = L;          // set delay line length
    DRC_H->logL[pp] = log( (float) L ); // constant used in RMS detector


    // Calculate the attack and decay filter coefs:

    DRC_H->attack_b0[pp] = DRC_ATTACK_B0;   // for now, just load pre-compued values
    DRC_H->attack_b1[pp] = DRC_ATTACK_B1;
    DRC_H->attack_b2[pp] = DRC_ATTACK_B2;
    DRC_H->attack_a1[pp] = DRC_ATTACK_A1;
    DRC_H->attack_a2[pp] = DRC_ATTACK_A2;

    DRC_H->decay_b0[pp] = DRC_DECAY_B0; 
    DRC_H->decay_b1[pp] = DRC_DECAY_B1; 
    DRC_H->decay_b2[pp] = DRC_DECAY_B2; 
    DRC_H->decay_a1[pp] = DRC_DECAY_A1; 
    DRC_H->decay_a2[pp] = DRC_DECAY_A2; 



   //ToggleAndReturn:
    
    // Now that all the paramters are stable, toggle pingPong
    // forces the _apply function in the processing loop to use the just updated params:
    DRC_H->pingPong =  pp;

    return( 0 );    // return successful

}


void DRC_apply( pDRC DRC_H, float *in, float *out )
{
    int i, j;

//  register //let the compiler figure it out

    float sum;

    int mute = DRC_H->mute;         // get local copies of some params
    int pp = DRC_H->pingPong;

//  int Nm1 = DRC_H->N[pp] - 1;     // N-1

//  float *coefsParamPtr = DRC_H->coefs[pp];    // Init pointer to coefs
//  float *stateParamPtr = DRC_H->state;        // Init pointer to state memory
//  int k = DRC_H->k;                           // get local copy of k for speed


//  for( i = 0; i < FRAMESIZE; i++ )

/*
    i = 0;
    do {
        sum = 0.0;
        j = Nm1;
        // Compute the DRC:
        do {
//          sum += coefsParamPtr[j] * stateParamPtr[ k = ( k + 1 ) % Nm1 ];     // sum weighted samples
            sum += coefsParamPtr[j--] * stateParamPtr[ k = ( k + 1 )&0x00FF ];      // sum weighted samples
        } while ( j );

//      out[i] = sum + coefsParamPtr[j] * ( stateParamPtr[ k = ( k + 1 ) % Nm1 ] = in[i] ); // sum last weighted samples, load new sample into history
        out[i++] = sum + coefsParamPtr[j] * ( stateParamPtr[ k = ( k + 1 )&0x00FF ] = in[i] );  // sum last weighted samples, load new sample into history

    } while( i < FRAMESIZE );

    DRC_H->k = k;   // save the pointer into the history state buffer

*/


    // Perform the fade to/from zero operation:
    if( mute != DRC_H->muteOld )    // if mute changed
    {
        DRC_H->muteOld = mute;      // update old mute

        i = FRAMESIZE_M1;
        if( mute )  // Mute just activated; fade to zero
        {
            do {
                out[i] = FadeWindow[i]*out[i];
            } while( i-- );
        }
        else        // Mute just de-activated; fade from zero
        {
            j = 0;
            do {
                out[i--] = FadeWindow[ j++ ]*out[i];
            } while( i );
        }
    }

    return;
}









// end of file




