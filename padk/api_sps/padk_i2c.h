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
 *  File        : PADK_I2C.h
 *  Description	: Utility functions for writing/reading on the I2C bus
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
#ifndef __PADK_I2C_H__
#define __PADK_I2C_H__


/**************************************************************************** 
 *
 * NAME  
 *     	I2C_WriteReg
 *
 * PURPOSE:
 *      This function writes a value to a register of the specified device
 *      using the I2C bus.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2C_WriteReg( int devAddr, unsigned char reg, unsigned char val ); 
 *
 *		devAddr	- (i)	Id of the device to write to
 *		reg     - (i)   Register 
 *		val     - (i)   Value
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2C_WriteReg( int devAddr, unsigned char reg, unsigned char val );


/**************************************************************************** 
 *
 * NAME  
 *     	I2C_WriteMem
 *
 * PURPOSE:
 *      This function writes N bytes to the specified device.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2C_WriteMem( int devAddr, unsigned short addr, unsigned char *buf, int n ); 
 *
 *		devAddr	- (i)	Id of the device to write to
 *		addr    - (i)   Address 
 *		buf     - (i)   Buffer of data to write
 *		n       - (i)   Number of bytes to write
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2C_WriteMem( int devAddr, unsigned short addr, unsigned char *buf, int n );


/**************************************************************************** 
 *
 * NAME  
 *     	I2C_WriteVal
 *
 * PURPOSE:
 *      This function send a byte to the specified device using the I2C bus.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2C_WriteVal( int devAddr, unsigned char val ); 
 *
 *		devAddr	- (i)	Id of the device to write to
 *		val     - (i)   Value
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2C_WriteVal( int devAddr, unsigned char val );


/**************************************************************************** 
 *
 * NAME  
 *     	I2C_ReadReg
 *
 * PURPOSE:
 *      This function reads the value of a register of the specified device
 *      using the I2C bus.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2C_ReadReg( int devAddr, unsigned char reg, unsigned char *val ); 
 *
 *		devAddr	- (i)	Id of the device to write to
 *		reg     - (i)   Register 
 *		val     - (o)   Value
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2C_ReadReg( int devAddr, unsigned char reg, unsigned char *val );


/**************************************************************************** 
 *
 * NAME  
 *     	I2C_ReadMem
 *
 * PURPOSE:
 *      This function reads N bytes from the specified device.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2C_ReadMem( int devAddr, unsigned short addr, unsigned char *buf, int n ); 
 *
 *		devAddr	- (i)	Id of the device to write to
 *		addr    - (i)   Address 
 *		buf     - (i)   Buffer of data to write
 *		n       - (i)   Number of bytes to write
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2C_ReadMem( int devAddr, unsigned short addr, unsigned char *buf, int n );


/**************************************************************************** 
 *
 * NAME  
 *     	I2C_ReadVal
 *
 * PURPOSE:
 *      This function read a byte from the specified device using the I2C bus.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2C_ReadVal( int devAddr, unsigned char *val ); 
 *
 *		devAddr	- (i)	Id of the device to write to
 *		val     - (o)   Value
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2C_ReadVal( int devAddr, unsigned char *val );


#endif //__PADK_I2C_H__
