#include "stdlib.h"

typedef enum { IHEX_DATA = 0x00, IHEX_END = 0x01, IHEX_ESA = 0x02,
               IHEX_SSA = 0x03, IHEX_ELA = 0x04, IHEX_SLA = 0x05 } ihex_rtype_t;

typedef char* ihex_rdata_t;
typedef unsigned int ihex_error_t;
typedef unsigned long ihex_addr_t;

typedef struct ihex_record {
	unsigned int ihr_length;
	ihex_rtype_t ihr_type;
	ihex_addr_t  ihr_address;
	ihex_rdata_t ihr_data;
} ihex_record_t;

typedef struct ihex_records {
	unsigned int ihrs_count;
	struct ihex_record *ihrs_records;
} ihex_records_t;

/// Parse Intel HEX string from file.
ihex_records_t* ihex_from_file(char* filename);

/// Parse Intel HEX string from string input.
ihex_records_t* ihex_from_string(char* data);

/// Return error code, or 0 if no error occurred.
ihex_error_t ihex_errno();

/// Return error string, or NULL if no error occurred.
char* ihex_error();

