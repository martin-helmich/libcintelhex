#ifndef CINTELHEX_H
#define CINTELHEX_H

#include <stdlib.h>
#include <stdint.h>

#define IHEX_ERR_INCORRECT_CHECKSUM 0x01
#define IHEX_ERR_NO_EOF 0x02
#define IHEX_ERR_PARSE_ERROR 0x03
#define IHEX_ERR_WRONG_RECORD_LENGTH 0x04
#define IHEX_ERR_NO_INPUT 0x05

typedef unsigned int uint_t;
typedef unsigned long ulong_t;
typedef enum { IHEX_DATA = 0x00, IHEX_EOF = 0x01, IHEX_ESA = 0x02,
               IHEX_SSA = 0x03, IHEX_ELA = 0x04, IHEX_SLA = 0x05 } ihex_rtype_t;

typedef uint8_t* ihex_rdata_t;
typedef uint8_t  ihex_rlen_t;
typedef uint8_t  ihex_rchks_t;
typedef unsigned int ihex_error_t;
typedef uint16_t ihex_addr_t;

typedef struct ihex_record {
	unsigned int ihr_length;
	ihex_rtype_t ihr_type;
	ihex_addr_t  ihr_address;
	ihex_rdata_t ihr_data;
	ihex_rchks_t ihr_checksum;
} ihex_record_t;

typedef struct ihex_recordset {
	unsigned int ihrs_count;
	ihex_record_t *ihrs_records;
} ihex_recordset_t;

#ifdef IHEX_PARSE_C
static ihex_error_t ihex_last_errno = 0;
static char*        ihex_last_error = NULL;
#else
extern ihex_error_t ihex_last_errno;
extern char*        ihex_last_error;
#endif

/// Parse Intel HEX string from file.
ihex_recordset_t* ihex_rs_from_file(char* filename);

/// Parse Intel HEX string from string input.
ihex_recordset_t* ihex_rs_from_string(char* data);

/// Return error code, or 0 if no error occurred.
ihex_error_t ihex_errno();

/// Checks if a record's checksum is valid.
int ihex_check_record(ihex_record_t *r);

/// Return error string, or NULL if no error occurred.
char* ihex_error();

/// Parse 8-bit hex input
uint8_t ihex_fromhex8(uint8_t *input);

/// Parse 16-bit hex input
uint16_t ihex_fromhex16(uint8_t *input);
#endif
