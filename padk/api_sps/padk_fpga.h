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
 *  File        : PADK.h
 *  Description	: Evaluation Board FPGA Defines
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:17 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/
#ifndef _PADK_FPGA_H_
#define _PADK_FPGA_H_

#define FPGA_BASE_ADDR				(ASYNC_CE + 0x6000)
#define FLASH_BASE_ADDR				(ASYNC_CE + 0x0000)
#define UART_BASE_ADDR				(ASYNC_CE + 0x6480)
#define MIDI_BASE_ADDR				(ASYNC_CE + 0x6500)
#define NFLASH_BASE_ADDR			(ASYNC_CE + 0x1000) // TBD
#define DSKCE2_BASE_ADDR			(ASYNC_CE + 0x2000)
#define DSKCE3_BASE_ADDR			(ASYNC_CE + 0x4000)

#define FPGA_FLASH        ((volatile unsigned short *)(FLASH_BASE_ADDR))

#define FPGA_USB_DATA_BYTE(n)  ((volatile unsigned char *)(FPGA_BASE_ADDR + (n) * 0x20 + 4))          // 0 <= n <= 3
#define FPGA_USB_DATA_HWORD(n) ((volatile unsigned short *)(FPGA_BASE_ADDR + (n) * 0x20))          // 0 <= n <= 3
#define FPGA_USB_DATA_WORD(n)  ((volatile unsigned *)(FPGA_BASE_ADDR + (n) * 0x20))          // 0 <= n <= 3
#define FPGA_USB_PKECNT(n)     (*((volatile unsigned short *)(FPGA_BASE_ADDR + (n) * 0x20 + 8)))          // 0 <= n <= 3
#define FPGA_USB_THRESHOLD(n)  (*((volatile unsigned short *)(FPGA_BASE_ADDR + 0x10 + (n) * 0x20)))   // 0 <= n <= 3
#define FPGA_USB_SIZE(n)       (*((volatile unsigned short *)(FPGA_BASE_ADDR + 0x10 + (n) * 0x20 + 8)))   // 0 <= n <= 3


#define FPGA_REG(n)   (*((volatile unsigned short *)(FPGA_BASE_ADDR + 0x0080 + (n) * 0x0010)))

#define FPGA_MISC_CTRL FPGA_REG(0) // reg-0, R/W
	#define DSP_RESET_BIT 		    0x0001
	#define UART_RESET_BIT 			0x0002 // 0 = reset
	#define NOT_PER_RESET_BIT 		0x0004 // 0 == reset
	#define NOT_USB_RESET_BIT 		0x0010
	#define NOT_ADC_RESET_BIT 		0x0020               
	
#define FPGA_USB_CONFIG FPGA_REG(1) // reg-1, R/W
	#define USB_FIFO_E0             0x0001 // 0 == disable, 1 = enable
	#define USB_FIFO_E1             0x0002 // 0 == disable, 1 = enable
	#define USB_FIFO_E2             0x0004 // 0 == disable, 1 = enable
	#define USB_FIFO_E3             0x0008 // 0 == disable, 1 = enable
	#define USB_FIFO_En(n)          (USB_FIFO_E0 << (n))

	#define USB_FIFO_OUT0           0x0010 // 1 == OUT, 0 = in (Read ONLY)
	#define USB_FIFO_OUT1           0x0020 // 1 == OUT, 0 = in (Read ONLY)
	#define USB_FIFO_OUT2           0x0040 // 1 == OUT, 0 = in (Read ONLY)
	#define USB_FIFO_OUT3           0x0080 // 1 == OUT, 0 = in (Read ONLY)
	#define USB_FIFO_OUTn(n)        (USB_FIFO_OUT0 << (n))

	#define USB_FIFO_IE0            0x0100 // 1 == enable, 0 = disable
	#define USB_FIFO_IE1            0x0200 // 1 == enable, 0 = disable
	#define USB_FIFO_IE2            0x0400 // 1 == enable, 0 = disable
	#define USB_FIFO_IE3            0x0800 // 1 == enable, 0 = disable
	#define USB_FIFO_IEn(n)         (USB_FIFO_IE0 << (n))

	#define USB_USBUP_IE            0x1000 // 1 == enable, 0 = disable
	#define USB_GIE                 0x8000 // 1 == enable, 0 = disable
	               
#define FPGA_USB_CTRL  FPGA_REG(2) // reg-2, R/W
	#define USB_GPO0_BIT   			0x0001
	#define USB_2    			    0x0020  // 1 = USB 2.0
	#define USB_READY  			    0x0004  // 1 = USB chip ready
	#define USB_GPO_OE_BIT 			0x0008
	#define USB_INT0_BIT   			0x0010
	#define USB_INT1_BIT   			0x0020
	#define USB_INTX_OE_BIT 		0x0040
		
#define FPGA_CLOCKS_CTRL   FPGA_REG(3) // reg-3, R/W	
	#define FPGA_PLL_ML_BIT  			0x0001
    #define FPGA_PLL_SR0_BIT  			0x0001
	#define FPGA_PLL_MC_BIT  			0x0002
    #define FPGA_PLL_FS1_BIT  			0x0002
	#define FPGA_PLL_MD_BIT  			0x0004
    #define FPGA_PLL_FS0_BIT  			0x0004
	#define FPGA_WORD_CLK_PRESENT	    0x0008
	#define FPGA_AHCLKR2				0x0070
	#define FPGA_AHCLKR2_S0				0x0010
	#define FPGA_AHCLKR2_S1				0x0020
	#define FPGA_AHCLKR2_S2				0x0040
	#define FPGA_AHCLKX1				0x0380
	#define FPGA_AHCLKX1_S0				0x0080
	#define FPGA_AHCLKX1_S1				0x0100
	#define FPGA_AHCLKX1_S2				0x0200
	#define FPGA_AHCLKR0				0x1C00
	#define FPGA_AHCLKR0_S0				0x0400
	#define FPGA_AHCLKR0_S1				0x0800
	#define FPGA_AHCLKR0_S2				0x1000
	#define FPGA_AHCLKX0				0xE000
	#define FPGA_AHCLKX0_S0				0x2000
	#define FPGA_AHCLKX0_S1				0x4000
	#define FPGA_AHCLKX0_S2				0x8000

#define FPGA_USB_STATUS FPGA_REG(4) // reg-4, R
	#define USB_FIFO_IP0        0x0100
	#define USB_FIFO_IP1        0x0200
	#define USB_FIFO_IP2        0x0400
	#define USB_FIFO_IP3        0x0800
	#define USB_FIFO_IPn(n)     (USB_FIFO_IP0 << (n))

	#define USB_USBUP_IP            0x1000
	#define USB_GIP                 0x8000

#define FPGA_USB_ACK FPGA_REG(4) // reg-4, W
	#define USB_FIFO_CIP0       0x0001
	#define USB_FIFO_CIP1       0x0002
	#define USB_FIFO_CIP2       0x0004
	#define USB_FIFO_CIP3       0x0008
	#define USB_FIFO_CIPn(n)     (USB_FIFO_CIP0 << (n))

	#define USB_FIFO_PKE2       0x0040
	#define USB_FIFO_PKE3       0x0080
	#define USB_FIFO_PKEn(n)    (USB_FIFO_PKE2 << ((n) -2)) // n= 2..3

	#define USB_USBUP_CIP           0x0100
	#define USB_CGIP                0x8000
	
#define FPGA_FLASH_CTRL   FPGA_REG(5) // reg-5, R/W	
	#define FPGA_FLASH_A12  			0x0001
    #define FPGA_FLASH_A13  			0x0002
    #define FPGA_FLASH_A14  			0x0004
    #define FPGA_FLASH_A15  			0x0008
    #define FPGA_FLASH_A16  			0x0010
    #define FPGA_FLASH_A17  			0x0020
    #define FPGA_FLASH_A18  			0x0040
    #define FPGA_FLASH_A19  			0x0080
	#define FPGA_FLASH_A20  			0x0100
	#define FPGA_FLASH_A21  			0x0200
	#define FPGA_FLASH_NAND_RDY			0x0400
	
#define FPGA_ADC_DAC_CTRL   FPGA_REG(6) // reg-6, R/W	
	#define FPGA_ADC_FMT0           0x0001
	#define FPGA_ADC_FS0            0x0002
	#define FPGA_ADC_FS1            0x0004
	#define FPGA_DAC_FMT0           0x0010
	#define FPGA_DAC_MUTE           0x0020
	#define FPGA_DAC_DEM0           0x0040
    #define FPGA_DAC_DEM1           0x0080
    #define FPGA_DAC_FS0            0x0100
    #define FPGA_DAC_FS1            0x0200
	#define FPGA_AUDIO_MUTE         0x0400
	#define FPGA_SRC_READY          0x0800
	
#define FPGA_REVISION_ID  FPGA_REG(7)	// reg-7, R
	#define FPGA_REV_ID_MASK        0x00FF
	#define FPGA_PROD_ID_MASK       0xFF00

#define FPGA_DC_CTRL  FPGA_REG(8)	// reg-8, R/W
	#define FPGA_DC_A10          0x0001
    #define FPGA_DC_A11          0x0002
    #define FPGA_DC_A12          0x0004
    #define FPGA_DC_A13          0x0008
    #define FPGA_DC_A14          0x0010
    #define FPGA_DC_A15          0x0020
    #define FPGA_DC_A16          0x0040
    #define FPGA_DC_A17          0x0080
    #define FPGA_DC_A18          0x0100
    #define FPGA_DC_A19          0x0200
    #define FPGA_DC_EXT_INT_SEL  0x0400
    #define FPGA_DAC_DATA_IRQ_EN 0x0800

#define FPGA_DAC_SPI  FPGA_REG(9)	// reg-9, R/W

#define FPGA_BUTTON_STATUS  FPGA_REG(10)	// reg-10, R/W
    #define FPGA_DELTA_BUTTON1   0x0001
    #define FPGA_DELTA_BUTTON2   0x0002
    #define FPGA_BUTTON1         0x0004
    #define FPGA_BUTTON2         0x0008

#endif // _PADK_FPGA_H_
