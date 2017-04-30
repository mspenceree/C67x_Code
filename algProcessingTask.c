
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
 * File: algProcessingTask.c
 *
 * This is the dMAX McASP driver (DMD) algProcessingTask source file.
 * This file is a modified version form the processing task from "echo" demo.
 *
 * This Task sets up the A/D and D/A pipes and drops into the processing loop.
 *
 *
 */

#include <algProcessingTask.h>
//#include <algorithms.h>
#include<main.h>

/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;

/* 
    Memory segments defined in the asacs.tcf file.
    IRAM  -> DSP internal RAM
    SDRAM -> External RAM
*/
extern Int IRAM;
extern Int SDRAM;

Float *inBuff[ IO_MAX_CHAN ];
Float *outBuff[ IO_MAX_CHAN ];
Float *Buff[ BUFF_MAX_P1 ];


/*
    The algProcessingTask demonstrates the usge of DMD driver to interface
    with the McASP peripheral.
    The task takes as an argument a parameter structure (IOParams).
    The parameter structure is defined in the header file: algProcessingTask.h
    The default parameter structure is defined in the source
    file algProcessingTaskParams.c (IOParams). The default parameter structure
    is provided to the algProcessingTask using the configuration in asacs.tcf
*/
Void algProcessingTask (IOParams * pParams)
{

    /* 
        The algProcessingTask handle is used to maintain variables and 
        configurations. This handle is created in the task and 
        provided to various functions to share information
    */
    pIOstreamHdl IOhdl;

    /* SIO Attrs to create SIO objects for McASP RX and TX */
    SIO_Attrs sioAttrs;
    Int32 i, error;

    /*
        DMD driver accepts a parameter structure to initialize
        the peripherals (McASP and dMAX). The parameter structure
        is defined in the header file: c672x_dmax_mcasp.h 
        The code reads the specified DMD parameter structure into the
        local dmd variable and provides it to the DMD driver for configuration
    */
    DMD_Params dmd;


    LOG_printf(&trace, "Entered algProcessingTask.c");


    /**********************************************************/
    /* Create and initialize the IO Stream handle                  */
    /**********************************************************/
    IOhdl = MEM_calloc (IRAM, sizeof(IOstreamHdl), 8);

    /* Store the pointer to the provided task parameters */
    IOhdl->pParams = pParams;

    /* Initialize the variables */
    IOhdl->externalMem = SDRAM; /* Pointer to external memory segment */
    IOhdl->internalMem = IRAM;  /* Pointer to internal memory segment */

    /**********************************************************/
    /* Allocate IP/OP driver buffers                          */
    /**********************************************************/
    for (i = 0; i < pParams->numDrvBuffers; i++) {
        /* Allocate the I/P and O/P buffers */
        IOhdl->ipDrvBuff[i] = MEM_calloc (
                                            IOhdl->internalMem, 
                                            pParams->maxIPChannels*
                                                      sizeof(Float)*
                                                      FRAMESIZE, 
                                             8
                                         );
        IOhdl->opDrvBuff[i] = MEM_calloc (
                                            IOhdl->internalMem, 
                                            pParams->maxOPChannels*
                                                      sizeof(Float)*
                                                      FRAMESIZE, 
                                             8
                                         );
        if (!IOhdl->ipDrvBuff[i] || !IOhdl->opDrvBuff[i]) {
            LOG_printf (&trace, "Driver buffer allocation failed. Exiting program.");   // fixthis
            exit (SYS_EALLOC);
        }
    }

    /**********************************************************/
    /* Allocate the frame buffers                             */
    /**********************************************************/
    inBuff[0] = MEM_calloc (
                                IRAM, 
                                sizeof(Float)*FRAMESIZE*pParams->maxFrameChannels, 
                                8
                               );
    outBuff[0] = MEM_calloc (
                                IRAM, 
                                sizeof(Float)*FRAMESIZE*pParams->maxFrameChannels, 
                                8
                               );
    Buff[0] = MEM_calloc (
                                IRAM, 
                                sizeof(Float)*FRAMESIZE*BUFF_MAX, 
                                8
                               );
    if ( !inBuff[0] || !outBuff[0] || !Buff[0] ) {
        LOG_printf (&trace, "Frame buffer allocation failed. Exiting program.");
        exit (SYS_EALLOC);
    }

    /* Assign pointers to various channel frame buffers */
    for (i = 1; i<pParams->maxFrameChannels; i++)
    {
        inBuff[i] = inBuff[i - 1] + FRAMESIZE;
        outBuff[i] = outBuff[i - 1] + FRAMESIZE;
    }

    for (i = 1; i < BUFF_MAX_P1; i++) {
        Buff[i] = Buff[i - 1] + FRAMESIZE;
    }
    
    /**********************************************************/
    /* Create the PADK H/W object                             */
    /**********************************************************/             
    /* 
        The PADK_board_open function initializes the PADK H/W.
        It returns with the board handle that can be used to 
        control the PADK H/W
    */
    IOhdl->brdHdl = PADK_board_open (IOhdl->internalMem);
    
    if (!IOhdl->brdHdl) {
        LOG_printf (&trace, "Board handle creation failed. Exiting program.");
        exit (SYS_EALLOC);
    }

    /* Call the PADK board control function to Mute the o/p */
    IOhdl->brdHdl->brdCtrl (IOhdl->brdHdl, PADK_MUTE_OUTPUT);

    // Added by MSpencer:
//  CLKGEN_OscSel(OSC_24_576MHz);
//  CLKGEN_OscSel(OSC_22_5792MHz);

    /**********************************************************/
    /* Initialize the sioAttrs to be used when creating the   */
    /* SIO objects for RX and TX                              */
    /**********************************************************/             
    sioAttrs.nbufs = 3; /* Maximum buffers that can be issued to DMD */
    sioAttrs.segid = IOhdl->internalMem; 
    sioAttrs.align = 8;

    /*
        Indicates the desired behavior for an output stream when it is deleted.
        If flush is TRUE, a call to SIO_delete causes the stream to discard all
        pending data and return without blocking. 
    */
    sioAttrs.flush = TRUE;
    sioAttrs.model = SIO_ISSUERECLAIM; /* SIO Usage model */

    /*
        Specifies the length of time the device driver waits for I/O completion
        before returning an error (for example, SYS_ETIMEOUT). timeout is 
        usually passed as a parameter to SEM_pend by the device driver.
        If the timeout expires before a buffer is available to be returned, 
        the I/O operation returns the value of (-1 * SYS_ETIMEOUT). 
        Otherwise the I/O operation returns the number of valid MADUs 
        in the buffer, or -1 multiplied by an error code.
    */
    sioAttrs.timeout = 20; /* 20 msec */
    
    /* Initialize the error flag to 'no error' */
    error = SYS_OK;

    /**********************************************************/
    /* The main processing loop                               */
    /*  Create the SIO objects for RX and TX                  */
    /*  Initialize the PADK H/W                               */
    /*    Issue buffers to RX and TX instances of the driver    */
    /*    Call processingLoop () to perform the DSP           */
    /**********************************************************/             
    while (1) {
        /* 
            If error occured delete the
            SIO object and restart the process.
        */            
        if (error) {
            /* Close both the devices */
            SIO_delete (IOhdl->sioIP);
            SIO_delete (IOhdl->sioOP);
        }
        
        /* Start the i/p and o/p clock circuitry on the H/W */
        IOhdl->brdHdl->brdCtrl (IOhdl->brdHdl, PADK_START_INPUT);
        IOhdl->brdHdl->brdCtrl (IOhdl->brdHdl, PADK_START_OUTPUT);        

        /* Load the DMD configuration for the RX Section */
        dmd = MultiChannelAnalogIP;
        /* Set the frame size */
        dmd.numSamples = FRAMESIZE;
        /* Create the SIO object for RX */
        IOhdl->sioIP = SIO_create("/DMD", SIO_INPUT, NULL, &sioAttrs);
        if (!IOhdl->sioIP) {
            LOG_printf (&trace, "SIO_create for RX failed. Exiting program.");
            exit (SYS_EALLOC);
        }

        /* Provide the DMD params to the DMD driver */
        SIO_ctrl (IOhdl->sioIP, DMD_USR_PARAM, (Arg) &dmd);
        /* 
            Start the McASP clock circuitry.
            The McASP clock circuitry is intentionally handled seperately.
            Some H/W may require clocks being present even before the
            data I/O is started 
        */
        SIO_ctrl (IOhdl->sioIP, DMD_START_CLK, (Arg) NULL);

        /* Load the DMD configuration for the TX Section */
        dmd = MultiChannelAnalogOP;
        /* Set the frame size */
        dmd.numSamples = FRAMESIZE;
        /* Create the SIO object for TX */
        IOhdl->sioOP = SIO_create("/DMD", SIO_OUTPUT, NULL, &sioAttrs);
        /* Provide the DMD params to the DMD driver */
        SIO_ctrl (IOhdl->sioOP, DMD_USR_PARAM, (Arg) &dmd);
        /* 
            Start the McASP clock circuitry.
            The McASP clock circuitry is intentionally handled seperately.
            Some H/W may require clocks being present even before the
            data I/O is started 
        */
        SIO_ctrl (IOhdl->sioOP, DMD_START_CLK, (Arg) NULL);

        /* Issue the buffers to both RX and TX sections */
        for (i = 0; i < IOhdl->pParams->numDrvBuffers; i++)    {
            SIO_issue (    IOhdl->sioIP, IOhdl->ipDrvBuff[i], 
                        pParams->maxIPChannels*sizeof(Float)*
                        FRAMESIZE, NULL);
        }
        for (i = 0; i < IOhdl->pParams->numDrvBuffers; i++)    {
            SIO_issue (    IOhdl->sioOP, IOhdl->opDrvBuff[i],
                        pParams->maxOPChannels*sizeof(Float)*
                        FRAMESIZE, NULL);
        }

        /* Call board control function to UnMute the output */
        IOhdl->brdHdl->brdCtrl (IOhdl->brdHdl, PADK_UNMUTE_OUTPUT);



        /*********************** Fall into the processingLoop code ************************/

        error = processingLoop( IOhdl );

        /**********************************************************************************/


        /* 
            If here, error happened while performing processingLoop.
            LOG the error and start again 
        */
        if (error != SYS_OK)
            LOG_printf (&trace, "SIO_reclaim or SIO_issue Error occured in processingLoop(). Restarting CODEC I/O stream.");

        /* Call board control function to Mute the output */
        IOhdl->brdHdl->brdCtrl (IOhdl->brdHdl, PADK_MUTE_OUTPUT);
        /* Sleep for 1msec */
        TSK_sleep (1);
    }
}


/*
    This hook function is provided by the DMD driver to add
    processing for any dMAX transfer ISRs that are not 
    initiated by the DMD.
*/

Void
DMD_ISR_hook () {
    return;
}


