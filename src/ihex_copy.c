#include "cintelhex.h"

#include <stdio.h>

void ihex_set_error(ihex_error_t errno, char* error);

int ihex_mem_copy(ihex_recordset_t *rs, void* dst, ulong_t n)
{
	int      r;
	uint_t   i, j;
	uint32_t offset = 0x00, address = 0x00;
	char*    last_error = NULL;
	
	ihex_rdata_t d = (ihex_rdata_t) dst;
	
	if ((r = ihex_mem_zero(dst, n)) != 0)
	{
		return r;
	}
	
	for (i = 0; i < rs->ihrs_count; i ++)
	{
		address = offset + rs->ihrs_records[i].ihr_address;
		
		if (address >= n)
		{
			char* e = malloc(512);
			sprintf(e, "Address 0x%08x is out of range.\n", address);
			
			ihex_set_error(IHEX_ERR_ADDRESS_OUT_OF_RANGE, e);
			return IHEX_ERR_ADDRESS_OUT_OF_RANGE;
		}
		
		switch (rs->ihrs_records[i].ihr_type)
		{
			case IHEX_DATA:
				for (j = 0; j < rs->ihrs_records[i].ihr_length; j ++)
				{
					d[address + j] = rs->ihrs_records[i].ihr_data[j];
					
					#ifdef IHEX_DEBUG
					printf("%08x -> %02x\n", address + j, rs->ihrs_records[i].ihr_data[j]);
					#endif
				}
				break;
			case IHEX_EOF:
				if (i < rs->ihrs_count - 1)
				{
					last_error = malloc(512);
					sprintf(last_error, "Premature EOF in record %i.\n", i + 1);
					
					ihex_set_error(IHEX_ERR_PREMATURE_EOF, last_error);
					return IHEX_ERR_PREMATURE_EOF;
				}
				else
				{
					return 0;
				}
			case IHEX_ESA:
				offset = rs->ihrs_records[i].ihr_data[0] << 4;
				
				#ifdef IHEX_DEBUG
				printf("Switched offset to 0x%08x.\n", offset);
				#endif
				
				break;
			case IHEX_ELA:
				offset = (rs->ihrs_records[i].ihr_data[0] << 24)
					+ (rs->ihrs_records[i].ihr_data[1] << 16);
				
				#ifdef IHEX_DEBUG
				printf("Switched offset to 0x%08x.\n", offset);
				#endif
				
				break;
			case IHEX_SSA:
				break;
			default:
				last_error = malloc(512);
				sprintf(last_error, "Unknown record type in record %i: 0x%02x.\n", i+1, rs->ihrs_records[i].ihr_type);
				
				ihex_set_error(IHEX_ERR_UNKNOWN_RECORD_TYPE, last_error);
				
				return IHEX_ERR_UNKNOWN_RECORD_TYPE;
		}
	}
	
	return 0;
}

int ihex_mem_zero(void* dst, ulong_t n)
{
	ulong_t i = 0;
	
	for (i = 0; i < n; i ++)
	{
		((uint8_t*) dst)[i] = 0x00;
	}
	
	return 0;
}
