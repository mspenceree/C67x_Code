
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
 * File: filterDesign.c
 *
 * This file contains all the IIR and FIR filter design functions and support functions.
 *
 */


//#include <algorithms.h>
//#include <algCreateBlocks.h>
#include <main.h>


int butter( float fs, int N, int passType, float f1, float f2,
            int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;

    // check parameters:


    return( result );
}


int chebyI( float fs, int N, int passType, float f1, float f2, float pR,
            int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;


    return( result );
}



int ellip(  float fs, int N, int passType, float f1, float f2, float pR, float sR,
            int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;


    return( result );
}



int bessel( float fs, int N, int passType, float f1, float f2,
            int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;


    return( result );
}



int linkwitz( float fs, int N, int passType, float f1, float f2,
              int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;


    return( result );
}



int biquads(  float fs, int Nb, float *Qs, float *freqs, float *gains,
              int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;


    return( result );
}



int shelving( float fs, int N, float Q, float g1, float g2, float f1,
              int *Ntrue, float *z, float *p, float *k )
{
    int result = -1;


    return( result );
}


// My function to convert poles and zeros to best sos's:
void zp2sos( int N, float *z, float *p, float k,        // Inputs are true filter order and zpk's
            float *sos, int *Nsos )                 // Outputs are sos params and number of sos's.
{


    return;
}


