
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
 * File: algProcessingTaskParams.c
 *
 * dMAX McASP driver (DMD) algProcessingTask Parameters
 * This file defines the parameter structures used to
 * initialize the processing loop task.
 * demonstrates the usage of dMAX McASP driver
 *
 *
 */

#include <algProcessingTask.h>
#include <main.h>

/*
    IOParams structure defines the parameters that are
    used to initialize the code. The structure is
    defined in the header file, algProcessingTask.h. The default
    structure is defined below and is provided to the algProcessingTask
    as TSK argument. 
*/
const IOParams IO_PARAMS =
{
    IO_MAX_CHAN,/* Maximum input channels */
    IO_MAX_CHAN,/* Maximum output channels */
    IO_MAX_CHAN,/* Maximum working channels */

    /* 
        Precision of input and output data.
        This information is used when converting input data
        to floating point and when converting the work
        buffer floating data to output buffer fixed point data.
    */
    24,
    24,

    /*
        Number of buffers issued to the DMD driver.
        Please note DMD driver can accept at max 3 buffers
        (must change DMD_MAX_BUFF back to 3 if used)
    */
    2,
};

/*
    DMD driver accepts a parameter structure to initialize
    the peripherals (McASP and dMAX). The parameter structure
    is defined in the header file: c672x_dmax_mcasp.h 
    The below configuration is for MultiChannel analog input.
    The below structure uses macros CSL_FMKT and CSL_FMK defined
    by CSL. Please refer the CSL header file cslr.h and the
    CSL reference guide. These Macros allow the user to use
    CSL defined Register Layer Symbolic Constants instead of
    hardcoded values
*/
const DMD_Params MultiChannelAnalogIP = 
{
    /* McASP Module to use */
    CSL_MCASP_0,

    /* Mask Register */
    CSL_FMKT(MCASP_RMASK_RMASK, MASK),

    /* Format Register */
    CSL_FMKT(MCASP_RFMT_RDATDLY, 0BIT)    |    \
    CSL_FMKT(MCASP_RFMT_RRVRS, MSBFIRST)    |    \
    CSL_FMKT(MCASP_RFMT_RBUSEL, VBUSP)    |    \
    CSL_FMK(MCASP_XFMT_XSSZ, 0xF)    |    \
    CSL_FMKT(MCASP_RFMT_RROT, NONE),

    /* Frame Sync Control Resgister */
    CSL_FMK(MCASP_AFSRCTL_RMOD, 2)    |    \
    CSL_FMK(MCASP_AFSRCTL_FRWID, 1)    |    \
    CSL_FMKT(MCASP_AFSRCTL_FSRM, INTERNAL)    |    \
    CSL_FMKT(MCASP_AFSRCTL_FSRP, ACTIVEHIGH),

    /* TDM Register */
    CSL_FMK(MCASP_RTDM_RTDMS, 3),

    /* Interrupt Control Register */
    CSL_FMKT(MCASP_RINTCTL_RSTAFRM, DISABLE)    |    \
    CSL_FMKT(MCASP_RINTCTL_RDATA, DISABLE)    |    \
    CSL_FMKT(MCASP_RINTCTL_RLAST, DISABLE)    |    \
    CSL_FMKT(MCASP_RINTCTL_RDMAERR, DISABLE)    |    \
    CSL_FMKT(MCASP_RINTCTL_RCKFAIL, DISABLE)    |     \
    CSL_FMKT(MCASP_RINTCTL_RSYNCERR, DISABLE)    |    \
    CSL_FMKT(MCASP_RINTCTL_ROVRN, DISABLE),

    /* Bit Clock Control Register */
    CSL_FMKT(MCASP_ACLKRCTL_CLKRP, RISING)    |    \
    CSL_FMKT(MCASP_ACLKRCTL_CLKRM, INTERNAL)    |     \
    CSL_FMK(MCASP_ACLKRCTL_CLKRDIV, 7),

    /* High Clock Control Register */
    CSL_FMKT(MCASP_AHCLKRCTL_HCLKRM, EXTERNAL)    |    \
    CSL_FMKT(MCASP_AHCLKRCTL_HCLKRP, RISING)    |     \
    CSL_FMK(MCASP_AHCLKRCTL_HCLKRDIV, 0),

    /* Clock Check Register */
    0x00FF0000,    /* NOT USED */

    /* Bit mask for the serializers active */
    /* **Please note SERIALIZER_1 corresponds to AXR0/1[0]** */
    (1<<SERIALIZER_1) | (1<<SERIALIZER_2) | 
    (1<<SERIALIZER_3) | (1<<SERIALIZER_4),

    /* Num Samples. Information initialized by the F/W */
    NULL,

    /* High Priority Transfer */
    CSL_DMAX_HI_PRIORITY,
   
    /* QTSL of 3. 16 sample burst */
    CSL_DMAX_EVENT0_QTSL_MOVE16_ELE,

    /* Rising edge polarity */
    CSL_DMAX_POLARITY_RISING_EDGE,
};

/*
    DMD driver accepts a parameter structure to initialize
    the peripherals (McASP and dMAX). The parameter structure
    is defined in the header file: c672x_dmax_mcasp.h 
    The below configuration is for MultiChannel analog output.
    The below structure uses macros CSL_FMKT and CSL_FMK defined
    by CSL. Please refer the CSL header file cslr.h and the
    CSL reference guide. These Macros allow the user to use
    CSL defined Register Layer Symbolic Constants instead of
    hardcoded values
*/

const DMD_Params MultiChannelAnalogOP = 
{
    /* McASP Module to use */
    CSL_MCASP_0,

    /* Mask Register */
    CSL_FMKT(MCASP_XMASK_XMASK, MASK),

    /* Format Register */
    CSL_FMKT(MCASP_XFMT_XDATDLY, 0BIT)    |    \
    CSL_FMKT(MCASP_XFMT_XRVRS, MSBFIRST)    |    \
    CSL_FMKT(MCASP_XFMT_XBUSEL, VBUSP)    |    \
    CSL_FMK(MCASP_XFMT_XSSZ, 0xF)    |    \
    CSL_FMKT(MCASP_XFMT_XROT, NONE),

    /* Frame Sync Control Resgister */
    CSL_FMK(MCASP_AFSXCTL_XMOD, 2)    |    \
    CSL_FMK(MCASP_AFSXCTL_FXWID, 1)    |    \
    CSL_FMKT(MCASP_AFSXCTL_FSXM, INTERNAL)    |    \
    CSL_FMKT(MCASP_AFSXCTL_FSXP, ACTIVEHIGH),

    /* TDM Register */
    CSL_FMK(MCASP_XTDM_XTDMS, 3),

    /* Interrupt Control Register */
    CSL_FMKT(MCASP_XINTCTL_XSTAFRM, DISABLE)    |    \
    CSL_FMKT(MCASP_XINTCTL_XDATA, DISABLE)    |    \
    CSL_FMKT(MCASP_XINTCTL_XLAST, DISABLE)    |    \
    CSL_FMKT(MCASP_XINTCTL_XDMAERR, DISABLE)    |    \
    CSL_FMKT(MCASP_XINTCTL_XCKFAIL, DISABLE)    |     \
    CSL_FMKT(MCASP_XINTCTL_XSYNCERR, DISABLE)    |    \
    CSL_FMKT(MCASP_XINTCTL_XUNDRN, DISABLE),

    /* Bit Clock Control Register */
    CSL_FMKT(MCASP_ACLKXCTL_CLKXP, FALLING)    |    \
    CSL_FMKT(MCASP_ACLKXCTL_CLKXM, INTERNAL)    |     \
    CSL_FMKT(MCASP_ACLKXCTL_ASYNC, ASYNC)    |     \
    CSL_FMK(MCASP_ACLKXCTL_CLKXDIV, 7),

    /* High Clock Control Register */
    CSL_FMKT(MCASP_AHCLKXCTL_HCLKXM, EXTERNAL)    |    \
    CSL_FMKT(MCASP_AHCLKXCTL_HCLKXP, RISING)    |     \
    CSL_FMK(MCASP_AHCLKXCTL_HCLKXDIV, 0),

    /* Clock Check Register */
    0x00FF0000,    /* NOT USED */

    /* Bit mask for the serializers active */
    /* **Please note SERIALIZER_1 corresponds to AXR0/1[0]** */
    (1<<SERIALIZER_5) | (1<<SERIALIZER_6) | 
    (1<<SERIALIZER_7) | (1<<SERIALIZER_11),

    /* Num Samples. Information initialized by the F/W */
    NULL,

    /* High Priority Transfer */
    CSL_DMAX_HI_PRIORITY,
    
    /* QTSL of 3. 16 sample burst */
    CSL_DMAX_EVENT0_QTSL_MOVE16_ELE,

    /* Rising edge polarity */
    CSL_DMAX_POLARITY_RISING_EDGE,
};


