#include "cintelhex.h"
#include <stdio.h>

#define IHEX_CHR_RECORDMARK 0x3A

static ihex_error_t errno = 0;
static char* error = NULL;

static int ihex_parse_single_record(ihex_rdata_t data, unsigned int length, ihex_record_t* record);

ihex_recordset_t* ihex_rs_from_file(char* filename)
{
	return NULL;
}

ihex_recordset_t* ihex_rs_from_string(char* data)
{
	uint_t i = 0;
	int    r = 0, c = 0;
	
	errno    = 0;
	error    = NULL;
	
	for (char *p = data; *p != 0x00; p ++)
	{
		if (*p == IHEX_CHR_RECORDMARK)
		{
			c ++;
		}
	}
	
	ihex_record_t    *rec   = (ihex_record_t*)    calloc(c, sizeof(ihex_record_t));
	ihex_recordset_t *recls = (ihex_recordset_t*) malloc(sizeof(ihex_recordset_t));
	
	recls->ihrs_count   = c;
	recls->ihrs_records = rec;
	
	while (*data != 0x00)
	{
		i ++;
		
		ihex_rlen_t    l = ihex_fromhex8(((ihex_rdata_t) data) + 1);
		
		if ((r = ihex_parse_single_record((ihex_rdata_t) data, l, &(rec[i-1]))) != 0)
		{
			char *e = malloc(512);
			sprintf(e, "Line %i: %s\n", i, error);
			
			errno = r;
			error = e;
			
			return NULL;
		}
		
		data += rec[i-1].ihr_length * 2 + 10;
		while (*data != IHEX_CHR_RECORDMARK && *data != 0x00)
		{
			data ++;
		}
	}
	
	return recls;
}

static int ihex_parse_single_record(ihex_rdata_t data, unsigned int length, ihex_record_t* record)
{
	uint_t i;
	
	if (data[0] != IHEX_CHR_RECORDMARK)
	{
		error = "Missing record mark.";
		return IHEX_ERR_PARSE_ERROR;
	}
	
	//               1         2         3         4
	// 0 12 3456 78 90123456789012345678901234567890 12
	// : 10 0100 00 214601360121470136007EFE09D21901 40
	
	record->ihr_length   = (ihex_rlen_t)  ihex_fromhex8(data + 1);
	record->ihr_address  = (ihex_addr_t)  ihex_fromhex16(data + 3);
	record->ihr_type     = (ihex_rtype_t) ihex_fromhex8(data + 7);
	record->ihr_checksum = (ihex_rchks_t) ihex_fromhex8(data + 9 + record->ihr_length * 2);

	record->ihr_data    = (ihex_rdata_t)  malloc(record->ihr_length);
	
	for (i = 0; i < record->ihr_length; i ++)
	{
		if (data[9 + i*2] == 0x0A || data[9 + i*2] == 0x0D)
		{
			error = "Unexpected end of line.";
			return IHEX_ERR_PARSE_ERROR;
		}
		record->ihr_data[i] = ihex_fromhex8(data + 9 + i*2);
	}
	
	if (ihex_check_record(record) != 0)
	{
		error = "Checksum validation failed.";
		return IHEX_ERR_INCORRECT_CHECKSUM;
	}
	
	return 0;
}

int ihex_check_record(ihex_record_t *r)
{
	uint_t  i;
	uint8_t t = 0;
	
	t += r->ihr_length + ((r->ihr_address >> 8) & 0xFF) + (r->ihr_address & 0xFF) + r->ihr_type;
	
	for (i = 0; i < r->ihr_length; i ++)
	{
		t += r->ihr_data[i];
	}
	
	t += r->ihr_checksum;
	
	return ((t & 0xFF) == 0) ? 0 : 1;
}

ihex_error_t ihex_errno()
{
	return errno;
}

char* ihex_error()
{
	return error;
}

inline uint8_t ihex_fromhex4(uint8_t i)
{
	if      (i >= 0x61 && i <= 0x66) return i - 0x61 + 10;
	else if (i >= 0x41 && i <= 0x46) return i - 0x41 + 10;
	else if (i >= 0x30 && i <= 0x39) return i - 0x30;
	else return 0;
}

uint8_t ihex_fromhex8(uint8_t *i)
{
	return (ihex_fromhex4(i[0]) << 4) + ihex_fromhex4(i[1]);
}

uint16_t ihex_fromhex16(uint8_t *i)
{
	return (ihex_fromhex4(i[0]) << 12) + (ihex_fromhex4(i[1]) << 8) + 
	       (ihex_fromhex4(i[2]) << 4) + ihex_fromhex4(i[0]);
}
