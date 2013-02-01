#define IHEX_PARSE_C

#include "cintelhex.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>

#define IHEX_CHR_RECORDMARK 0x3A

static int ihex_parse_single_record(ihex_rdata_t data, unsigned int length, ihex_record_t* record);

ihex_recordset_t* ihex_rs_from_file(char* filename)
{
	struct stat s;
	int         fd;
	ulong_t     l;
	char*       c;
	
	ihex_recordset_t* r;
	
	fd = open(filename, O_RDONLY);
	if (fd == 0)
	{
		ihex_last_error = "Input file does not exist.";
		ihex_last_errno = IHEX_ERR_NO_INPUT;
		
		return NULL;
	}
	
	if (fstat (fd, &s) != 0)
	{
		close(fd);
		
		ihex_last_error = "Could not stat input file.";
		ihex_last_errno = IHEX_ERR_NO_INPUT;
		
		return NULL;
	}
	
	l = s.st_size;
	c = (char*) mmap(NULL, l, PROT_READ, MAP_PRIVATE, fd, 0);
	
	r = ihex_rs_from_string(c);
	
	munmap((void*) c, l);
	close(fd);
	
	return r;
}

ihex_recordset_t* ihex_rs_from_string(char* data)
{
	uint_t i = 0;
	int    r = 0, c = 0;
	
	ihex_last_errno    = 0;
	ihex_last_error    = NULL;
	
	// Count number of record marks in input string.
	for (char *p = data; *p != 0x00; p ++)
	{
		if (*p == IHEX_CHR_RECORDMARK)
		{
			c ++;
		}
	}
	
	// Allocate memory for the record container structure and for each
	// individual record.
	ihex_record_t    *rec   = (ihex_record_t*)    calloc(c, sizeof(ihex_record_t));
	ihex_recordset_t *recls = (ihex_recordset_t*) malloc(sizeof(ihex_recordset_t));
	
	recls->ihrs_count   = c;
	recls->ihrs_records = rec;
	
	while (*data != 0x00)
	{
		i ++;
		
		ihex_rlen_t l = ihex_fromhex8(((ihex_rdata_t) data) + 1);
		if ((r = ihex_parse_single_record((ihex_rdata_t) data, l, &(rec[i-1]))) != 0)
		{
			char *e = malloc(512);
			sprintf(e, "Line %i: %s\n", i, ihex_last_error);
			
			ihex_last_errno = r;
			ihex_last_error = e;
			
			return NULL;
		}
		
		data += rec[i-1].ihr_length * 2 + 10;
		while (*data != IHEX_CHR_RECORDMARK && *data != 0x00)
		{
			data ++;
		}
	}
	
	if (recls->ihrs_records[recls->ihrs_count - 1].ihr_type != IHEX_EOF)
	{
		ihex_last_errno = IHEX_ERR_NO_EOF;
		ihex_last_error = "Missing EOF record.\n";
	}
	
	return recls;
}

static int ihex_parse_single_record(ihex_rdata_t data, unsigned int length, ihex_record_t* record)
{
	uint_t i;
	
	// Records needs to begin with record mark (usually ":")
	if (data[0] != IHEX_CHR_RECORDMARK)
	{
		ihex_last_error = "Missing record mark.";
		return IHEX_ERR_PARSE_ERROR;
	}
	
	// Record layout:
	//               1         2         3         4
	// 0 12 3456 78 90123456789012345678901234567890 12
	// : 10 0100 00 214601360121470136007EFE09D21901 40
	
	record->ihr_length   = (ihex_rlen_t)  ihex_fromhex8(data + 1);
	record->ihr_address  = (ihex_addr_t)  ihex_fromhex16(data + 3);
	record->ihr_type     = (ihex_rtype_t) ihex_fromhex8(data + 7);
	record->ihr_checksum = (ihex_rchks_t) ihex_fromhex8(data + 9 + record->ihr_length * 2);

	record->ihr_data     = (ihex_rdata_t) malloc(record->ihr_length);
	
	// Records needs to end with CRLF or LF.
	if ((data[11 + record->ihr_length*2] != 0x0D ||
	     data[12 + record->ihr_length*2] != 0x0A) &&
	    (data[11 + record->ihr_length*2] != 0x0A))
	{
		ihex_last_error = "Incorrect record length.";
		return IHEX_ERR_WRONG_RECORD_LENGTH;
	}
	
	for (i = 0; i < record->ihr_length; i ++)
	{
		if (data[9 + i*2] == 0x0A || data[9 + i*2] == 0x0D)
		{
			ihex_last_error = "Unexpected end of line.";
			return IHEX_ERR_WRONG_RECORD_LENGTH;
		}
		record->ihr_data[i] = ihex_fromhex8(data + 9 + i*2);
	}
	
	if (ihex_check_record(record) != 0)
	{
		ihex_last_error = "Checksum validation failed.";
		return IHEX_ERR_INCORRECT_CHECKSUM;
	}
	
	return 0;
}

int ihex_check_record(ihex_record_t *r)
{
	uint_t  i;
	uint8_t t = 0;
	
	t += r->ihr_length + ((r->ihr_address >> 8) & 0xFF)
		+ (r->ihr_address & 0xFF) + r->ihr_type + r->ihr_checksum;
	
	for (i = 0; i < r->ihr_length; i ++)
	{
		t += r->ihr_data[i];
	}
	
	return ((t & 0xFF) == 0) ? 0 : 1;
}

ihex_error_t ihex_errno()
{
	return ihex_last_errno;
}

char* ihex_error()
{
	return ihex_last_error;
}

void ihex_set_error(ihex_error_t errno, char* error)
{
	ihex_last_errno = errno;
	ihex_last_error = error;
				
	#ifdef IHEX_DEBUG
	printf(error);
	#endif
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
	       (ihex_fromhex4(i[2]) << 4) + ihex_fromhex4(i[3]);
}
