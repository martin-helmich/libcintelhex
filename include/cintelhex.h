#include "stdlib.h"

typedef unsigned int ihex_rtype_t;
typedef void* ihex_rdata_t;
typedef unsigned int ihex_error_t;

typedef struct ihex_record {
	unsigned int ihr_length;
	ihex_rtype_t ihr_type;
	ihex_rdata_t ihr_data;
} ihex_record_t;

typedef struct ihex_records {
	unsigned int ihrs_count;
	struct ihex_records *ihrs_records;
} ihex_records_t;

/// Parse Intel HEX string from file.
ihex_records_t* ihex_from_file(char* filename);

/// Parse Intel HEX string from string input.
ihex_records_t* ihex_from_string(char* data);

/// Return error code, or 0 if no error occurred.
ihex_error_t ihex_errno();

/// Return error string, or NULL if no error occurred.
char* ihex_error();

