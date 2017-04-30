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
 *  File        : PADK_USB.h
 *  Description	: CY7C68013 EZ-USB® FX2 USB Microcontroller High-Speed USB 
 *				  Peripheral Controller Driver
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
#ifndef __PADK_USB_H__
#define __PADK_USB_H__

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

/****************************************************************************
 *                                  Macros                                  *
 ****************************************************************************/

#define USB_NB_FIFOS             4  // number of fifo: 4
#define USB_MAX_FIFO_SIZE      511  // max size of internal buffer for each endpoints

// Associated FIFO number
#define USB_HOST2DSP_PIPE1 0	
#define USB_HOST2DSP_PIPE2 1
#define USB_DSP2HOST_PIPE1 2
#define USB_DSP2HOST_PIPE2 3

// hardware access macros:
#define USB_DISABLE_ALL()           (FPGA_USB_CONFIG = 0)
#define USB_ENABLE_FIFO(f)          (FPGA_USB_CONFIG |= USB_FIFO_En(f))
#define USB_DISABLE_FIFO(f)         (FPGA_USB_CONFIG &= ~USB_FIFO_En(f))
#define USB_IS_OUT_FIFO(f)          (FPGA_USB_CONFIG & USB_FIFO_OUTn(f) ? 1 : 0)
#define USB_ENABLE_FIFO_INT(f)      (FPGA_USB_CONFIG |= USB_FIFO_IEn(f))
#define USB_DISABLE_FIFO_INT(f)     (FPGA_USB_CONFIG &= ~USB_FIFO_IEn(f))
#define USB_ENABLE_USBUP_INT()      (FPGA_USB_CONFIG |= USB_USBUP_IE)
#define USB_DISABLE_USBUP_INT()     (FPGA_USB_CONFIG &= ~USB_USBUP_IE)
#define USB_ENABLE_G_INT()          (FPGA_USB_CONFIG |= USB_GIE)
#define USB_DISABLE_G_INT()         (FPGA_USB_CONFIG &= ~USB_GIE)

#define USB_READ_PKECNT(f)          (FPGA_USB_PKECNT(f))
#define USB_WRITE_PKECNT(f,d)       (FPGA_USB_PKECNT(f) = (d))
#define USB_WRITE_THRESHOLD(f,d)    (FPGA_USB_THRESHOLD(f) = (d))
#define USB_READ_SIZE(f)            (FPGA_USB_SIZE(f))
     
#define USB_WRITE_DATA_BYTE(f,d)    (*FPGA_USB_DATA_BYTE(f) = (d))
#define USB_WRITE_DATA_HWORD(f,d)   (*FPGA_USB_DATA_HWORD(f) = (d))
#define USB_WRITE_DATA_WORD(f,d)    (*FPGA_USB_DATA_WORD(f) = (d))
#define USB_READ_DATA_BYTE(f)       (*FPGA_USB_DATA_BYTE(f))
#define USB_READ_DATA_WORD(f)       (*FPGA_USB_DATA_WORD(f))
#define USB_DATA_ADDR_BYTE(f)       (FPGA_USB_DATA_BYTE(f))
#define USB_DATA_ADDR_WORD(f)       (FPGA_USB_DATA_WORD(f))

#define USB_GET_FIFO_IP(f)			(FPGA_USB_STATUS & USB_FIFO_IPn(f) ? 1 : 0)
#define USB_GET_USBUP_IP()			(FPGA_USB_STATUS & USB_USBUP_IP ? 1 : 0)
#define USB_GET_G_IP()				(FPGA_USB_STATUS & USB_GIP ? 1 : 0)

#define USB_WRITE_PKTEND(f)         (FPGA_USB_ACK = USB_FIFO_PKEn(f))
#define USB_CLEAR_FIFO_IP(f)        (FPGA_USB_ACK = USB_FIFO_CIPn(f))
#define USB_CLEAR_USBUP_IP()        (FPGA_USB_ACK = USB_USBUP_CIP)
#define USB_CLEAR_G_IP()            (FPGA_USB_ACK = USB_CGIP)

#define USB_READ_PROD_REV_ID()      (FPGA_REV_ID)

#define USB_IS_ACTIVE()             (FPGA_USB_CTRL & USB_READY)
#define USB_MAX_PACKET_SIZE()       ((FPGA_USB_CTRL & USB_2) ? 512 : 64) // valid if USB_IS_ACTIVE() is non zero


/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct {

	int irq_en;				// Global USB Interrupt Enable
	int usbup_irq_en;		// Active USB connection IRQ Enable

	int h2d1_en;	   		// Host to DSP Pipe #1 Enable
	unsigned char h2d1_thrs;// Host to DSP Pipe #1 FIFO Threshold (0-510)
	int h2d1_irq_en;		// Host to DSP Pipe #1 FIFO IRQ Enable

	int d2h1_en;	   		// Host to DSP Pipe #1 Enable
	unsigned char d2h1_thrs;// Host to DSP Pipe #1 FIFO Threshold (0-510)
	int d2h1_irq_en;		// Host to DSP Pipe #1 FIFO IRQ Enable

	int h2d2_en;	   		// DSP to Host Pipe #2 Enable
	unsigned char h2d2_thrs;// DSP to Host Pipe #2 FIFO Threshold (0-510)
	int h2d2_irq_en;		// Host to DSP Pipe #1 FIFO IRQ Enable

	int d2h2_en;		   	// DSP to Host Pipe #2 Enable
	unsigned char d2h2_thrs;// DSP to Host Pipe #2 FIFO Threshold (0-510)
	int d2h2_irq_en;		// Host to DSP Pipe #1 FIFO IRQ Enable

} USB_Params;

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define USB_FIFO_ENABLE		1
#define USB_FIFO_DISABLE	0

#define USB_IRQ_ENABLE		1
#define USB_IRQ_DISABLE		0


// Default params
#define USB_DEFAULT_PARAMS {    		\
	USB_IRQ_DISABLE,                 	\
    USB_IRQ_DISABLE,                   	\
	USB_FIFO_ENABLE,                   	\
	0,              			     	\
	USB_IRQ_DISABLE,                   	\
	USB_FIFO_ENABLE,                   	\
	0,              			     	\
	USB_IRQ_DISABLE,                   	\
	USB_FIFO_ENABLE,                   	\
	0,              			     	\
	USB_IRQ_DISABLE,                   	\
	USB_FIFO_ENABLE,                   	\
	0,              			     	\
	USB_IRQ_DISABLE                   	\
}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      USB_Init
 *
 * PURPOSE:
 *		Initialize the USB controller and set the AMUTEIN0 pin to be used as 
 *		external interrupt for the USB IRQs.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Init( 
 *				USB_Params *param,		// (i) Initialisation parameters
 *		);
 *
 * RETURN VALUE
 *		Error code (-1:error, 0:success)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int USB_Init( USB_Params *param );


/**************************************************************************** 
 *
 * NAME  
 *      USB_Reset
 *
 * PURPOSE:
 *		Initialize the USB controller with the default configuration
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Reset(); 
 *
 * RETURN VALUE
 *		Error code (-1:error, 0:success)
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int USB_Reset();


/**************************************************************************** 
 *
 * NAME  
 *      USB_Read
 *
 * PURPOSE:
 *		This function read up to N bytes from the USB controller. The call can
 *		be blocking or not. 
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Read(
 *			int fifo,				// (i) USB FIFO [0 or 1]
 *			unsigned char *buff,    // (o) Pointer to the destination buffer
 *			int N, 			        // (i) Number of bytes to read 
 *			int wait 		        // (i) Blocking or not 
 *		);
 *
 * RETURN VALUE
 *		Error code or the number of bytes received (-1:error)
 *
 * REFERENCE
 *
 * NOTE     
 *		This function is just a simple example of how to interface the USB
 *		controller. The dMAX controller and the IRQ can be used to achieve
 *		better performance.
 * 		
 ****************************************************************************/
far int USB_Read(
			int fifo,				// (i) USB FIFO [0 or 1]
			unsigned char *buff,    // (o) Pointer to the destination buffer
 			int N, 			        // (i) Number of bytes to read 
 			int wait ); 		    // (i) Blocking or not 


/**************************************************************************** 
 *
 * NAME  
 *      USB_Write
 *
 * PURPOSE:
 *		This function write up to N bytes to the USB controller. The call can
 *		be blocking or not. If the call is blocking, a packet end will be 
 *		generated at the end of the data. In non-blocking mode, the caller
 *		is reponsible to generate the packet end if needed.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int USB_Write(
 *			int fifo,				// (i) USB FIFO [2 or 3]
 *			unsigned char *buff,    // (o) Pointer to the source buffer
 *			int N, 			        // (i) Number of bytes to write
 *			int wait 		        // (i) Blocking or not 
 *		);
 *
 * RETURN VALUE
 *		Error code or the number of byte written (-1:error)
 *
 * REFERENCE
 *
 * NOTE     
 *		This function is just a simple example of how to interface the USB
 *		controller. The dMAX controller and the IRQ can be used to achieve
 *		better performance.
 * 		
 ****************************************************************************/ 
far int USB_Write(
			int fifo,				// (i) USB FIFO [0 or 1]
			unsigned char *buff,    // (o) Pointer to the destination buffer
 			int N, 			        // (i) Number of bytes to read 
 			int wait );		        // (i) Blocking or not 


#endif // __PADK_USB_H__
