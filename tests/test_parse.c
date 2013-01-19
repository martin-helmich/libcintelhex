#include "cintelhex.h"

static void test_can_read_ihex_from_string(char* s);

void test_can_read_ihex_from_string_1(void);

void test_can_read_ihex_from_string_1(void)
{
	char* s = ":10010000214601360121470136007EFE09D2190140\r\n:00000001FF\r\n";
	test_can_read_ihex_from_string(s);
}

static void test_can_read_ihex_from_string(char* s)
{
	ihex_records_t *records = ihex_from_string(s);

	CU_ASSERT_FATAL(records != NULL);
	CU_ASSERT_FATAL(records->ihrs_count == 2);

	CU_ASSERT(records->ihrs_records[0].ihr_length  == 0x10);
	CU_ASSERT(records->ihrs_records[0].ihr_data[0] == 0x21);
	CU_ASSERT(records->ihrs_records[0].ihr_type    == IHEX_DATA);
	CU_ASSERT(records->ihrs_records[0].ihr_address == 0x0100);
}
