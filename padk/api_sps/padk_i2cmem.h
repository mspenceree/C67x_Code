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
 *  File        : PADK_I2CMEM.h
 *  Description	: Utility functions for writing/reading the I2C memory
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
#ifndef __PADK_I2CMEM_H__
#define __PADK_I2CMEM_H__

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *     	I2CMEM_Write
 *
 * PURPOSE:
 *      This function writes a vector of bytes in the I2C EEPROM.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2CMEM_Write( unsigned short ofst, unsigned char buf[], int n ); 
 *
 *		ofst  - (i)	Offset where to write to
 *		buf   - (i) Data to be written 
 *		n     - (i) Number of bytes to be written
 *
 * RETURN VALUE
 *		Return the number of bytes written
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2CMEM_Write( unsigned short offset, unsigned char buf[], int n );


/**************************************************************************** 
 *
 * NAME  
 *     	I2CMEM_Read
 *
 * PURPOSE:
 *      This function reads vector of bytes in the I2C EEPROM.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int I2CMEM_Read( unsigned short ofst, unsigned char buf[], int n ); 
 *
 *		ofst  - (i)	Offset where to read from
 *		buf   - (o) Data to be read
 *		n     - (i) Number of bytes to be read
 *
 * RETURN VALUE
 *		Return the number of bytes read
 *      
 * REFERENCE
 *
 ****************************************************************************/	
far int I2CMEM_Read( unsigned short offset, unsigned char buf[], int n );


#endif	// __PADK_I2CMEM_H__

