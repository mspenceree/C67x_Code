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
 *  File        : PADK_FLASH.c
 *  Description	: CFI Flash Memory driver.
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
#include <stdlib.h>

#include "PADK.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/
#define FLASH_TIMEOUT				3000000

// Generic CFI Command Set
#define CFI_QUERY					0x9898

// Command Set #1 (Intel/Sharp Extended Command Set)
#define CMDSET1_RESET				0xFFFF
#define CMDSET1_READID				0x9090
#define CMDSET1_PROGRAMSETUP		0x4040
#define CMDSET1_ERERASESETUP		0x2020
#define CMDSET1_ERERASECONFIRM		0xD0D0

// Command Set #2 (AMD/Fujitsu Standard Command Set)
#define CMDSET2_RESET				0xF0F0
#define CMDSET2_UNLOCK1				0xAAAA
#define CMDSET2_UNLOCK2				0x5555
#define CMDSET2_UNLOCKBYPASS		0x2020
#define CMDSET2_UNLOCKBYPASSRESET1	0x9090
#define CMDSET2_UNLOCKBYPASSRESET2	0x0000
#define CMDSET2_PROGRAM				0xA0A0
#define CMDSET2_SETUP				0x8080
#define CMDSET2_AUTOSELECT			0x9090
#define CMDSET2_SECTORERASE			0x3030

#define	ID_AMD_AM29LV160BB			0x00012249
#define	ID_AMD_AM29LV160BT			0x000122C4
#define	ID_AMD_AM29LV800BB			0x0001225B
#define	ID_AMD_AM29LV800BT			0x000122DA 
#define	ID_AMD_AM29LV640D			0x000122D7
#define	ID_AMD_AM29LV640MU			0x0001227E
#define	ID_ST_M29W160BB				0x00202249
#define	ID_ST_M29W160BT				0x002022C4

#define CMDSET_NONE					0x0000		// NULL
#define CMDSET_INTEL_SHARP_EXT		0x0001		// Intel/Sharp Extended Command Set
#define CMDSET_AMD_FUJITSU_STD		0x0002		// AMD/Fujitsu Standard Command Set
#define CMDSET_INTEL_STD			0x0003		// Intel Standard Command Set
#define CMDSET_AMD_FUJITSU_EXT		0x0004		// AMD/Fujitsu Extended Command Set
#define CMDSET_MITSUBISHI_STD		0x0100		// Mitsubishi Standard Command Set
#define CMDSET_MITSUBISHI_EXT		0x0101		// Mitsubishi Extended Command Set
#define CMDSET_SST					0x0102		// Page Write Command Set

#define FLASH_MAX_SECTORS  		 	128      	// Maximum number of sectors supported (AML640MU)

/****************************************************************************
 *                                  Types                                   *
 ****************************************************************************/
typedef struct 
{
	unsigned base;
	unsigned size;
} t_flash_sector;

typedef struct 
{
	unsigned cmdset;
	unsigned size;
	unsigned nsec;
	
	int (* FlashEraseSector)( unsigned base );
	int (* FlashProgram16)( unsigned base, unsigned short *data, unsigned length );
	
	t_flash_sector sec[FLASH_MAX_SECTORS];
} t_flash_geometry;

/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                            Private Functions                             *
 ****************************************************************************/
/**************************************************************************** 
 *
 * NAME  
 *      IntelExtEraseSector
 *
 * PURPOSE:
 *      Erase a complete sector of the FLASH memory usign the Intel
 * 		Extended Command Set
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int IntelExtEraseSector( unsigned base );
 *
 *      offset - (i) offset of the base address of the sector (word16)
 *
 * REFERENCE
 *      Intel Common Flash Memory Interface and Command Sets, AN-646, April 2000
 *     	Intel Advanced+ Boot Block Flash Memory (C3) datasheet, May 2003
 *
 * ASSUMPTION
 * 		none
 *
 ****************************************************************************/
static int IntelExtEraseSector( unsigned offset )
{
	unsigned timeout;

	// Write BLOCK ERASE/CONFIRM Command
	FLASH_WRITE_WORD( offset, CMDSET1_ERERASESETUP );
	FLASH_WRITE_WORD( offset, CMDSET1_ERERASECONFIRM );	

	// Wait end of Erase Sector operation	
	timeout = FLASH_TIMEOUT;
	while( !(FLASH_READ_WORD(offset) & 0x00000080) ) 
	{					
		if( !--timeout) 
		{	
			// Error timeout
			return FLASH_ERROR_ERASE_TO; 					
   		}									
	}										

	// Success
	return FLASH_ERROR_NONE;
}

/**************************************************************************** 
 *
 * NAME  
 *      IntelExtProgram16
 *
 * PURPOSE:
 *      Program some 16-bit words in the FLASH memory usign the Intel
 * 		Extended Command Set
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int IntelExtProgram16( unsigned addr, unsigned *data, unsigned length );
 *
 *      offset - (i) offset in the FLASH memory (word16)
 *      data   - (i) pointer to the buffer containing the data to program
 *      length - (i) number of words in the buffer (word16)
 *
 * REFERENCE
 *      Intel Common Flash Memory Interface and Command Sets, AN-646, April 2000
 *     	Intel Advanced+ Boot Block Flash Memory (C3) datasheet, May 2003
 *
 * ASSUMPTION
 * 		none
 *
 ****************************************************************************/
static int IntelExtProgram16( unsigned offset, unsigned short *data, unsigned length )
{
	unsigned timeout;

	while ( length-- )
	{
		FLASH_WRITE_WORD( offset, CMDSET1_PROGRAMSETUP );
		FLASH_WRITE_WORD( offset, *data );
	
		// Wait end of Erase Sector operation	
		timeout = FLASH_TIMEOUT;
		while( FLASH_READ_WORD(offset) != *data ) 
		{					
			if( !--timeout) 
			{	
				// Reset
				FLASH_WRITE_WORD( offset, CMDSET1_RESET );

				// Error timeout
				return FLASH_ERROR_WRITE_TO; 					
	   		}									
		}										
		data++;
		offset++;
	}
	
	// Reset
	FLASH_WRITE_WORD( offset, CMDSET1_RESET );

	// Success
	return FLASH_ERROR_NONE;
}

/**************************************************************************** 
 *
 * NAME  
 *      AmdStdEraseSector
 *
 * PURPOSE:
 *      Erase a complete sector of the FLASH memory unsign the AMD
 * 		Standard Command Set
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int AmdStdEraseSector( unsigned base );
 *
 *      offset - (i) offset of the sector (16-bit words)
 *
 * REFERENCE
 *      AMD Am29LV160B datasheet, publication #22358, rev: B, November 10, 2000
 *      AMD Common Flash Memory Interface Specification, Release 2.0, December 1, 2001
 *
 * ASSUMPTION
 * 		none
 *
 ****************************************************************************/
static int AmdStdEraseSector( unsigned offset )
{
	unsigned timeout;

	// Write ERASE SECTOR Command
	FLASH_WRITE_WORD( 0x0555, CMDSET2_UNLOCK1 );
	FLASH_WRITE_WORD( 0x02AA, CMDSET2_UNLOCK2 );	
	FLASH_WRITE_WORD( 0x0555, CMDSET2_SETUP );
	FLASH_WRITE_WORD( 0x0555, CMDSET2_UNLOCK1 );
	FLASH_WRITE_WORD( 0x02AA, CMDSET2_UNLOCK2 );	
	FLASH_WRITE_WORD( offset, CMDSET2_SECTORERASE );
	
	// Wait end of Erase Sector operation	
	timeout = FLASH_TIMEOUT;
	while( FLASH_READ_WORD(offset) != 0xFFFF ) 
	{					
		if( !--timeout) 
		{	
			// Error timeout
			return FLASH_ERROR_ERASE_TO; 					
   		}									
	}										

	// Success
	return FLASH_ERROR_NONE;
}

/**************************************************************************** 
 *
 * NAME  
 *      AmdStdProgram16
 *
 * PURPOSE:
 *      Program some 16-bit words in the FLASH memory usign the AMD
 * 		Standard Command Set
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int AmdStdProgram16( unsigned addr, unsigned *data, unsigned length );
 *
 *      offset - (i) offset in the FLASH memory (16-bit words)
 *      data   - (i) pointer to the buffer containing the data to program
 *      length - (i) number of words in the buffer (16-bit words)
 *
 * REFERENCE
 *      AMD Am29LV160B datasheet, publication #22358, rev: B, November 10, 2000
 *      AMD Common Flash Memory Interface Specification, Release 2.0, December 1, 2001
 *
 * ASSUMPTION
 * 		none
 *
 ****************************************************************************/
static int AmdStdProgram16( unsigned offset, unsigned short *data, unsigned length )
{
	volatile unsigned var;
	unsigned timeout;
	while ( length-- )
	{
		// Put in PROGRAM mode
		FLASH_WRITE_WORD( 0x0555, CMDSET2_UNLOCK1 );
		FLASH_WRITE_WORD( 0x02AA, CMDSET2_UNLOCK2 );	
		FLASH_WRITE_WORD( 0x0555, CMDSET2_PROGRAM );
		FLASH_WRITE_WORD( offset, *data );

		// Wait end of Program operation	
		timeout = FLASH_TIMEOUT;
		while( FLASH_READ_WORD(offset) != *data ) 
		{					
			if( !--timeout) 
			{	
				// Reset
				FLASH_WRITE_WORD( offset, CMDSET2_RESET );

				// Error timeout
				return FLASH_ERROR_WRITE_TO; 					
	   		}									
		}										
		data++;
		offset++;
	}
	
	// Reset
	FLASH_WRITE_WORD( offset, CMDSET2_RESET );

	// Success
	return FLASH_ERROR_NONE;
}

/**************************************************************************** 
 *
 * NAME  
 *      FlashGetID
 *
 * PURPOSE:
 *      This function retreives the FLASH Manufacturer ID and the Device ID.
 * 		This function works for devices using the Intel Extended Command Set
 * 		and the AMD Standard Command Set.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int DbFlashGetID( unsigned *id );
 *
 *      id - (o) [bit 31-16] Manufacturer ID
 *               [bit 15- 0] Device ID
 *
 * REFERENCE
 *      AMD Am29LV160D datasheet, publication #22358, rev: B, November 10, 2000
 *      AMD Common Flash Memory Interface Specification, Release 2.0, December 1, 2001
 *      Intel Common Flash Memory Interface and Command Sets, AN-646, April 2000
 *     	Intel Advanced+ Boot Block Flash Memory (C3) datasheet, May 2003
 *
 * ASSUMPTION
 * 		none
 *
 ****************************************************************************/	
static int FlashGetID( unsigned *id )
{
	unsigned short devId1;
	unsigned short manId1;

	// RESET Command
	FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
	FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );

	// AUTOSELECT Command	
	FLASH_WRITE_WORD( 0x0555, CMDSET2_UNLOCK1 );
	FLASH_WRITE_WORD( 0x02AA, CMDSET2_UNLOCK2 );
	FLASH_WRITE_WORD( 0x0555, CMDSET2_AUTOSELECT );	

	// Read the Manufacturer ID of the FLASH
	manId1 = FLASH_READ_WORD( 0x0000 );
	
	// Read the Device ID of the FLASH
	devId1 = FLASH_READ_WORD( 0x0001 );
	
	// RESET Command
	FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
	FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );

	*id = (((unsigned)manId1) << 16) | (devId1);
	return FLASH_ERROR_NONE;
}

/**************************************************************************** 
 *
 * NAME  
 *      FlashGetDeviceGeometry
 *
 * PURPOSE:
 *      This function retreives the geometry of the FLASH memory device.
 * 		This function works for devices using the Intel Extended Command Set
 * 		and the AMD Standard Command Set.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int FlashGetDeviceGeometry( t_flash_geometry *geo );
 *
 *      geo - (o) structure describing the geometry of the FLASH memory
 *
 * REFERENCE
 *      AMD Am29LV160D datasheet, publication #22358, rev: B, November 10, 2000
 *      AMD Common Flash Memory Interface Specification, Release 2.0, December 1, 2001
 *      Intel Common Flash Memory Interface and Command Sets, AN-646, April 2000
 *     	Intel Advanced+ Boot Block Flash Memory (C3) datasheet, May 2003
 *
 * ASSUMPTION
 * 		none
 *
 ****************************************************************************/	
static int FlashGetDeviceGeometry( t_flash_geometry *geo )
{
	int i,j;
	int res;
	unsigned id;
	unsigned base;
	unsigned region;	
	
	// Get the Manufacturer and the Device IDs
	if ( (res=FlashGetID(&id)) != FLASH_ERROR_NONE )
	{
		// Error
		return res;
	}
	
	// RESET Command
	FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
	FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );

	// Enter QUERY Mode
	FLASH_WRITE_WORD( 0x0055, CFI_QUERY );

	// Verify if it is a CFI device
	if (   (FLASH_READ_WORD(0x10) != 0x0051)	// Q
		|| (FLASH_READ_WORD(0x11) != 0x0052)	// R
		|| (FLASH_READ_WORD(0x12) != 0x0059) )	// Y

	{
		// RESET Command
		FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
		FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );
	
		// Error, it's not CFI devices
		return FLASH_ERROR_DEV_UNSUPPORTED;
	}	
	
	// Device Size (2^n bytes)
	geo->size = 1 << (FLASH_READ_WORD(0x27)&0x000000FF);
	
	// Get the number of erase region
	region = FLASH_READ_WORD(0x2C) & 0x000000FF;

	// Init Command Set
	//geo->cmdset = pFlash[0x28 & 0x000000FF; 
	geo->cmdset = FLASH_READ_WORD(0x13) & 0x000000FF; 


	switch ( geo->cmdset )
	{
		case CMDSET_INTEL_SHARP_EXT:	// Intel/Sharp Extended Command Set
		{
			for ( i=0, geo->nsec=0, base=0; i<region; i++ )
			{
				//unsigned num  =   ((pFlash[(0x2E+4*i) & 0x0000FFFF) << 8)
				unsigned num  =  ((FLASH_READ_WORD((0x2E)+(i<<2)) & 0x0000FFFF) << 8)
								+ (FLASH_READ_WORD((0x2D+4*i))    & 0x0000FFFF) 
								+ 1;
				unsigned size  =  ((FLASH_READ_WORD((0x30+4*i)) & 0x0000FFFF) << 17)
								+ ((FLASH_READ_WORD((0x2F+4*i)) & 0x0000FFFF) <<  9);
				
				for ( j=0; j<num; j++, geo->nsec++ )
				{
					geo->sec[geo->nsec].base = base;
					geo->sec[geo->nsec].size = size;
					
					base += size;
				}
			}
	
			geo->FlashEraseSector = IntelExtEraseSector;
			geo->FlashProgram16   = IntelExtProgram16;					
			break;
		}
		
		case CMDSET_AMD_FUJITSU_STD:	// AMD/Fujitsu Standard Command Set
		{
			// The AM29LV160B is a special case, 
			// it has not a field specifying if it is 
			// a top boot or a bottom boot device
			switch ( id )
			{
				case ID_AMD_AM29LV160BB:				
				{	
AmdUniform:
AmdBottomBoot:		// Bottom Boot device					
					for ( i=0, geo->nsec=0, base=0; i<region; i++ )
					{
						unsigned num  =  ((FLASH_READ_WORD((0x2E)+(i<<2)) & 0x0000FFFF) << 8)
										+ (FLASH_READ_WORD((0x2D+4*i))    & 0x0000FFFF) 
										+ 1;
						unsigned size  =  ((FLASH_READ_WORD((0x30+4*i)) & 0x0000FFFF) << 16)
										+ ((FLASH_READ_WORD((0x2F+4*i)) & 0x0000FFFF) <<  8);
						
						for ( j=0; j<num; j++, geo->nsec++ )
						{
							geo->sec[geo->nsec].base = base;
							geo->sec[geo->nsec].size = size;
							
							base += size;
						}
					}

					geo->FlashEraseSector = AmdStdEraseSector;
					geo->FlashProgram16   = AmdStdProgram16;
					break;
				} 
                
                case ID_AMD_AM29LV160BT:
				{
AmdTopBoot:			// Top Boot device		
					for ( i=region-1, geo->nsec=0,base=0; i>=0; i-- )
					{
						//unsigned num  =   ((pFlash[(0x2E+4*i)] & 0x0000FFFF) << 8)
						unsigned num  =  ((FLASH_READ_WORD((0x2E)+(i<<2)) & 0x0000FFFF) << 8)
										+ (FLASH_READ_WORD((0x2D+4*i))    & 0x0000FFFF) 
										+ 1;
						unsigned size  =  ((FLASH_READ_WORD((0x30+4*i)) & 0x0000FFFF) << 16)
										+ ((FLASH_READ_WORD((0x2F+4*i)) & 0x0000FFFF) <<  8);
				
						for ( j=0; j<num; j++, geo->nsec++ )
						{
							geo->sec[geo->nsec].base = base;
							geo->sec[geo->nsec].size = size;
							
							base += size;
						}
					}		
					geo->FlashEraseSector = AmdStdEraseSector;
					geo->FlashProgram16   = AmdStdProgram16;
					break;
				}
				default:
				{
					unsigned extQueryTbl = FLASH_READ_WORD(0x15) & 0x0000FFFF;
					if ( (region == 1) )
					{
						goto AmdUniform;			
					}
					else if ( (extQueryTbl != 0x0000) && (region > 1) )
					{					
						// Read the Primary Algorithm extended Query Table to 
						// determine if it is a Bottom Boot or a Top Boot device
						switch ( (FLASH_READ_WORD((extQueryTbl+0xF)) & 0x000000FF) ) 
						{
							case 2:	// Bottom boot device
								goto AmdBottomBoot;
							
							case 3:	// Top boot device
								goto AmdTopBoot;							

							case 4:	// Uniform, Bottom WP Protect
							case 5:	// Uniform, Top WP Protect
							default:												
								// RESET Command
								FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
								FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );

								// Unsupported device
								return FLASH_ERROR_DEV_UNSUPPORTED;							
						}
					}
				}
			}
			break;
		}	
		default:
		{
			// RESET Command
			FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
        	FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );

			// Unsupported Command Set
			return FLASH_ERROR_DEV_UNSUPPORTED;
		}
	}

	// RESET Command
	FLASH_WRITE_WORD( 0x0000, CMDSET1_RESET );
	FLASH_WRITE_WORD( 0x0000, CMDSET2_RESET );

	// Success	
	return FLASH_ERROR_NONE;
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      FlashWrite
 *
 * PURPOSE:
 *      This function write any number of bytes of data in the FLASH memory
 * 		at any offset. Only the data being written is mofified, the remainder
 * 		of the data in the FLASH memory is preserved.
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int FLASH_Write( unsigned offset, unsigned char *data, unsigned length );
 *
 *      offset - (i) offset in the FLASH memory where to write the data
 *      data   - (i) pointer to the data to be written
 *      length - (i) length in bytes of the data buffer.
 *
 * REFERENCE
 *
 * ASSUMPTION
 * 		This function allocates (malloc) a buffer in the HEAP memory that is 
 * 		twice the size of the maximum sector size of the FLASH memory.
 * 		(128 kB for the Am29LV160D)
 *
 ****************************************************************************/	
int FLASH_Write( unsigned offset, unsigned char *data, unsigned length )
{
    int i;
	int res;
	int sec;   
	unsigned CurSecOffset;
	unsigned short *buffer_addr;
	unsigned short *buffer = NULL;
	t_flash_geometry geo;
	
	CurSecOffset = offset;
	
	// Get Device Geometry. This structure is recreated 
	// each time a write is done to minimize the memory footprint
	if ( (res=FlashGetDeviceGeometry(&geo)) != FLASH_ERROR_NONE )
	{
		return res;
	}
	
	while ( length > 0 )
	{
		// Find the sector associated with offset
		if ( offset > geo.size )
		{
			// Error, offset exceed the FLASH memory size
			return FLASH_ERROR_OVERFLOW;
		}
		
		for ( sec=geo.nsec-1; sec>=0; sec-- )
		{
			if ( offset >= geo.sec[sec].base )
				break;
		}

		// Allocate a buffer to contain the data in the FLASH 
		// memory while the secteor is being erased.
		// WARNING: the address returned by malloc can be byte aligned ?!!!
		buffer_addr = malloc( geo.sec[sec].size + 3 );
		if ( buffer_addr == NULL )
		{
			// Memory error
			return FLASH_ERROR_NOMEMORY;
		}
		buffer = (unsigned short *)(((unsigned)buffer_addr+3) & 0xFFFFFFFE);
		
		// copy data from the flash memory to the buffer
//		memcpy( buffer, (const void *)(geo.sec[sec].base>>1]), geo.sec[sec].size );
        for ( i=0; i<geo.sec[sec].size>>1; i++ )     
            ((unsigned short *)buffer)[i] = FLASH_READ_WORD( (geo.sec[sec].base>>1)+i );

		// put data to program in flash in the buffer
		memcpy( &buffer[(offset-geo.sec[sec].base)>>1], data, 
				  (length>(geo.sec[sec].size-CurSecOffset)) ? (geo.sec[sec].size-CurSecOffset) : length );
		data   += (length>(geo.sec[sec].size-CurSecOffset)) ? (geo.sec[sec].size-CurSecOffset) : length;
		length -= (length>(geo.sec[sec].size-CurSecOffset)) ? (geo.sec[sec].size-CurSecOffset) : length;
        offset += (geo.sec[sec].size-CurSecOffset);       
        
		// Erase the sector
		if ( (res=(*geo.FlashEraseSector)(geo.sec[sec].base>>1)) != FLASH_ERROR_NONE )
		{
			// Free the memory allocated for the buffer
			free( buffer_addr );
		
			//Return the error code
			return res;
		}

		// Program the sector
		if ( (res=(*geo.FlashProgram16)(geo.sec[sec].base>>1, buffer, geo.sec[sec].size>>1)) != FLASH_ERROR_NONE )
		{
			// Free the memory allocated for the buffer
			free( buffer_addr );
		
			//Return the error code
			return res;
		}
		
		// Free the memory allocated for the buffer
		free( buffer_addr ); 
		CurSecOffset = 0;
	}
			
	// Success	
	return FLASH_ERROR_NONE;
}


/**************************************************************************** 
 *
 * NAME  
 *      FlashRead
 *
 * PURPOSE:
 *      This function reade any number of bytes of data in the FLASH memory
 * 		at any offset. 
 * 		
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int FLASH_Read( unsigned offset, unsigned char *data, unsigned length );
 *
 *      offset - (i) offset in the FLASH memory where to read the data
 *      data   - (o) buffer pointer
 *      length - (i) length in bytes to read.
 *
 * REFERENCE
 *
 * ASSUMPTION
 *
 ****************************************************************************/	
far int FLASH_Read( unsigned offset, unsigned char *data, unsigned length )
{
    while ( length-- )
    {
        // Set Page pointer
        FPGA_REG(5) = offset >> 12;

        // Read the data
        *data++ = ((volatile unsigned char *)FPGA_FLASH)[offset++];
    }      
    return 0;
}
