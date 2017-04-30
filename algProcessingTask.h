
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
 * File: algProcessingTask.h
 *
 * dMAX McASP driver (DMD) header file 
 * demontrates the dMAX McASP driver usage
 *
 * This is a modificaion of the 'echo' demo originally from Texas Instruments, Inc.
 *
 *
 */

#ifndef ALGPROCESSING_TASK_
#define ALGPROCESSING_TASK_

/* CSL header files */
#include <soc.h>
#include <csl.h>
#include <csl_chip.h>
#include <csl_dmax.h>
#include <csl_mcasp.h>

/* BIOS header files */
#include <std.h>
#include <tsk.h>
#include <dev.h>
#include <hwi.h>
#include <log.h>
#include <mem.h>
#include <que.h>
#include <sem.h>
#include <sys.h>
#include <sio.h>
#include <c62.h>

/* 
    The c672x_dmax_mcasp.h file defines the structures and 
    definitions that are required by the DMD driver.
*/
#include <c672x_dmax_mcasp.h>

/* XDAIS header file */
#include <ialg.h>
#include <xdas.h>

/* PADK H/W specific header file */
#include <board_padk.h>

/* Maximum channels that are used
   8 */
#define IO_MAX_CHAN 8

// Maximum number of intermediate buffers:
#define BUFF_MAX    4
#define BUFF_MAX_P1 5   // BUFF_MAX + 1

/* 
    Frame size used when receiving data from the McASP.
    After the frameSize amount of data is received, the data
    is converted to floating point. The data is then converted
    back to fixed point and provided to McASP for processingLoop.
    The fixed->float and float->fixed steps are performed
    to demonstrate how the data can be modified in-between
    McASP RX and McASP TX steps. 

    MSpencer - Use: 64. 512 is faster, 1024 fails
*/
//#define FRAMESIZE                 128
//#define FRAMESIZE_M1          127
//#define FRAMESIZE_P1          129
//#define ONE_DIV_FRAMESIZE_M1  7.87401575e-3   //  1.0/(FRAMESIZE - 1)

#define FRAMESIZE               256
#define FRAMESIZE_M1            255
#define FRAMESIZE_P1            257
#define ONE_DIV_FRAMESIZE_M1    3.92156863e-3   //  1.0/(FRAMESIZE - 1)

//#define FRAMESIZE_M1          FRAMESIZE - 1   // this does not work
//#define FRAMESIZE_P1          FRAMESIZE + 1
//#define ONE_DIV_FRAMESIZE_M1  1.0/(FRAMESIZE - 1)

/*
    IOParams structure defines the parameters that are
    used to initialize the code. The default structure is
    defined in the source file, algProcessingTaskParams.c. The default
    structure is provided to the algProcessingTask as TSK argument. 
*/
typedef struct IOParams
{
    Int maxIPChannels; /* Maximum input channels */
    Int maxOPChannels; /* Maximum output channels */
    Int maxFrameChannels; /* Maximum working channels */

    /* 
        Precision of input and output data.
        This information is used when converting input data
        to floating point and when converting the work
        buffer floating data to output buffer fixed point data.
    */
    Int ipPrecision; 
    Int opPrecision;

    /*
        Number of buffers issued to the DMD driver.
        Please note DMD driver can accept at max 3 buffers
    */
    Int numDrvBuffers;

} IOParams;

/*
    IOstreamHdl structure maintains the variables used by
    the algProcessingTask.c at runtime. These include the pointers to
    various buffers and other configuration information.
*/
typedef struct IOstreamHdl
{
    Int internalMem; /* Internal memory segment */
    Int externalMem; /* External memory segment */

    /*
        FrameBuffer (working buffer) array holds the pointers
        to various audio channels.
    */
    // Removed by MSpencer. I will be handling frame buffers directly.
    //Float * frameBuff [IO_MAX_CHAN];

    /*
        ipDRVBuff array holds the pointers to the
        buffers used to receive data from McASP 
    */
    Void * ipDrvBuff[DMD_MAX_BUFF];
    /*
        opDRVBuff array holds the pointers to the
        buffers used to transmit data from McASP 
    */
    Void * opDrvBuff[DMD_MAX_BUFF];

    PADK_brdHdl brdHdl; /* PADK board instance handle */
    IOParams * pParams; /* Pointer to default params provided */
    
    /*
        To perform processing, there are two instances of the 
        driver created. The first instance interfaces with the
        McASP to receive data. The second instance interfaces with
        the McASP to transmit data.
        For each instance of the DMD driver, a seperate SIO
        object is created. sioIP for input instance and
        sioOP for the o/p instance of the driver 
    */
    SIO_Handle sioIP;
    SIO_Handle sioOP;

} IOstreamHdl;


typedef struct IOstreamHdl *pIOstreamHdl;


/*
    DMD driver accepts a parameter structure to initialize
    the peripherals (McASP and dMAX). 
    The MultiChannelAnalogIP is used to initialize the RX section.
      8 channel analog input using AXR0[0], AXR0[1], AXR0[2] and AXR0[3]
    The MultiChannelAnalogOP is used to initialize the TX section.
     8 channel analog output using AXR0[4], AXR0[5], AXR0[6] and AXR0[10]
*/
extern const DMD_Params MultiChannelAnalogIP;
extern const DMD_Params MultiChannelAnalogOP;

#define MVAL32BIT 2147483647.0f    /* 0x7fffffff, used for float<->fixed */


#define ch1 0   // Use to unscramble the input and output channels:
#define ch2 4   // e.g. inBuff(ch3)
#define ch3 1
#define ch4 5
#define ch5 2
#define ch6 6
#define ch7 3
#define ch8 7

// For algProcessing Loop:
#define DSP_OP_STOP     4
#define DSP_OP_START    3
#define DSP_OP_RUNNING  2
#define DSP_OP_STOPPING 1
#define DSP_OP_STOPPED  0


#endif  /* ALGPROCESSING_TASK_ */
