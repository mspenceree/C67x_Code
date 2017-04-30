/***************************************************************************
 *
 *
 *         **   **    **  ******  ********  ********  ********  **    **    
 *        **    **   **  **   ** ********  ********  ********  **    **
 *       **     *****   **   **    **     **        **        **    **
 *      **       **    ******     **     ****      **        ********
 *     **       **    **  **     **     **        **        **    **
 *    *******  **    **   **    **     ********  ********  **    **
 *   *******  **    **    **   **     ********  ********  **    **  
 *
 *            L Y R T E C H   S I G N A L   P R O C E S S I N G              
 *
 ***************************************************************************
 *                                                                          
 *  Project     : PADK
 *  File        : PADK_MCASP.h
 *  Description	: Define the default configuration for the McASPs
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:18 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/
#ifndef __PADK_MCASP_H__
#define __PADK_MCASP_H__

#include <csl_mcasp.h>

#define ADC_32K_CLK		(12-1)
#define ADC_44_1K_CLK	(8-1)
#define ADC_48K_CLK		(8-1)
#define ADC_64K_CLK		(6-1)
#define ADC_88_2K_CLK	(4-1)
#define ADC_96K_CLK		(4-1)
#define ADC_176_4K_CLK	(2-1)
#define ADC_192K_CLK	(2-1)

#define DAC_32K_CLK		(12-1)
#define DAC_44_1K_CLK	(8-1)
#define DAC_48K_CLK		(8-1)
#define DAC_64K_CLK		(6-1)
#define DAC_88_2K_CLK	(4-1)
#define DAC_96K_CLK		(4-1)
#define DAC_176_4K_CLK	(2-1)
#define DAC_192K_CLK	(2-1)

#define DIR_32K_CLK		(12-1)
#define DIR_44_1K_CLK	(8-1)
#define DIR_48K_CLK		(8-1)
#define DIR_64K_CLK		(6-1)
#define DIR_88_2K_CLK	(4-1)
#define DIR_96K_CLK		(4-1)
#define DIR_176_4K_CLK	(2-1)
#define DIR_192K_CLK	(2-1)
#define DIR_WORD_CLK	(4-1)
#define DIR_REC_CLK		(4-1)

#define DIT_32K_CLK		(6-1)
#define DIT_44_1K_CLK	(4-1)
#define DIT_48K_CLK		(4-1)
#define DIT_64K_CLK		(3-1)
#define DIT_88_2K_CLK	(2-1)
#define DIT_96K_CLK		(2-1)
#define DIT_176_4K_CLK	(1-1)
#define DIT_192K_CLK	(1-1)
#define DIT_WORD_CLK	(2-1)
#define DIT_REC_CLK		(2-1)

/*
 *  PADK_MCASP0_HWSETUP_DEFAULTS
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This configuration can be used to initialize the McASP 0 to access the 
 *  ADCs and the DACs. All the connected serializers are enabled. The application 
 *  needs to disable unwanted serializers.
 */
#define  PADK_MCASP0_HWSETUP_DEFAULTS {										\
    /* Value to be loaded in global control register (GLBCTL) */			\
    {																		\
        /* Pin function register (PFUNC) */									\
          (1<< 9)          /* AXR9: GPIO (DSP_EXT_INT5) */					\
        | (1<< 8)          /* AXR8: GPIO (DSP_EXT_INT4)	*/					\
        | (1<< 7),         /* AXR7: GPIO (DSP_EXT_INT6)	*/					\
        																			\
        /* Pin direction register (PDIR) */									\
          (1<<31)          /* AFSR  : Output */								\
        | (0<<30)          /* AHCLKR: Input */								\
        | (1<<29)          /* ACLKR : Output */								\
        | (1<<28)          /* AFSX  : Output */								\
        | (0<<27)          /* AHCLKX: Input */								\
        | (1<<26)          /* ACLKX : Output */								\
        | (1<<25)          /* AMUTE : Output */								\
		| (1<<10)          /* AXR10 : Output (DAC_DATA4) */					\
        | (0<< 9)          /* AXR9  : Input (DSP_EXT_INT5) */				\
        | (0<< 8)          /* AXR8  : Input (DSP_EXT_INT4) */				\
        | (0<< 7)          /* AXR7  : Input (DSP_EXT_INT6) */				\
        | (1<< 6)          /* AXR6  : Output (DAC_DATA3) */					\
        | (1<< 5)          /* AXR5  : Output (DAC_DATA2) */					\
        | (1<< 4)          /* AXR4  : Output (DAC_DATA1) */					\
        | (0<< 3)          /* AXR3  : Input (ADC_DATA4) */					\
        | (0<< 2)          /* AXR2  : Input (ADC_DATA3) */					\
        | (0<< 1)          /* AXR1  : Input (ADC_DATA2) */					\
        | (0<< 0),         /* AXR0  : Input (ADC_DATA1) */					\
																			\
        /* Global control register (GBLCTL) */								\
        0x00000000,         /* Everything in reset */						\
																			\
        /* Decides whether McASP operates in DIT mode (DITCTL) */			\
        0x00000000,         /* DIT mode disable */							\
																			\
        /* Digital loopback mode setup (DLBEN) */							\
        0x00000000,         /* Loopback mode disable */						\
																			\
        /* Mute control register (AMUTE) */									\
        0x00000000,         /* AMUTE pin is disabled */						\
																			\
        /* Setup serializer control register (SRCTL0-15) */					\
        0x00000002,         /* Serializer  0 is in rx (ADC 0) */			\
        0x00000002,         /* Serializer  1 is in rx (ADC 1) */			\
        0x00000002,         /* Serializer  2 is in rx (ADC 2) */			\
        0x00000002,         /* Serializer  3 is in rx (ADC 3) */			\
        0x00000001,         /* Serializer  4 is in tx (DAC 0) */			\
        0x00000001,         /* Serializer  5 is in tx (DAC 1) */			\
        0x00000001,         /* Serializer  6 is in tx (DAC 2) */			\
        0x00000000,         /* Serializer  7 is unused */  					\
		0x00000000,         /* Serializer  8 is unused */					\
		0x00000000,         /* Serializer  9 is unused */					\
		0x00000001,         /* Serializer 10 is in tx (DAC 3) */			\
		0x00000000,         /* Serializer 11 is unused */					\
		0x00000000,         /* Serializer 12 is unused */					\
		0x00000000,         /* Serializer 13 is unused */					\
		0x00000000,         /* Serializer 14 is unused */					\
		0x00000000          /* Serializer 15 is unused */					\
    },																		\
																			\
    /* Receiver settings */													\
    {        																\
        /* To mask or not to mask (RMASK) */								\
        0xFFFFFFFF,         /* 24-bits left justified */            		\
																			\
        /* Format details as per (RFMT) */									\
          ( 0<<16)         /* XDATDLY: Transmit with 0 bit delay */			\
        | ( 1<<15)         /* XRVRS  : MSB first */							\
        | ( 0<< 3)         /* XBUSSEL: Writes from dMAX port */				\
		| (15<< 4)         /* XSSZ   : 32-bit slot size */					\
        | ( 0<< 0),        /* XROT   : No rotation */						\
																			\
        /* Configure the rcv frame sync (AFSRCTL) */						\
          (2<< 7)          /* RMOD : 2-slot TDM */							\
        | (1<< 4)          /* FRWID: Single word frame sync */				\
        | (1<< 1)          /* FSRM : Internally-generated tx frame sync */	\
        | (0<< 0),         /* FSRP : Start on rising edge */				\
                                                                            \
        /* Specifies which TDM slots are active (RTDM) */					\
        0x00000003,         /* Enable all slots */							\
                                                                            \
        /* Controls generation of McASP interrupts (RINTCTL) */				\
        0x00000000,         /* No interrupts */								\
																			\
        /* Status register (controls writable fields of STAT reg) (RSTAT) */\
        0x00000000,															\
																			\
        /* Event control register (REVTCTL) */								\
        0x00000000,															\
																			\
        /* Clock settings for rcv */										\
        {																	\
			/* Clock details (ACLKRCTL) */									\
              (1<< 7)          /* CLKXP  : RX on rising edge */				\
            | (1<< 6)          /* ASYNC  : TX and RX are asynchronous ? */	\
            | (1<< 5)          /* CLKRM  : Int clk source from PLL div */	\
            | (0<< 0),         /* CLKRDIV: Need to be initialized by user */\
																			\
            /* High clock details (AHCLKRCTL) */							\
              (0<<15)          /* HCLKRM  : Clock from AHCLKR pin */		\
            | (0<<14)          /* HCLKRP  : Rising edge */					\
            | (0<< 0),         /* HCLKRDIV: AHCLKR = AUXCLK / 1 */			\
																			\
            /* Config receive/transmit clock failure detection (RCLKCHK) */	\
            0x00FF0000          /* Not used */								\
        }																	\
    },																		\
																			\
    /* Transmitter settings */												\
    {        																\
        /* To mask or not to mask (XMASK) */								\
        0xFFFFFFFF,         /* 24-bits left justified */					\
																			\
        /* Format details as per (XFMT) */									\
          ( 0<<16)         /* XDATDLY: Transmit with 0 bit delay */			\
        | ( 1<<15)         /* XRVRS  : MSB first */							\
        | ( 0<< 3)         /* XBUSSEL: Writes from dMAX port */				\
		| (15<< 4)         /* XSSZ   : 32-bit slot size */					\
        | ( 0<< 0),        /* XROT   : No rotation */						\
																			\
        /* Configure the xmt frame sync (AFSXCTL) */						\
          (2<< 7)          /* XMOD : 2-slot TDM */							\
        | (1<< 4)          /* FXWID: Single word frame sync */				\
        | (1<< 1)          /* FSXM : Internally-generated tx frame sync */	\
        | (0<< 0),         /* FSXP : Start on rising edge */				\
																			\
        /* Specifies which TDM slots are active (XTDM) */					\
        0x00000003,         /* Enable all slots */							\
																			\
        /* Controls generation of McASP interrupts (XINTCTL) */				\
        0x00000000,         /* No interrupts */								\
																			\
        /* Status register (controls writable fields of STAT reg) (XSTAT) */\
        0x00000000,															\
																			\
        /* Event control register (XEVTCTL) */								\
        0x00000000,															\
																			\
        /* Clock settings for xmt */										\
        {																	\
			/* Clock details (ACLKXCTL) */									\
              (1<< 7)          /* CLKXP  : TX on falling edge */			\
            | (1<< 6)          /* ASYNC  : TX and RX are asynchronous ? */	\
            | (1<< 5)          /* CLKXM  : Int clk source from PLL div */	\
            | (0<< 0),         /* CLKXDIV: Need to be initialized by user */\
																			\
            /* High clock details (AHCLKXCTL) */							\
              (0<<15)          /* HCLKXM  : Clock from AHCLKX pin */		\
            | (0<<14)          /* HCLKXP  : Rising edge */					\
            | (0<< 0),         /* HCLKXDIV: AHCLKX = AUXCLK / 1 */			\
																			\
            /* Conf receive/transmit clock failure detection (XCLKCHK) */	\
            0x00FF0000          /* Not used */								\
        }																	\
    },																		\
																			\
    /* Power down emulation mode params - PWRDEMU */						\
    CSL_MCASP_PWRDEMU_FREE_OFF												\
}

/*
 *  PADK_MCASP1_HWSETUP_DEFAULTS
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This configuration can be used to initialize the McASP 1 to access the 
 *  DIR device.
 */
#define  PADK_MCASP1_HWSETUP_DEFAULTS {										\
    /* Value to be loaded in global control register (GLBCTL) */			\
    {																		\
        /* Pin function register (PFUNC) */									\
        0x00000000,															\
																			\
        /* Pin direction register (PDIR) */									\
          (1<<31)          /* AFSR  : Output */								\
        | (0<<30)          /* AHCLKR: Input */								\
        | (1<<29)          /* ACLKR : Output */								\
        | (1<<28)          /* AFSX  : Output */								\
        | (0<<27)          /* AHCLKX: Input */								\
        | (1<<26)          /* ACLKX : Output */								\
        | (1<<25)          /* AMUTE : Output */								\
        | (0<< 0),         /* AXR0  : Input (DIR) */						\
																			\
        /* Global control register (GBLCTL) */								\
        0x00000000,         /* Everything in reset */						\
																			\
        /* Decides whether McASP operates in DIT mode (DITCTL) */			\
        0x00000000,         /* DIT mode disable */							\
																			\
        /* Digital loopback mode setup (DLBEN) */							\
        0x00000000,         /* Loopback mode disable */						\
																			\
        /* Mute control register (AMUTE) */									\
        0x00000000,         /* AMUTE pin is disabled */						\
																			\
        /* Setup serializer control register (SRCTL0-15) */					\
        0x00000002,         /* Serializer 0 is in rx */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000          /* Serializer not used */						\
    },																		\
																			\
																			\
    /* Receiver settings */													\
    {        																\
        /* To mask or not to mask (RMASK) */								\
        0xFFFFFF00,         /* 24-bits left justified */           			\
																			\
        /* Format details as per (RFMT) */									\
          ( 0<<16)         /* XDATDLY: Transmit with 0 bit delay */			\
        | ( 1<<15)         /* XRVRS  : MSB first */							\
        | ( 0<< 3)         /* XBUSSEL: Writes from dMAX port */				\
		| (15<< 4)         /* XSSZ   : 32-bit slot size */					\
        | ( 0<< 0),        /* XROT   : No rotation */						\
																			\
        /* Configure the rcv frame sync (AFSRCTL) */						\
          (2<< 7)          /* RMOD : 2-slot TDM */							\
        | (1<< 4)          /* FRWID: Single word frame sync */				\
        | (1<< 1)          /* FSRM : Internally-generated tx frame sync */	\
        | (1<< 0),         /* FSRP : Start on falling edge */				\
																			\
        /* Specifies which TDM slots are active (RTDM) */					\
        0x00000003,         /* Enable slot 0-1 */							\
																			\
        /* Controls generation of McASP interrupts (RINTCTL) */				\
        0x00000000,         /* No interrupts */								\
																			\
        /* Status register (controls writable fields of STAT register) (RSTAT) */ \
        0x00000000,															\
																			\
        /* Event control register (REVTCTL) */								\
        0x00000000,															\
																			\
        /* Clock settings for rcv */										\
        {																	\
			/* Clock details (ACLKRCTL) */									\
              (1<< 7)          /* CLKXP  : RX on rising edge */				\
            | (1<< 6)          /* ASYNC  : TX and RX are asynchronous ? */	\
            | (1<< 5)          /* CLKRM  : Internal clk source from PLL div */ \
            | (0<< 0),         /* CLKRDIV: Need to be initialized by user */\
																			\
            /* High clock details (AHCLKRCTL) */							\
              (0<<15)          /* HCLKRM  : Clock from AHCLKR pin */		\
            | (0<<14)          /* HCLKRP  : Rising edge */					\
            | (0<< 0),         /* HCLKRDIV: AHCLKR = AUXCLK / 1 */			\
																			\
            /* Configures receive/transmit clock failure detection (RCLKCHK) */ \
            0x00FF0000          /* Not used */								\
        }																	\
    },																		\
																			\
    /* Transmitter settings */												\
    {      																	\
        CSL_MCASP_XMASK_RESETVAL,											\
		CSL_MCASP_XFMT_RESETVAL,											\
		CSL_MCASP_AFSXCTL_RESETVAL,											\
		CSL_MCASP_XTDM_RESETVAL,											\
		CSL_MCASP_XINTCTL_RESETVAL,											\
		CSL_MCASP_XSTAT_RESETVAL,											\
		CSL_MCASP_XEVTCTL_RESETVAL,											\
		CSL_MCASP_ACLKXCTL_RESETVAL,										\
		CSL_MCASP_AHCLKXCTL_RESETVAL,										\
		CSL_MCASP_XCLKCHK_RESETVAL   										\
    },																		\
																			\
    /* Power down emulation mode params - PWRDEMU  */						\
    CSL_MCASP_PWRDEMU_FREE_OFF												\
}


/*
 *  PADK_MCASP2_HWSETUP_DEFAULTS
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This configuration can be used to initialize the McASP 2 in DIT mode
 *	for SPDIF transmission through the optical and RCA connectors. Both 
 *  connected serializers are enabled. The application needs to disable 
 *  unwanted serializers.
 */
#define  PADK_MCASP2_HWSETUP_DEFAULTS {										\
    /* Value to be loaded in global control register (GLBCTL) */			\
    {																		\
        /* Pin function register (PFUNC) */									\
        0,																	\
																			\
        /* Pin direction register (PDIR) */									\
          (1<<31)          /* AFSR  : Output */								\
        | (0<<30)          /* AHCLKR: Input */								\
        | (1<<29)          /* ACLKR : Output */								\
        | (1<<28)          /* AFSX  : Output */								\
        | (0<<27)          /* AHCLKX: Input */								\
        | (1<<26)          /* ACLKX : Output */								\
        | (1<<25)          /* AMUTE : Output */								\
        | (1<< 1)          /* AXR1  : Output (Opto) */						\
        | (1<< 0),         /* AXR0  : Output (Rca) */						\
																			\
        /* Global control register (GBLCTL) */								\
        0x00000000,         /* Everything in reset */						\
																			\
        /* Decides whether McASP operates in DIT mode (DITCTL) */           \
        0x00000001,         /* DIT mode enable */							\
																			\
        /* Digital loopback mode setup (DLBEN) */							\
        0x00000000,         /* Loopback mode disable */						\
																			\
        /* Mute control register (AMUTE) */									\
        0x00000000,         /* AMUTE pin is disabled */						\
																			\
        /* Setup serializer control register (SRCTL0-5) */					\
        0x00000001,         /* Serializer 0 is in tx */						\
        0x00000001,         /* Serializer 1 is in tx */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000,         /* Serializer not used */						\
		0x00000000          /* Serializer not used */						\
    },																		\
																			\
    /* Receiver settings */													\
    {        																\
        CSL_MCASP_RMASK_RESETVAL,											\
		CSL_MCASP_RFMT_RESETVAL,											\
		CSL_MCASP_AFSRCTL_RESETVAL,											\
		CSL_MCASP_RTDM_RESETVAL,											\
		CSL_MCASP_RINTCTL_RESETVAL,											\
		CSL_MCASP_RSTAT_RESETVAL,											\
		CSL_MCASP_REVTCTL_RESETVAL,											\
		CSL_MCASP_ACLKRCTL_RESETVAL,										\
		CSL_MCASP_AHCLKRCTL_RESETVAL,										\
		CSL_MCASP_RCLKCHK_RESETVAL											\
    },																		\
																			\
    /* Transmitter settings */												\
    {      																	\
        /* To mask or not to mask (XMASK) */								\
        0xFFFFFF00,                  										\
																			\
        /* Format details as per (XFMT) */									\
          ( 0<<16)         /* XDATDLY: Transmit with 0 bit delay */			\
        | ( 0<<15)         /* XRVRS  : LSB first */							\
        | ( 0<< 3)         /* XBUSSEL: Writes from dMAX port */				\
		| (15<< 4)         /* XSSZ   : 32-bit slot size */					\
        | ( 2<< 0),        /* XROT   : Rotate right by 8 bits */			\
																			\
        /* Configure the xmt frame sync (AFSXCTL) */						\
          (384<< 7)        /* XMOD : 384-slot TDM */						\
        | (  0<< 4)        /* FXWID: Single bit frame sync */				\
        | (  1<< 1)        /* FSXM : Internally-generated tx frame sync */	\
        | (  0<< 0),       /* FSXP : Start on rising edge */				\
																			\
        /* Specifies which TDM slots are active (XTDM) */					\
        0xFFFFFFFF,         /* Enable all slots */							\
																			\
        /* Controls generation of McASP interrupts (XINTCTL) */				\
        0x00000000,         /* No interrupts */								\
																			\
        /* Status register (controls writable fields of STAT register) (XSTAT) */ \
        0x00000000,															\
																			\
        /* Event control register (XEVTCTL) */								\
        0x00000000,															\
																			\
        /* Clock settings for xmt */										\
        {																	\
			/* Clock details (ACLKXCTL) */									\
              (0<< 7)          /* CLKXP  : TX on rising edge */				\
            | (1<< 6)          /* ASYNC  : TX and RX are asynchronous ? */	\
            | (1<< 5)          /* CLKXM  : Internal clk source from PLL div */ \
            | (0<< 0),         /* CLKXDIV: Need to be initialized by user */\
																			\
            /* High clock details (AHCLKXCTL) */							\
              (0<<15)          /* HCLKXM  : Clock from AHCLKX pin */		\
            | (0<<14)          /* HCLKXP  : Rising edge */					\
            | (0<< 0),         /* HCLKXDIV: AHCLKX = AUXCLK / 1 */			\
																			\
            /* Configures receive/transmit clock failure detection (XCLKCHK)*/ \
            0x00FF0000          /* Not used */								\
        }																	\
    },																		\
																			\
    /* Power down emulation mode params - PWRDEMU */						\
    CSL_MCASP_PWRDEMU_FREE_OFF												\
}


#endif	// __PADK_MCASP_H__
