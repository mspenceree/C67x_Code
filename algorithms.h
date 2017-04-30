
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
 * File: algorithms.h
 *
 * This is the header file for the algorithms.c file and any file that references and algorithm
 * (or DSP-block), handles or parameters.
 *
 * This header defines all algorithm structure definitions and function prototypes.
 *
 */

#ifndef ALGORITHMS_
#define ALGORITHMS_

#define SAMPLING_RATE   44100   // informational only fixthis: use true value




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  GAIN Block definitions
//
//
//  The GAIN Block is a very fancy volume control that ramps a step volume change over the duration
//  of the input frame. See the algorithm for more info.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define GAIN_BLOCK_TYPE_CODE    1
#define GAIN_BLOCK_MAX_INSTANCE 8


// Filter coefs for "slow" gain change:
#define GAIN_BLOCK_B0   4.7179321604354819e-003     // These are the 2nd order IIR filter coefs
#define GAIN_BLOCK_B1   9.4358643208709639e-003     // for Bessel like, fs = 44100/128, f1 = 0.5 =>
#define GAIN_BLOCK_B2   4.7179321604354819e-003     //  ~0.2 sec. response time, very little overshoot
#define GAIN_BLOCK_A1   -1.7574829734897031e+000    // Should have unity gain at DC.
#define GAIN_BLOCK_A2   7.7635470213144497e-001

//  Define the handle structure for parameter values and state pointers for the algorithm:
typedef struct GAIN_Handle
{
// "User Direct" parameters (have NO influence on "Dependent" parameter calculation (i.e. filter coefs) ):
/*1*/   int mute;           // 0= normal, 1= output is muted (Block processing is shutdown)
        int muteOld;        // (for change detect in _apply)

/*2*/   int bypass;         // 0= normal, 1= input is passed to output if not Muted (Block processing is shutdown)
        int bypassOld;      // (for change detect in _apply)

        
// "User Driving" parameters (has influence on "Dependent" parameter calculation (i.e. filter coefs) ):
/*3!*/  int dbFlag;         // dB: 0= "gain" is the multiplier, 1= "gain" specified in dB
/*4!*/  int slowFlag;       // Slow: Ramp gain slowly on change (~1/10 sec. to get to new gain)
        int slowFlagOld;    // (for change detect)

/*5!*/  float gain;         // gain. In absolute terms or in dB, depending on control bit above

// "Dependent" parameters (updated using the _update() function):
/*128*/ float multiplier;   // target gain multiplier

// "User Output" parameters (if any) (these are calculated in the _apply() function in the processing loop):
//float PeakSignalLevel;
        

// These are the state variables:
        float d0;       // for second order filter that is used to ramp the gain slowly (in frame time)
        float d1;
        float multiplierOld;    // used for proportional ramping over the frame
        float iirOutOld;

} GAIN_Handle;


// Type for pointer an instance of algorithm handle
typedef struct GAIN_Handle *pGAIN;


// Prototype functions:
pGAIN GAIN_create();
void GAIN_clearState( pGAIN GAIN_H );
int GAIN_update( pGAIN GAIN_H );
void GAIN_apply( pGAIN GAIN_H, float *inBuf, float *outBuf );



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  IIR Filter block definitions
//
//  The IIR filter is realized by cascaded, Direct Form-II, second order sections (sos's). The sos's
//  may be specified directly or the filter may be designed within the DSP processor. The user may
//  specify the filter in a number of different formats such as poles and zeros, Butterworth with
//  cutoff frequencies, etc.
//
//  The supported filter specification types (or IIR_Spec) are detailed below along with the required
//  filter parameters.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define IIR_BLOCK_TYPE_CODE     2
#define IIR_BLOCK_MAX_INSTANCE  7


#define IIR_MAX_FILTER_ORDER 20 
#define IIR_MAX_NSOS 20 // The actual filter order will 2*N if a BandPass or BandStop filter is specified
                        // (so make space in sos)

#define SOS         0   // IIR_Spec type codes
#define BUTTER      1
#define CHEBYI      2
#define ELLIP       3
#define BESSEL      4
#define LINKWITZ    5
#define BIQUADS     6
#define SHELVING    7
#define ZPK         8

//  Define the handle structure for parameter values and state pointers for the algorithm:
typedef struct IIR_Handle
{
// "User Direct" parameters (have NO influence on "Dependent" parameter calculation (i.e. filter coefs) ):

/*1*/   int mute;           // 0= normal, 1= output is muted (Block processing is shutdown)
        int muteOld;        // (for change detect in _apply)

/*2*/   int bypass;         // 0= normal, 1= input is passed to output if not Muted (Block processing is shutdown)
        int bypassOld;      // (for change detect in _apply)


// "User Driving" parameters (has influence on "Dependent" parameter calculation (i.e. filter coefs) ):

        //float gain;           // overall gain of filter
/*3*/   int IIR_Spec;       //IIR Filter Specification Type               Required parameters (see below)
                            //
                            //  SOS (Second Order Sections)                 N, sosIn
                            //  BUTTER (Butterworth)                        N, passType, f1, f2
                            //  CHEBYI (Chebyshev Type I)                   N, passType, f1, f2, pR
                            //  ELLIP (Elliptic)                            N, passType, f1, f2, pR, sR
                            //  BESSEL (Bessel-Impulse Invariance)          N, passType, f1, f2
                            //  LINKWITZ (Linkwitz-Riley (Butterworth sq.)) N, passType, f1, f2
                            //  BIQUADS (Biquad sos's)                      N, *Qs, *freqs, *gains
                            //  SHELVING (Shelving filter)                  N, Q, g1, g2, f1
                            //  ZPK (Zeros, Poles, gain)                    N, *z, *p, k

/*4*/   int Nin;            // FilterOrder 1 to 20 (in the case of Biquads, Nin is the number of Biquads)
/*5*/   int passType;       // 1= LowPass, 2= HighPass, 3= BandPass (true order is 2*Nin), 4= BandStop (true order is 2*Nin)
/*6*/   float f1;           // Filter cutoff frequency or first band edge in Hz
/*7*/   float f2;           // Second band edge in Hz
/*8*/   float pR;           // Pass band Ripple in dB
/*9*/   float sR;           // Stop band Attenuation in dB
/*10*/  float *Qs;          // pointer to Q or quality factor of Biquad sections (length=Nin)
/*11*/  float *freqs;       // pointer to center frequencies of Biquad sections (length=Nin)
/*12*/  float *gains;       // pointer to gains of Biquad sections in dB (length=Nin)
/*13*/  float Q;            // Q or quality factor of Shelving filter
/*14*/  float g1;           // Gain of left portion of Shelving filter (in dB)
/*15*/  float g2;           // Gain of right portion of Shelving filter (in dB)
/*16*/  float *z;           // zeros of the *z,*p,k (stored: real then imaginary) (length=2*Nin)
/*17*/  float *p;           // poles of the *z,*p,k (stored: real then imaginary) (length=2*Nin)
/*18*/  float k;            // gain of the overall z,p,k
/*19*/  float sosIn;        // pointer to second order sections (see below for definitions) (length=5*Nin)

// "Dependent" parameters (calculated using the _update() function):
/*128*/ int Nsos[2];            // number of sos's ( usually computed to be ceil(Nin/2) ) (length=2)
/*129*/ float *sos[2];          // This array holds the pointers (ping and pong) to the second order section coefs
                                // (size each: 5*Nsos) (length=2*5*Nsos)
                                //
                                // sos = [ b01 b11 b21   a11 a21 
                                //         b02 b12 b22   a12 a22
                                //         ...
                                //         b0Nsos b1Nsos b2Nsos   a1Nsos a2Nsos ]

/*130*/ int pingPong;           // Used to ping-pong the dependent parameters so that one set may be updated
                                // while the other is being used in the _apply() function

// "User Output" parameters (if any) (these are calculated in the _apply() function in the processing loop):
//float PeakSignalLevel;
        

// Pointer to the algorithm's state vector (size 2*Nsos)
        float *state;

} IIR_Handle;


// Type for pointer an instance of algorithm handle
typedef struct IIR_Handle *pIIR;


// Prototype functions:
pIIR IIR_create();
void IIR_clearState( pIIR IIR_H );
int IIR_update( pIIR IIR_H );
void IIR_apply( pIIR IIR_H, float *inBuf, float *outBuf );



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FIR Filter block definitions
//
// The FIR filter is realized by 
//
// The supported filter specification types (or FIR_Spec) are detailed below along with the required
// filter parameters.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define FIR_BLOCK_TYPE_CODE     3
#define FIR_BLOCK_MAX_INSTANCE  4



#define FIR_MAX_COEFS 1025  
#define FIR_MAX_FREQS   20
#define FIR_MAX_WEIGHTS 10
    

#define COEFS       0   // FIR_Spec type codes
#define WINDOW      1
#define WINDOW_ARB  2
#define EQUIRIPPLE  3

//  Define the handle structure for parameter values and state pointers for the algorithm:
typedef struct FIR_Handle
{
// "User Direct" parameters (have NO influence on "Dependent" parameter calculation (i.e. filter coefs) ):

/*1*/   int mute;           // 0= normal, 1= output is muted (Block processing is shutdown)
        int muteOld;        // (for change detect in _apply)

/*2*/   int bypass;         // 0= normal, 1= input is passed to output if not Muted (Block processing is shutdown)
        int bypassOld;      // (for change detect in _apply)


// "User Driving" parameters (has influence on "Dependent" parameter calculation (i.e. filter coefs) ):
        //float gain;           // overall gain of filter
/*3*/   int FIR_Spec;   //FIR Filter Specification Type               Required parameters (see below)
                        //
                        //  COEFS                                       Nin, coefsIn
                        //      Directly specify coefficients
                        //
                        //  WINDOW                                      Nin, passType, f1, f2, windowType
                        //      Window design of standard LP, HP,
                        //      BP, BS filter (Matlab's fir1)
                        //
                        //  WINDOW_ARB                                  Nin, Nfreqs, *freqs, *mags, windowType
                        //      Window design for arbitrary shape
                        //      filter (Matlab's fir2)
                        //
                        //  EQUIRIPPLE                                  Nin, passType, *freqs, *mags, *weights
                        //      Parks-McClellan equiripple LP, HP,
                        //      BP, BS filter (Matlab's firpm)

/*4*/   int Nin;        // Target number of filter taps. Nin = 1 to FIR_MAX_FILTER_ORDER
/*5*/   int Nfreqs;     // Target number of frequencies
/*6*/   int passType;   // 1= LowPass, 2= HighPass, 3= BandPass, 4= BandStop
/*7*/   int windowType; // 1= Hamming, (ignored for now)
/*8*/   float f1;       // Filter cutoff frequency or first band edge in Hz
/*9*/   float f2;       // Second band edge in Hz
/*10*/  float *freqs;   // freqs is a vector of frequency points in the range from 0 to 1, where 1
                        // corresponds to the Nyquist frequency. The first point must be 0 and
                        // the last point 1. The frequency points must be in increasing order. (length=2*Nfreqs)
/*11*/  float *mags;    // mags is a vector containing the desired magnitude response at the points
                        // specified in freqs. freqs and mags must be the same length. Duplicate
                        // frequency points are allowed, corresponding to steps in the frequency response. (length=2*Nfreqs)
                        // For EQUIRIPPLE freqs and mags must be specified in pairs.

/*12*/  float *weights; // band weighting to weight the fit in each frequency band. The length of weights
                        // is half the length of freqs and mags, so there is exactly one weight per band. (length=2*Nfreqs)

/*13*/  float *coefsIn; // pointer to FIR filter coefs (length=Nin)


// "Dependent" parameters (calculated using the _update() function):
/*128*/ int N[2];       // Actual number of filter taps (could be N+1) (ping and pong). N = 1 to FIR_MAX_FILTER_ORDER 
/*129*/ int symmetry;   // Coefficient symmetry. Computed as: 0 = no symmetry exists, 1 = even symmetry (cos), 2 = odd symmetry (sin)
                        // This is used in FIR_apply() to speed up processing.
/*130*/ float *coefs[2];// This array holds the pointers (ping and pong) to the FIR coefs in normal order
                        // (size N) (length=2*Nin)
                        //
                        // coefs = [ w0 w1 w2 ... w(N-1) ] 

/*131*/ int pingPong;   // Used to ping-pong the dependent parameters so that one set may be updated
                        // while the other is being used in the _apply() function

// "User Output" parameters (if any) (these are calculated in the _apply() function in the processing loop):
//float PeakSignalLevel;
        

// State variables:
        int k;          // Index into next state entry (oldest input data)
        float *state;   // Pointer to the algorithm's state vector.
                        // This holds the oldest (N-1) input samples.
                        // (size N-1)

} FIR_Handle;


// Type for pointer an instance of algorithm handle
typedef struct FIR_Handle *pFIR;


// Prototype functions:
pFIR FIR_create();
void FIR_clearState( pFIR FIR_H );
int FIR_update( pFIR FIR_H );
void FIR_apply( pFIR FIR_H, float *inBuf, float *outBuf );




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DRC Block definitions
//
// The is the "Dynamic Range Controller" based on my notes of 10/2007. It compresses or expands the
// input dB signal level using 4 linear zones (such as Limit, Compress, Downward Expand and Noise
// gate).
//
// The number of zones (from 1 to 4), the slopes and the thresholds of each zone may all be set
// by the user. Detectors include Peak, RMS and Power. Attack and decay times may be specified.
//
// A look-ahead buffer is used in the signal chain along with a hold operation to guarantee that no
// output over-load occurs (if using the Peak detector). A Bessel filter is used to filter the
// detected output for smooth (zero first derivative) gain transitions.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define DRC_BLOCK_TYPE_CODE     4
#define DRC_BLOCK_MAX_INSTANCE  4


/*              Dynamic Range Controller Mapping Example

                            XdB (dB in)  --->

                 X2=-80             X1=-20      X0=0
          ________|__________________|___________|______________
         |                                                      |
         |                                     _ *---------->  -|-  Y0 = 0dB
         |                                 _ -      s0=0        |
         |                             _ -                      |
         |                           *     s1=0.25             -|-  -5dB
         |                         /                            |               /|\
         |                      /                               |                |
         |                   /                                  |                |
         |                /  s2=0.5                             |
         |             /                                        |           YdB (dB out)
         |          /                                           |           Ydb = F(Xdb)
         |        *                                            -|-  -35dB
         |       /                                              |
         |      /                                               |
         |     / s3=3                                           |
         |    /                 Compression Ratio, r = 1/s      |
         |   /                                                  |
         |  v                                                   |
         |______________________________________________________|

*/


#define DRC_MAX_DELAY_LINE_LENGTH   882 // Make 20mm max. (= SAMPLING_RATE*(20ms) = 44100*0.02 = 882 )

// Attack: 2nd order Bessel filter coefs for 5msec (200Hz):
//
//   1.9809219697672993e-004,   3.9618439395345986e-004,   1.9809219697672993e-004,   -1.9510475373100584e+000,   9.5183990609796543e-001,
//
#define DRC_ATTACK_B0   1.9809219697672993e-004
#define DRC_ATTACK_B1   3.9618439395345986e-004
#define DRC_ATTACK_B2   1.9809219697672993e-004
#define DRC_ATTACK_A1   -1.9510475373100584e+000
#define DRC_ATTACK_A2   9.5183990609796543e-001

// Decay: 2nd order Bessel filter coefs for 25msec (40Hz):
//
//   8.0798480466110184e-006,   1.6159696093222037e-005,   8.0798480466110184e-006,   -1.9901451957876028e+000,   9.9017751517978925e-001,
//
#define DRC_DECAY_B0    8.0798480466110184e-006
#define DRC_DECAY_B1    1.6159696093222037e-005
#define DRC_DECAY_B2    8.0798480466110184e-006
#define DRC_DECAY_A1    -1.9901451957876028e+000
#define DRC_DECAY_A2    9.9017751517978925e-001

//  Define the handle structure for parameter values and state pointers for the algorithm:
typedef struct DRC_Handle
{
// "User Direct" parameters (have NO influence on "Dependent" parameter calculation (i.e. filter coefs) ):
/*1!*/  int mute;           // 0= normal, 1= output is muted (Block processing is shutdown)
        int muteOld;        // (for change detect in _apply)

/*2!*/  int bypass;         // 0= normal, 1= input is passed to output if not Muted (Block processing is shutdown)
        int bypassOld;      // (for change detect in _apply)


// "User Driving" parameters (has influence on "Dependent" parameter calculation (i.e. filter coefs) ):
/*3!*/  int Nzones;         // Number of line segments that make up compressor (1 to 4)
/*4!*/  float attackTime;   // Attack time in seconds for detector filter. Also, sets the look-ahead delay to the same.
                            // (maximum value ~20ms for delay line lenth: DRC_MAX_DELAY_LINE_LENGTH)
/*5!*/  float decayTime;    // Decay time in seconds for the detector filter (should not be less than attackTime
                            // to avoid possible gain distortion on decay).
/*6!*/  int LookAheadFlag;  // 1= look-ahead delay line used (Delay line set to "attackTime"; up to 20ms)
                            // 0= look-ahead delay not used (use to minimize signal latency).
/*7!*/  int detectorType;   // 1= Peak, 2= RMS, 3= Power
/*8!*/  int usePeakLimiter; // 1= the Peak detector is used for signals in the Limiting zone (independent of "detectorType")
                            // 0= the specified detectorType is used in all zones
/*9!*/  float X0;           // XdB threshold value of zone 0 (limiting zone). See figure above. (in dB)
/*10!*/ float X1;           // XdB threshold value of zone 1. See figure above. (in dB)
/*11!*/ float X2;           // XdB threshold value of zone 2. See figure above. (in dB)
/*12!*/ float Y0;           // YdB output level at X0 input level. (in dB)
/*13!*/ float s0;           // slope of limiting region. (Compression ratio = r = 1/s)
/*14!*/ float s1;           // slope of region 1.
/*15!*/ float s2;           // slope of region 2.
/*16!*/ float s3;           // slope of region 3 (use a very large number for a noise gate. eg. 1e20)


// "Dependent" parameters (updated using the _update() function):
/*128*/ float k_const;      // constant: = log( 10.0 )/20.0

/*129*/ int LdelayLine[2];  // Length of delay line. Calculated as the lessor of SAMPLING_RATE * attackTime
                            // or DRC_MAX_DELAY_LINE_LENGTH

/**/    float logL[2];      //  = log( LdelayLine )

/*128*/ float m0p[2];       // Slopes (see DRC Notes 10/2007)
/*128*/ float m1p[2];
/*128*/ float m2p[2];
/*128*/ float m3p[2];
/*128*/ float m0pp[2];
/*128*/ float m1pp[2];
/*128*/ float m2pp[2];
/*128*/ float m3pp[2];
/**/    float b0[2];        // Intercepts
/**/    float b1[2];
/**/    float b3[2];
/**/    float b4[2];
/**/    float D0p[2];       // Limits
/**/    float D1p[2];
/**/    float D2p[2];
/**/    float D0pp[2];
/**/    float D1pp[2];
/**/    float D2pp[2];

/**/    float attack_b0[2]; // attack filter parameters
/**/    float attack_b1[2];
/**/    float attack_b2[2];
/**/    float attack_a1[2];
/**/    float attack_a2[2];

/**/    float decay_b0[2];  // decay filter parameters 
/**/    float decay_b1[2]; 
/**/    float decay_b2[2]; 
/**/    float decay_a1[2]; 
/**/    float decay_a2[2]; 


/*13*/  int pingPong;   // Used to ping-pong the dependent parameters so that one set may be updated
                        // while the other is being used in the _apply() function

// "User Output" parameters (if any) (these are calculated in the _apply() function in the processing loop):
        //float PeakSignalLevel;
        

// These are the state variables:
        float d0;               // for second order detector filter state
        float d1;
        float *delayLine;       // for look-ahead delay (size: DRC_MAX_DELAY_LINE_LENGTH )
        float *delayLinePeak;   // for Peak detector (size: DRC_MAX_DELAY_LINE_LENGTH )
        float *delayLineRMS;    // for RMS detector (size: DRC_MAX_DELAY_LINE_LENGTH )

//      float multiplierOld;    // used for proportional ramping over the frame
//      float iirOutOld;
    
} DRC_Handle;


// Type for pointer an instance of algorithm handle
typedef struct DRC_Handle *pDRC;


// Prototype functions:
pDRC DRC_create();
void DRC_clearState( pDRC DRC_H );
int DRC_update( pDRC DRC_H );
void DRC_apply( pDRC DRC_H, float *inBuf, float *outBuf );




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DEFT Block definitions
//
//
//  The DEFT Block uses the iso9613_1 standard to get air attenuation and
//  then computes an air frequency response filter, h_air and an air
//  response-corrrection filter, h_inv. These filters are placed
//  in the signal path if not "bypassed"
//
//  The inverse filter approximates inverse-air response from DC to the
//  smaller of: maxFreq or the frequency where "maxCorrection_dB" occurs
//  at the given "distance".
//
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define DEFT_BLOCK_TYPE_CODE    5
#define DEFT_BLOCK_MAX_INSTANCE 2

//  Define the handle structure for parameter values and state pointers for the algorithm:
typedef struct DEFT_Handle
{
// "User Direct" parameters (have NO influence on "Dependent" parameter calculation (i.e. filter coefs) ):
/*1!*/  int mute;           // 0= normal, 1= output is muted (Block processing is shutdown)
        int muteOld;        // (for change detect in _apply)

/*2!*/  int bypass;         // 0= normal, 1= input is passed to output if not Muted (Block processing is shutdown)
        int bypassOld;      // (for change detect in _apply)


// "User Driving" parameters (has influence on "Dependent" parameter calculation (i.e. filter coefs) ):
/*3*/   int     N_air_in;       // h_air filter order (for simulated air response)
/*4*/   int     N_inv_in;       // h_inv filter order

/*5!*/  int     SimulateFlag;   // 0= Normal DEFT operation, 1= Test mode: Inserts an "Air Response" filter in signal path

/*6!*/  int     relhFlag;       // 0= Use measured relh (from sensor)
                                // 1= Use the relh below if sensor data not available
                                // 2= Always use the relh below
/*7!*/  float   relh;           // Relative humidity in percent (0 to 100.0)

/*8!*/  int     tempcFlag;      // 0= Use measured tempc (from sensor)
                                // 1= Use the tempc below if sensor data not available
                                // 2= Always use the tempc below
/*9!*/  float   tempc;          // Air temperature in deg. C

/*10!*/ int     distanceFlag;   // 0= Use measured distance (from range finder)
                                // 1= Use the distance below if range finder data not available
                                // 2= Always use the distance below
/*11!*/ float   distance;       // Distance to target in meters

/*12!*/ float   TimeConst;      // DEFT input data (relh, tempc and distance) is filtered with an RC-type filter.
                                // "TimeCosnt" specifies the filter's time constant in seconds.
                                // Use 0.0 for instantaneous
                                // (My note: the DEFT parameters are updated at the frame rate)

/*13!*/ float   correctionPercent;  // Percentage correction (0-none, 100-Full DEFT)
/*14!*/ float   maxCorrection_dB;   // Peak target gain of of inverse filter (in dB)
/*15!*/ float   unityGainFreq;  // Frequency where the inverse filter has unity
                                // gian. Could be called the piviot frequency.
/*16!*/ float   maxFreq;        // Cutoff frequency of system. Should be set to
                                // the same frequency as the (talk/listen
                                // Lowpass filter). The frequency that the h_inv
                                // filter boost response flattens out is the
                                // smaller of: maxFreq or freq of max Correction.

/*17*/  float   relpres;        // Relative pressure (sea level = 1.0)


// "Dependent" parameters (calculated using the _update() function):
/*128*/ int     Nfreqs;         // Number of frequencies used to compute the air response
/*129*/ float   *freqs;         // Frequencies used to compute the air response (length=Nfreqs)
/*130*/ float   *airResponse_dB; // Air response in dB at the frequencies in freqs

/*131*/ int     N_air[2];   // Number of filter taps (ping and pong). N = 1 to FIR_MAX_FILTER_ORDER  (length=N_air_in)
/*132*/ float   *h_air[2];  // This array holds the pointers (ping and pong) to the Linear phase FIR
                            // filter that approximates the air response.
                            // (size N_air_in) (length=2*N_air_in)
                            //
                            // coefs = [ w0 w1 w2 ... w(N-1) ] 

/*133*/ int     N_inv[2];   // Number of filter taps (ping and pong). N = 1 to FIR_MAX_FILTER_ORDER  (length=N_inv_in)
/*134*/ float   *h_inv[2];  // This array holds the pointers (ping and pong) to the Linear phase FIR
                            // filter that approximates the air response.
                            // (size N_inv_in) (length=2*N_inv_in)
                            //
                            // coefs = [ w0 w1 w2 ... w(N-1) ] 

/*135*/ int pingPong;       // Used to ping-pong the dependent parameters so that one set may be updated
                            // while the other is being used in the _apply() function

// "User Output" parameters (if any) (these are calculated in the _apply() function in the processing loop):
        //float PeakSignalLevel;
        

        // State variables:
        int airk;           // Index into next state entry (oldest input data)
        float *airState;    // Pointer to the algorithm's state vector.
                            // This holds the oldest (N_air_in - 1) input samples.
                            // (size N_air_in - 1)

        int invk;           // Index into next state entry (oldest input data)
        float *invState;    // Pointer to the algorithm's state vector.
                            // This holds the oldest (N_inv_in - 1) input samples.
                            // (size N_inv_in - 1)
} DEFT_Handle;


// Type for pointer an instance of algorithm handle
typedef struct DEFT_Handle *pDEFT;


// Prototype functions:
pDEFT DEFT_create();
void DEFT_clearState( pDEFT DEFT_H );
int DEFT_update( pDEFT DEFT_H );
void DEFT_apply( pDEFT DEFT_H, float *inBuf, float *outBuf );




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  AFC Block definitions
//
//
//  The AFC Block
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define AFC_BLOCK_TYPE_CODE     6
#define AFC_BLOCK_MAX_INSTANCE  1




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DRE Block definitions
//
//
//  The DRE Block 
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define DRE_BLOCK_TYPE_CODE     7
#define DRE_BLOCK_MAX_INSTANCE  1




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ANC Block definitions
//
//
//  The ANC Block 
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define ANC_BLOCK_TYPE_CODE     8
#define ANC_BLOCK_MAX_INSTANCE  1




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  TEST Block definitions
//
//
//  The TEST Block 
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define TEST_BLOCK_TYPE_CODE    9
#define TEST_BLOCK_MAX_INSTANCE 1















///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  CONTROLLER Block definitions
//
//
//  The CONTROLLER Block is used by the pollUsbTask to change the DSP_Mode and invoke other commands.
//  These tasks are performed in the "pollUsbTask" function.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define CONTROLLER_BLOCK_TYPE_CODE      0
#define CONTROLLER_BLOCK_MAX_INSTANCE   1



/*1!*/  // int DSP_Mode     // Set the DSP's operating mode as follows:
                            // 0= STANDBY - Disables all the DSP signal processing blocks and
                            //              deactivates the audio Power Amplifiers using the
                            //              shutdown logic signal.
                            // 1= LISTEN  - Configures the system into listen back mode as
                            //              shown in the "".
                            // 2= TALK    - Configures the system into talk (or audio playback)
                            //              mode as shown in the "
                            //              Diagrams".
                            //
                            // See Note 2, below in the DIGITAL_IN Block definitions section
                            // for behavior when the Mic Push-to-Talk button is pushed.
                            // However, any DSP_Mode changes will be queued until after PTT is
                            // released.
                            
/*2*/   // int command;     // Execute a command as follows (write only):
                            // 1= RESET_DSP         - Perform warm restart of DSP
                            // 99= FACTORY_RESET    - Write factory default parameters to DSP and
                            //                        then do warm restart of DSP.




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  SELECTOR Block definitions
//
//  The SELECTOR Block switches to the desired input signal. See ""
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define SELECTOR_BLOCK_TYPE_CODE        100
#define SELECTOR_BLOCK_MAX_INSTANCE     6

/*1!*/  // int address      // 0 to 8

// The Global Variables related to the instances are:

//  BlockInstance   Global Control Variable
//  1               int AsacsInputSelect;
//  2               int AsacsNoiseSourceSelect;
//  3               int PCLineInLSelect;
//  4               int PCLineInRSelect;
//  5               int AuxOut1Select;
//  6               int AuxOut2Select;



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  HEADPHONE_SWITCH Block definitions
//
//  The HEADPHONE_SWITCH Block select the desired monitor point for the headphone jack.
//  See ""
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define HEADPHONE_SWITCH_BLOCK_TYPE_CODE        101
#define HEADPHONE_SWITCH_BLOCK_MAX_INSTANCE     1

// Read only (for now)
/*1*/   // int addrA67      //  0= Monitor PCLineIn (this is DSPOut6 and 7) (default)
                            //  1= Monitor AuxOut (this is DSPOut4 and 5)
                            //  2= Monitor PCLineOut (Listen to your MP3 music anybody?)
                            //  3= Monitor CompressorOut signal on Interface Board

// To change call:
// analogMuxHeadphoneSelect(int addrA67)



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DIGITAL_IN Block definitions
//
//  The DIGITAL_IN Block reads the switch states. Bit assignments are below.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define DIGITAL_IN_BLOCK_TYPE_CODE      102
#define DIGITAL_IN_BLOCK_MAX_INSTANCE   1

// Global Variable access  (read only by PC):
/*1!*/  // unsigned int         SwitchState

/*

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

*/




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  DIGITAL_OUT Block definitions
//
//  The DIGITAL_OUT Block reads the switch states. Bit assignments are below.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define DIGITAL_OUT_BLOCK_TYPE_CODE     103
#define DIGITAL_OUT_BLOCK_MAX_INSTANCE  1

// Global Variable access (read only by PC):
/*1*/   // unsigned char int    DigitalOut_Copy




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  TEMPERATURE Block definitions
//
//  The TEMPERATURE Block returns the Temperature from the Rense (www.rense.nl) Model HT-923
//  Temperature/Humidity Probe. 
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define TEMPERATURE_BLOCK_TYPE_CODE     104
#define TEMPERATURE_BLOCK_MAX_INSTANCE  1

// Global Variable access (read only by PC):
/*1!*/  //  float   Temperature         //  in deg. C, -20 to 100 C calibrated, can possibly read -100 to 200 C
/*2!*/  //  int     TemperatureOK       //  0-reading invalid, 1-reading valid (readings are flagged invalid if not updated for 5 seconds)



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  TEMPERATURE_DEW Block definitions
//
//  The TEMPERATURE_DEW Block returns the Dew point Temperature from the Rense (www.rense.nl) Model HT-923
//  Temperature/Humidity Probe. 
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define TEMPERATURE_DEW_BLOCK_TYPE_CODE     105
#define TEMPERATURE_DEW_BLOCK_MAX_INSTANCE  1

// Global Variable access (read only by PC):
/*1*/   //  float   TemperatureDew          //  in deg. C, This is the temperature of an object that will condense moisture
/*2*/   //  int     TemperatureDewOK        //  0-reading invalid, 1-reading valid (readings are flagged invalid if not updated for 5 seconds)



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  HUMIDITY Block definitions
//
//  The HUMIDITY Block returns the Humidity from the Rense (www.rense.nl) Model HT-923
//  Temperature/Hunidity Probe. 
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define HUMIDITY_BLOCK_TYPE_CODE        106
#define HUMIDITY_BLOCK_MAX_INSTANCE     1

// Global Variable access (read only by PC):
/*1!*/  //  float   RelativeHumidity     // in %, 1 to 95% calibrated, can read from 0.0 to 100%.
/*2!*/  //  int     RelativeHumidityOK   // 0-reading invalid, 1-reading valid (readings are flagged invalid if not updated for 5 seconds)



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  RANGE Block definitions
//
//  The RANGE Block returns the range from the model: xxx laser range finder.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define RANGE_BLOCK_TYPE_CODE       107
#define RANGE_BLOCK_MAX_INSTANCE    1

// Global Variable access (read only by PC):
/*1!*/  //  float   Range       // in meters, xx to xxx calibrated, can read from xx to xxx (TBD)
/*2!*/  //  int     RangeOK     // 0-reading invalid, 1-reading valid



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ADC Block definitions
//
//  This task reads the first three 8-bit values from the ADS7830 analog to digital converter.
//  The input signals are available on the MIDI port. The global variable below are updated
//  approximately every 50 msec.
//
//
//  The #define below specifies the "BlockTypeCode" for this Block and is used by the command
//  interpreter and for USB communications with the PC.
//
//  The numbers shown below as /*1*/, /*2*/, ... specify the "ParameterCode" for the Block.

#define ADC_BLOCK_TYPE_CODE     108
#define ADC_BLOCK_MAX_INSTANCE  1

// Global Variable access (read only by PC):
/*1*/   //  float   AuxAnalogIn1    // A/D input value from 0.0 to 255.0 (pin 4 of MIDI connector)
/*2*/   //  float   AuxAnalogIn2    // A/D input value from 0.0 to 255.0 (pin 9 of MIDI connector)
/*3*/   //  float   AuxAnalogIn3    // A/D input value from 0.0 to 255.0 (pin 5 of MIDI connector)
/*4*/   //  int     AuxAnalogInOK   // 0-readings invalid, 1-readings valid





























#endif  /* ALGORITHMS_ */

