
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
 * File: asacs.cmd
 *
 * Linker CMD file for asacs.pjt
 *
 * M.Spencer
 *
 */

/* Define the search path for the libraries */

/*-i ..\padk\lib	/* PADK board support library  */ 

-i ..\csl_C672x_v3_00_08_04\lib 	/* CSL library */
/* -i C:\CCStudio_v3.3\C6000\CSL_v3_00_08_04\csl_C672x\dsp\lib 			 /* MSpencer CSL library */
/* -i C:\CCStudio_v3.3\C6000\CSL_v3_00_08_04\csl_C672x_intc\dsp\lib 	 /* MSpencer CSL library */

-i ..\PatchDspRom /* 672x system patch library */

-i C:\CCStudio_v3.3\bios_5_20_03\packages\ti\rtdx\lib\c6000 		/* Must use this version to "link with ROM" (in asacs.tcf file) */
																	/* This is the version that is actually in the current C672x chip */
/*-i C:\CCStudio_v3.3\bios_5_31_02\packages\ti\rtdx\lib\c6000		/* This version will not work */


/* Include Objects and Library to apply the System Patch */
-l applySystemPatch.obj
-l c672xSystemPatchV2_00_00.lib

/* Include fastRTS. This does not seem to give access to anything? */
/* -l fastmath67x.lib */

/* This is so I can use the internal ROMed version of some math functions: */
/* -l fastrts67xR.lib */

/* Generated CMD file */
-l asacscfg.cmd

/* CSL Libraries */
-l csl_C6727.lib
/*-l csl_C672x_intc.lib */

/* PADK board support library */
/* -l padk.lib */
