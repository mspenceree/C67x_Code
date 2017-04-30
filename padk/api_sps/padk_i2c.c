
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
 * This drive is originaly from V2.0 of theLyrtech PADK Driver collection
 * and may have been modified (repaired) by MSpencer.
 *
 * File: padk_i2c.c
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <csl_i2c.h>
#include <soc.h>

#include "PADK.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/
static CSL_I2cHandle hI2c = NULL;
static CSL_I2cObj i2cObj;

/* 
 *  I2cDefaultClkSetup
 */
static CSL_I2cClkSetup I2cDefaultClkSetup = 
{ 
	0x0002,	// Prescalar to the input clock
//	0x00F4,	// Low time period of the clock
	0x0120,	// Low time period of the clock
	0x00F4	// High time period of the clock
//	0x01F4	// High time period of the clock
};

/* 
 *  I2cDefaultHwSetup
 */
static CSL_I2cHwSetup I2cDefaultHwSetup = 
{ 
    CSL_I2C_MODE_SLAVE,			// Master or Slave Mode
    CSL_I2C_DIR_RECEIVE,		// Transmitter Mode or Receiver Mode
	CSL_I2C_ADDRSZ_SEVEN,		// Addressing Mode
	CSL_I2C_STB_DISABLE,		// Start Byte Mode
	0x7f,	                	// Address of the own device
	CSL_I2C_ACK_ENABLE,	    	// NACK mode while receiver
	CSL_I2C_FREE_MODE_DISABLE,	// Run mode of I2C
	CSL_I2C_REPEAT_MODE_DISABLE,// Repeat Mode of I2C
	CSL_I2C_DLB_DISABLE,		// LoopBack mode of I2C (master tx-er only)
	CSL_I2C_FDF_DISABLE,		// Free Data Format of I2C
	CSL_I2C_IRS_ENABLE,	    	// I2C Reset Mode
	CSL_I2C_BCM_DISABLE,		// I2C Backward Compatibility Mode
	0x0,						// Interrupt Enable mask
	&I2cDefaultClkSetup			// Prescalar, Clock Low/High Time for Clock Setup
};


// Added by MSpencer:

#include <log.h>

/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;

#define MAX_WAIT_LOOP_COUNT	10000	// Max loop count while waiting for hardware
									// before returning with with timeout error.



/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/
static int PADK_I2cInit();
static void PADK_I2cRreset();

int timeout_error( unsigned int count );


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

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
far int I2C_WriteReg( int devAddr, unsigned char reg, unsigned char val )
{
	// First, verify if the I2C is initialized
	if ( hI2c == NULL )
	{
		if ( PADK_I2cInit() ) return -1; 
	}

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );

	// Wait for Master mode cleared
	while ( hI2c->regs->ICMDR & CSL_I2C_ICMDR_MST_MASK );

	// Set slave address
	hI2c->regs->ICSAR = devAddr;

	// Set the data count for TX
	hI2c->regs->ICCNT = 0x2;

    // Switch to master-transmit mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_TRX_MASK	// Transmit
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

	// Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) );

	// Set first word of data to send
	hI2c->regs->ICDXR = reg;	

	// Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) );

	// Set second word of data to send	
	hI2c->regs->ICDXR = val;	

	// Wait register access ready
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ARDY_MASK) );

    // Generate STOP
	hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );
	return 0;
}


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
far int I2C_WriteMem( int devAddr, unsigned short addr, unsigned char *buf, int n )
{
	// First, verify if the I2C is initialized
	if ( hI2c == NULL )
	{
		if ( PADK_I2cInit() ) return -1; 
	}

restart_writemem:

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );

	// Wait for Master mode cleared
	while ( hI2c->regs->ICMDR & CSL_I2C_ICMDR_MST_MASK );

	// Set slave address
	hI2c->regs->ICSAR = devAddr;

	// Set the data count for TX
	hI2c->regs->ICCNT = n+2;

    // Switch to master-transmit mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_TRX_MASK	// Transmit
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

	while( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK) );

    // Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) )
	{
	    if ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_NACK_MASK )
	    {
			// Generate STOP
			//hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

			// Check for Bus busy 
		    //while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );
			hI2c->regs->ICMDR = 0;
	        goto restart_writemem;
	    }
	}

	// Set first word of data to send
    hI2c->regs->ICDXR = addr>>8;	

    // Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) )
	{
	    if ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_NACK_MASK )
	    {
			// Generate STOP
			//hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

			// Check for Bus busy 
		    //while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );
			hI2c->regs->ICMDR = 0;
	        goto restart_writemem;
	    }
	}

	// Set first word of data to send
    hI2c->regs->ICDXR = addr;	

    while ( n-- )
    {
	    // Wait ready to TX
	    while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) );

	    // Set second word of data to send	
	    hI2c->regs->ICDXR = *buf++;	
    }

	// Wait register access ready
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ARDY_MASK) );

    // Generate STOP
	hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );
	return 0;
}

                              
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
 *		 0 				for success
 *		-1 				for can't initialize I2C failure
 *		-2, 3, -4 etc.	for a timeout error (see code)
 *      
 * Modifed by M.Spencer
 *
 ****************************************************************************/	
far int I2C_WriteVal( int devAddr, unsigned char val )
{

	int retry;
	unsigned int cnt;

// LOG_printf(&trace, "****** Entered I2C_WriteVal(), Device Adddres = 0x%x\n", devAddr );

	for( retry = 0; retry < 10; retry++)
	{
		cnt = 0;

		// First, verify if the I2C is initialized
		if ( hI2c == NULL )
		{
			if ( PADK_I2cInit() ) return -1; 
		}

		// Check for Bus busy 
	    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK )
			if( timeout_error( cnt++ ) ) return ( -2 );

		// Wait for Master mode cleared
		while ( hI2c->regs->ICMDR & CSL_I2C_ICMDR_MST_MASK )
			if( timeout_error( cnt++ ) ) return ( -3 );

		// Set slave address
	    hI2c->regs->ICSAR = devAddr;

		// Set the data count for TX
		hI2c->regs->ICCNT = 0x1;

		// Wait ready to TX
		while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) )
			if( timeout_error( cnt++ ) ) return ( -4 );

		// Set first word of data to send	
		hI2c->regs->ICDXR = val;	

	    // Switch to master-transmit mode
		hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
								| CSL_I2C_ICMDR_MST_MASK	// Master 
								| CSL_I2C_ICMDR_TRX_MASK	// Transmit
								| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

		// Wait for the transfer to complete
		while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ARDY_MASK) )
			if( timeout_error( cnt++ ) ) return ( -5 );

		// Verify if ACK return
		if ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_NACK_MASK) )
		{
			// Generate STOP
        	hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

        	// Check for Bus busy 
            while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK )
				if( timeout_error( cnt++ ) ) return ( -6 );

        	return 0;
		}

		// Generate STOP
		hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

	    // Close I2C Port
	    CSL_i2cClose( hI2c );
		hI2c = NULL;

	}

	return ( -10 );	// Too many loops without ACKnowlage
}


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
far int I2C_ReadReg( int devAddr, unsigned char reg, unsigned char *val )
{
	// First, verify if the I2C is initialized
	if ( hI2c == NULL )
	{
		if ( PADK_I2cInit() ) return -1; 
	}

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );

	// Wait for Master mode cleared
	while ( hI2c->regs->ICMDR & CSL_I2C_ICMDR_MST_MASK );

	// Set slave address
    hI2c->regs->ICSAR = devAddr;

	// Set the data count for TX
	hI2c->regs->ICCNT = 0x1;

    // Switch to master-transmit mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_TRX_MASK	// Transmit
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

	// Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) );

	// Set first word of data to send	
	hI2c->regs->ICDXR = reg;	

	// Set the data count for RX
	hI2c->regs->ICCNT = 0x1;

   	// Switch to master-receive mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

	// Waiting for the data reception
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICRRDY_MASK) );

    // Copy the data from Data Recieve register 
	*val = (unsigned char)hI2c->regs->ICDRR;	

	// Wait register access ready
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ARDY_MASK) );
                
    // Generate STOP
	hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );
	return 0;
}


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
far int I2C_ReadMem( int devAddr, unsigned short addr, unsigned char *buf, int n )
{
	// First, verify if the I2C is initialized
	if ( hI2c == NULL )
	{
		if ( PADK_I2cInit() ) return -1; 
	}

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );

	// Wait for Master mode cleared
	while ( hI2c->regs->ICMDR & CSL_I2C_ICMDR_MST_MASK );

	// Set slave address
	hI2c->regs->ICSAR = devAddr;

	// Set the data count for TX
	hI2c->regs->ICCNT = 2;

    // Switch to master-transmit mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_TRX_MASK	// Transmit
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

    // Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) );

	// Set the high byte of the address
    hI2c->regs->ICDXR = addr>>8;	

    // Wait ready to TX
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICXRDY_MASK) );

    // Set the low byte of the address
    hI2c->regs->ICDXR = addr;	

	// Set the data count for RX
	hI2c->regs->ICCNT = n;

    // Switch to master-receive mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

    while ( n-- )
    {
        // Waiting for the data reception
	    while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICRRDY_MASK) );

        // Copy the data from Data Recieve register 
	    *buf++ = (unsigned char)hI2c->regs->ICDRR;	
    }

	// Wait register access ready
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ARDY_MASK) );

    // Generate STOP
	hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK );
	return 0;
}


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
 *		 0 				for success
 *		-1 				for can't initialize I2C failure
 *		-2, 3, -4 etc.	for a timeout error (see code)
 *      
 * Modifed by M.Spencer
 *
 ****************************************************************************/	
far int I2C_ReadVal( int devAddr, unsigned char *val )
{

	unsigned int cnt = 0;


// LOG_printf(&trace, "****** Entered I2C_ReadVal(), Device Adddres = 0x%x\n", devAddr );


	// First, verify if the I2C is initialized
	if ( hI2c == NULL )
	{
		if ( PADK_I2cInit() ) return -1; 
	}

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK )
		if( timeout_error( cnt++ ) ) return ( -2 );

	// Wait for Master mode cleared
	while ( hI2c->regs->ICMDR & CSL_I2C_ICMDR_MST_MASK )
		if( timeout_error( cnt++ ) ) return ( -3 );

	// Set slave address
    hI2c->regs->ICSAR = devAddr;

	// Set the data count for RX
	hI2c->regs->ICCNT = 0x1;
   
   	// Switch to master-receive mode
	hI2c->regs->ICMDR = 	  CSL_I2C_ICMDR_STT_MASK	// Gen start cond
							| CSL_I2C_ICMDR_MST_MASK	// Master 
							| CSL_I2C_ICMDR_IRS_MASK;	// Out of reset

	// Wait register access ready
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ARDY_MASK) )
		if( timeout_error( cnt++ ) ) return ( -4 );

	// Waiting for the data reception
	while ( !(hI2c->regs->ICSTR & CSL_I2C_ICSTR_ICRRDY_MASK) )
		if( timeout_error( cnt++ ) ) return ( -5 );

    // Copy the data from Data Recieve register
	*val = (unsigned char)hI2c->regs->ICDRR;		
                
    // Generate STOP
	hI2c->regs->ICMDR |= CSL_I2C_ICMDR_STP_MASK;

	// Check for Bus busy 
    while ( hI2c->regs->ICSTR & CSL_I2C_ICSTR_BB_MASK )
		if( timeout_error( cnt++ ) ) return ( -6 );

	return 0;
}


/****************************************************************************
 *                             Local Functions                              *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *     	PADK_I2cInit
 *
 * PURPOSE:
 *      Initialise the I2C interface.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int PADK_I2cInit(); 
 *      
 * RETURN VALUE
 *		Error code
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/	
static int PADK_I2cInit()
{    
    Uint16     response;
	CSL_Status status = CSL_SOK;

    /* Clear local data structures */
    memset( &i2cObj, 0, sizeof(CSL_I2cObj) );

    /* Initialize I2C module */
    status = CSL_i2cInit( NULL );    
    hI2c = CSL_i2cOpen( &i2cObj, CSL_I2C_1, NULL, &status );
    if ( (status != CSL_SOK) || (hI2c == NULL) ) 
    {
		// Failed to open I2C interface
		return -1;
	}

    /* Reset I2C Registers */
    PADK_I2cRreset();
	status = CSL_i2cHwSetup( hI2c, &I2cDefaultHwSetup );
    if ( status != CSL_SOK ) 
    {
		// Failed to configure the I2C
		CSL_i2cClose( hI2c );
		hI2c = NULL; 
		return -1;
	}

    /* Enable I2C */
    status = CSL_i2cHwControl( hI2c, CSL_I2C_CMD_ENABLE, NULL );
    if ( status != CSL_SOK ) 
    {
		// Error while controling the I2C
		CSL_i2cClose( hI2c );
		hI2c = NULL; 
		return -1;
    }

     /* Waiting for hwcontrol command to get executed */
    do 
    {
        CSL_i2cGetHwStatus( hI2c, CSL_I2C_QUERY_RDONE, &response );

    } while( response != CSL_I2C_RESET_DONE ); 

	return 0;
}


/**************************************************************************** 
 *
 * NAME  
 *     	PADK_I2cRreset
 *
 * PURPOSE:
 *      Reset the I2C engine
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		void PADK_I2cRreset(); 
 *
 * RETURN VALUE
 *		none
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/	
static void PADK_I2cRreset()
{
	CSL_I2cConfig config = CSL_I2C_CONFIG_DEFAULTS;

    /* for configuring clk registers */
    CSL_i2cHwControl( hI2c, CSL_I2C_CMD_RESET, NULL );

    /* i2c registers set to default values */
    CSL_i2cHwSetupRaw( hI2c, &config );  

}


/**************************************************************************** 
 *
 * NAME  
 *     	timeout_error()
 *
 * PURPOSE:
 *      This function checks for a timeout in the I2C routines.
 * 		
 *
 * RETURN VALUE
 *		-1 for failure, 0 for success
 *      
 * by M.Spencer
 *
 ****************************************************************************/	
int timeout_error( unsigned int count )
{

	if( count >  MAX_WAIT_LOOP_COUNT )	// We had a timeout; return with error code
	{   
	    CSL_i2cClose( hI2c );	// Close I2C Port
		hI2c = NULL;
		return ( -1 );
	}
	else
	{
		return ( 0 );
	}
}

