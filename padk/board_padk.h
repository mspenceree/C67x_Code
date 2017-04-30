/*
 * $Source: E:/Lyrtech/cvsrepository/projets/lyrtech/PADK/dsp_bios_app/boards/padk/board_padk.h,v $
 * $Revision: 1.1 $
 *
 * PADK H/W specific source header file
 * This file defines various board specific parameter structures needed
 * to configure the H/W. 
 *
 * Copyright 2004-2005, Texas Instruments, Inc.  All rights reserved.
 *
 * $Log: board_padk.h,v $
 * Revision 1.1  2006/02/22 19:32:18  louischarles.dumais
 * ajout initial avec tous les fichiers cette fois
 *
 */


#ifndef BOARD_PADK_
#define BOARD_PADK_

/* 
    PADK board header file provided by Lyrtech
*/
#include <padk.h>
#include <std.h>

/*
    Internal data structure used by the board_padk code.
    This structure is not utilized by the F/W.
*/
typedef struct PADK_internal
{
    CLKGEN_Params clkgenParams;
    ADC_Params adcParams;
    DAC_Params dacParams;
} PADK_internal;

/*
    Structure exposed to the F/W to interface with the
    PADK H/W. The interface to the H/W is through the brdCtl().
*/
typedef struct PADK_brdCfg
{
    Fxn brdCtrl;
    PADK_internal * padkInt;    
} PADK_brdCfg;

/*
    PADK H/W instance handle
*/
typedef struct PADK_brdCfg *PADK_brdHdl;

/*
    The commands supported by the control function are defined 
    by the PADK_brdCmd enum
*/
enum PADK_brdCmd
{
    PADK_START_INPUT    = 0x1, /* Start input circuitry */
    PADK_START_OUTPUT   = 0x2, /* Start output circuitry */
    PADK_MUTE_OUTPUT    = 0x3, /* Mute the board o/p */
    PADK_UNMUTE_OUTPUT  = 0x4  /* Un-mute the o/p */
};

/*
    The board specific errors are defined by the 
    PADK_err enum
*/
enum PADK_err
{
    PADK_NOERR           = 0,    /* No error */
    PADK_ENOTIMPL        = - 0x1 /* Control state not implemented */
};

extern Int32 PADK_board_control (PADK_brdHdl hPADK, Uns cmd);
extern PADK_brdHdl PADK_board_open (Int32 segid);

#endif  /* BOARD_PADK_ */
