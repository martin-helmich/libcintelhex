#include "cintelhex.h"

static void test_can_read_ihex_from_string(char* s);

void test_can_read_ihex_from_string_1(void);

void test_can_read_ihex_from_string_1(void)
{
	char* s = ":10010000214601360121470136007EFE09D2190140\r\n:00000001FF\r\n";
	test_can_read_ihex_from_string(s);
}

void test_no_error_on_correct_checksum(void)
{
	char* s = ":10010000214601360121470136007EFE09D2190140\r\n:00000001FF\r\n";
	ihex_records_t *records = ihex_from_string(s);

	CU_ASSERT(ihex_errno() == 0);
	CU_ASSERT(ihex_error() == NULL);
}

void test_error_on_incorrect_checksum(void)
{
	//                                                   v--- Wrong byte!
	char* s = ":10010000214601360121470136007EFE09D2190141\r\n:00000001FF\r\n";
	ihex_records_t *records = ihex_from_string(s);

	CU_ASSERT(ihex_errno() == IHEX_ERR_INCORRECT_CHECKSUM);
}

void test_checksum_is_verified_when_correct(void)
{
	u_int8_t data[0x10] = {0x21,0x46,0x01,0x36,0x01,0x21,0x47,0x01,0x36,0x00,0x7E,0xFE,0x09,0xD2,0x19,0x01};
	ihex_record_t r = {
		.ihr_length = 0x10, .ihr_type = IHEX_DATA, .ihr_address = 0x0100,
		.ihr_data = &data, .ihr_checksum = 0x40
	};
	
	CU_ASSERT(ihex_check_record(&r) == 0);
}

void test_checksum_is_not_verified_when_incorrect(void)
{
	u_int8_t data[0x10] = {0x21,0x46,0x01,0x36,0x01,0x21,0x47,0x01,0x36,0x00,0x7E,0xFE,0x09,0xD2,0x19,0x01};
	ihex_record_t r = {
		.ihr_length = 0x10, .ihr_type = IHEX_DATA, .ihr_address = 0x0100,
		.ihr_data = &data, .ihr_checksum = 0x20
	};
	
	CU_ASSERT(ihex_check_record(&r) == 1);
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
