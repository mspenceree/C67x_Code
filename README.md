# C67x_Code

Sample DSP C-code for frame-based IIR and FIR digital filters.

This is an example that exhibits one of my embedded software projects. It runs on Lyrtech Inc PADK DSP board, a commercial TMS320C672x based board. The code uses TI’s DSP/BIOS Real-time operating system (RTOS). Tool versions used: CCStudio v3.3 and DSP/BIOS v 5_31_02

### Filter code functions from [`algorithms.c`](algorithms.c "algorithms.c"):

This IIR filter code snip shows the function that implements a cascade of 2nd-order sections in Direct Form II. It processes and returns `FRAMESIZE` new input samples per call.
```c++
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
```


This FIR filter code uses a circular buffer to avoid having to explicitly shift the data in the sample history buffer. It processes and returns `FRAMESIZE` new input samples per call.
```c++
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
```


### No License is offered for this software at this time.
