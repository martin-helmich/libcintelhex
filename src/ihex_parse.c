#include "cintelhex.h"

#define IHEX_CHR_RECORDMARK 0x3A

ihex_records_t* ihex_from_file(char* filename)
{
	return NULL;
}

ihex_records_t* ihex_from_string(char* data)
{
	return NULL;
}

int ihex_parse_single_record(u_int8_t* data, unsigned int length, ihex_record_t* record)
{
	u_int i;
	
	if (data[0] != IHEX_CHR_RECORDMARK)
	{
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
		record->ihr_data[i] = ihex_fromhex8(data + 9 + i);
	}
	
	if (ihex_check_record(record) != 0)
	{
		return IHEX_ERR_INCORRECT_CHECKSUM;
	}
	
	return 0;
}

int ihex_check_record(ihex_record_t *r)
{
	u_int    i;
	u_int8_t t = 0;
	
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
	return -1;
}

char* ihex_error()
{
	return NULL;
}

inline u_int8_t ihex_fromhex4(u_int8_t i)
{
	if      (i >= 0x61 && i <= 0x66) return i - 0x61 + 10;
	else if (i >= 0x41 && i <= 0x46) return i - 0x41 + 10;
	else if (i >= 0x30 && i <= 0x39) return i - 0x30;
	else return 0;
}

u_int8_t ihex_fromhex8(u_int8_t *i)
{
	return (ihex_fromhex4(i[0]) << 4) + ihex_fromhex4(i[1]);
}

u_int16_t ihex_fromhex16(u_int8_t *i)
{
	return (ihex_fromhex4(i[0]) << 12) + (ihex_fromhex4(i[1]) << 8) + 
	       (ihex_fromhex4(i[2]) << 4) + ihex_fromhex4(i[0]);
}
