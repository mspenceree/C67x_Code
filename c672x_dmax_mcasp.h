
/**************************************************************************************************
 *																								  *
 *							            SSSSS    PPPPPP    SSSSS								  * 
 *							          SS   SS   PP   PP  SS   SS								  * 
 *							         SS        PP   PP  SS										  *
 *							         SSSSS    PPPPPP    SSSSS									  *
 *							            SS   PP            SS									  *
 *							      SS   SS   PP       SS   SS									  *
 *							      SSSSS    PP        SSSSS										  *
 *						 																		  *
 *							     SIGNAL  PROCESSING  SOLUTIONS									  *
 *																								  *
 **************************************************************************************************
 *																								  *
 *					Project:							  *
 *																								  *
 *					Author:		Michael E. Spencer, Ph.D.										  *
 *																								  *
 *					Copyright(c) Signal Processing Solutions, 2007								  *
 *																								  *
 **************************************************************************************************
 *
 * File: c672x_dmax_mcasp.h
 *
 * Original by Lyrtech and TI.
 *
 *
 */

#ifndef C672X_DMAX_MCASP_
#define C672X_DMAX_MCASP_

/* CSL header files */
#include <soc.h>
#include <csl.h>
#include <csl_chip.h>
#include <csl_dmax.h>
#include <csl_mcasp.h>

/* BIOS header files */
#include <std.h>
#include <dev.h>
#include <hwi.h>
#include <log.h>
#include <mem.h>
#include <que.h>
#include <sem.h>
#include <sys.h>
#include <c62.h>



/* 
    There can be atmost 6 instances of the driver running 
    simultaneously. Thus, the maximum TCC values that need
    to be reserved for the driver is 6
*/
#define DMD_MAX_TCC  6

/*
    The DMD driver uses TCC 0-5 for creating the dMAX object.
    The 6 TCC are reserved for the DMD driver
*/
#define DMD_TCC_MASK 0x3F

/*
    The driver is designed to accept maximum 3 buffers at any
    given time. There can be maximum three buffers that can be
    issued to the driver. Before issuing any new buffer, a
    reclaim call needs to be made.
*/
#define DMD_MAX_BUFF 3



/*
     The below structure is the parameter structure for the driver.
     The McASP and the dMAX peripherals are initialized based on
     the parameters passed in the structure.
*/
typedef struct DMD_Params {
    Int32 moduleNum;     /* McASP module to use */
    Int32 rxMask;        /* Receive Mask register */
    Int32 rxFmt;         /* Receive Format register */
    Int32 rxFSCtl;       /* Receive FrameSync control register */
    Int32 rxTDM;         /* Receive TDM register */
    Int32 rxIntCtl;      /* Receive Interrupt control register */
    Int32 rxClkSetup;    /* Receive BitClock control register */
    Int32 rxHIClkSetup;  /* Receive HighClock control register */
    Int32 rxClkChk;      /* Receive Clock Check register */
    Int32 pinMask;       /* Receive PinMask register */
    /* 
        Number of samples to be received/transmitted for each
        buffer that is issues to the driver
    */
    Int32 numSamples;
    Int32 priority;      /* dMAX priority to be used */
    Int32 qtsl;          /* QTSL to be used for the dMAX initialization */
    Int32 polarity;      /* Polarity to be used the dMAX event initialization */
} DMD_Params;

/*
    This is internal structure used by the driver to 
    understand the resource usage
*/
typedef struct DMD_DriverObject
{
    Int32 tcc;    /* Current status of TCC in use */
    /* Hnadle for McASP objects */
    CSL_McaspHandle hMcasp [CSL_MCASP_CNT];
    /* Handle for the various instances of the driver */
    DEV_Handle    pDevice[CSL_MCASP_CNT*2];
} DMD_DriverObject;

/*
    The DMD driver extension structure. 
    This structure holds information for one instance of the
    DMD driver.  
*/
typedef struct DMD_DeviceExtension
{
    Uint8                tcc;       /* TCC used by the instance */
    Uint8                runState;  /* Whether the instance in active */
    Uint8                numSerializers;/* Num serializers used */
    Uint8                numQueuedRcv;  /* Buffers in RCV Queue */
    Uint8                numQueuedXmt;  /* Buffers in XMT Queue */
    Uint8                pp;            /* Flag to track PP bit */
    Int32                errorState;    /* Error tracking flag */
    /* 
        Semaphore handle used by the DRV instance to signal 
        completion of a buffer transfer
    */
    SEM_Handle          sync;    
    QUE_Obj             rcvQue;    /* RCV queue object */
    QUE_Obj             xmtQue;    /* XMT queue object */
    CSL_DmaxHandle      hDmax;     /* dMAX handle used by the instance */
    LOG_Obj             *pTraceLog;/* LOG trace to print real time info */
    const DMD_Params    *pParams;  /* Params used to create the instance */
} DMD_DeviceExtension;


/*
    DMD driver provides a control function to allow the framework to
    interface with the driver. The control function accepts as an
    argument the control code that indicates to the control function
    what action to take 
*/
enum DMD_ControlCode
{
    /* 
        The control function call is to provide the driver params for
        the given instance of the DMD driver
    */
    DMD_USR_PARAM    = 0x1,    
    /* 
        The control function call is to provide the LOG object that
        the driver may use to print real time information
    */
    DMD_HOOK_LOG     = 0x2,
    /* 
        The control function call is to indicate to the driver to start
        the McASP clocks with the configuration as described in the
        driver parameters.
    */
    DMD_START_CLK    = 0x3
};

/*
    For an error condition, the DMD driver will return with an
    error code. The various error codes are described in the below enum.
*/
enum DMD_ErrorCode
{
    DMD_ERR_ALLOC     =     0x100,  /* Error in memory allocation */
    DMD_ERR_NOTCC     =     0x101,  /* No TCC available for allocation */
    DMD_ERR_SPOPEN    =     0x102,  /* Error in McASP object creation */
    /* 
        Driver instance can't be created because the McASP peripheral
        requested is already in use by some other instance
     */
    DMD_ERR_BUSY      =     0x103, 
    DMD_ERR_NOBUF     =     0x104,  /* Driver is out of new buffer */
    DMD_ERR_ENQUE     =     0x105,  /* Error adding new buffer to drv queue */
    DMD_ERR_SPERR     =     0x106   /* McASP error occured */
};

#endif























