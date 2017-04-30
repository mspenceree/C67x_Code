
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
 * File: main.h
 *
 * This file defines all
 *
 * It is to be included in the other *.c files that ...?
 *
 */

#ifndef MAIN_
#define MAIN_


/* CSL header files */
#include <soc.h>
#include <csl.h>
#include <csl_chip.h>
#include <csl_dmax.h>
#include <csl_mcasp.h>

/* BIOS header files */
#include <std.h>
#include <stdlib.h>
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

#include <math.h>               // for FLT_MAX definition
// #define _TI_ENHANCED_MATH_H 1    // so we can use "new" functions like exp10f() (exp10f still not working)

/* PADK H/W specific header file */
#include <board_padk.h>


//////////////////////////////////////////////////////////
// Define the Interface Board specific constants:
//////////////////////////////////////////////////////////
// For switch inputs:
#define I2C_ADDR_ASACS_BYTE_IN0 0x25
#define I2C_ADDR_ASACS_BYTE_IN1 0x26
#define I2C_ADDR_ASACS_BYTE_IN2 0x27

#define SW1_VIDEO_ON_OFF            (1<<0)
#define SW2_SPARE                   (1<<1)
#define SW3_DEFT_ON_OFF             (1<<2)
#define SW3_PLAY_MODE_SINGLE        (1<<3)
#define SW4_PLAY_MODE_LOOP          (1<<4)
#define SW5_PLAY_MODE_LIBRARY       (1<<5)
#define SW6_LIGHT_MODE_SPOT         (1<<6)
#define SW7_LIGHT_MODE_FLOOD        (1<<7)

#define SW9_LIGHT_MODE_STROBE       (1<<8)
#define SW10_SPARE                  (1<<9)
#define SW11_ZOOM_IN                (1<<10)
#define SW12_ZOOM_OUT               (1<<11)
#define SW13_FILE_SELECT            (1<<12)
#define SW14_AUDIO_TRIGGER          (1<<13)
#define SW15_VOLUME_MEDIUM          (1<<14)
#define SW16_VOLUME_HIGH            (1<<15)

#define SW17_SPARE                  (1<<16)

#define SW18_AUDIO_ON_OFF           (1<<21)
#define SW19_UNLOCK_SWITCH          (1<<22)
#define SW20_PUSH_TO_TALK_F         (1<<23)


// For Digital Outputs:
#define I2C_ADDR_ASACS_BYTE_OUT     0x20

#define INPUT_OK_LED_BIT            (1<<4)
#define DSP_OK_LED_BIT              (1<<3)  // This realy means DSP and Interface Board are OK
#define TRANCEDUCER_AMP_OK_LED_BIT  (1<<2)
#define SPARE1_LED_BIT              (1<<1)
#define SPARE2_LED_BIT              (1<<0)

#define AMPSHUTDOWN_F_BIT           (1<<5)
#define DSP_PUSHTOTALK_F_BIT        (1<<6)
#define DSP_READY_F_BIT             (1<<7)

// For analog mux address port outputs:
#define I2C_ADDR_ASACS_ADDR_A       0x22
#define I2C_ADDR_ASACS_ADDR_B       0x23
#define I2C_ADDR_ASACS_ADDR_C       0x24



//////////////////////////////////////////////////////////
// Define glogal variable/parameter related constants:
//////////////////////////////////////////////////////////
//#define PC_STATUS_INTERFACE_ERROR_BIT (1<<1)



//////////////////////////////////////////////////////////
// Define USB communications Protocol constants:
//////////////////////////////////////////////////////////
// DSP Flags Byte, bits:
#define DSP_FLAGS_INTERFACE_BOARD_ERROR_BIT     (1<<0)
#define DSP_FLAGS_SWITCHES_CHANGED_BIT          (1<<1)
#define DSP_FLAGS_AMP_SPEAKER_TEST_ERROR_BIT    (1<<2)

// Error Codes:
#define NO_ERROR                        0
#define BLOCK_TYPE_CODE_ERROR           1
#define BLOCK_INSTANCE_ERROR            2
#define PARAMETER_CODE_ERROR            3
#define WRITE_NOT_SUPPORTED             4
#define READ_NOT_SUPPORTED              5
#define DATA_VALUES_OUT_OF_RANGE        6
#define NUMBER_OF_DATA_WORDS_INCORRECT  7


// DSP_Modes codes:
#define UNDEFINED -1
#define STANDBY 0
#define LISTEN  1
#define TALK    2
#define TEST    3


#define FADE_WINDOW_START     0.0   // start at 0dB
#define FADE_WINDOW_STOP    -50.0   // end at -50dB

//forward declorations:
int my_compare_func( const void *ptr1, const void *ptr2 );
float my_median5( float *inSample, float *history );
float my_atof( char s[] );
int i2c_error( int I2C_result );
int digitalOutByte( unsigned char byteVal );
int digitalOutBit( int bitVal, unsigned char mask );
int analogMuxHeadphoneSelect( int addrA67 );
int analogMuxDSPIn78Select( int addrC0123 );
int initInterfaceBoard();
void initGlobalVars();
void ChangeDSP_Mode( int newDSP_Mode );
int StopOp( int *OpStatus, int Confirm );
int StartOp( int *OpStatus, int Confirm );



#endif  /* MAIN_ */

