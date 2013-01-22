
void test_recordset_size_is_correctly_computed();

int init_recordsuite(void)
{
	return 0;
}

int clean_recordsuite(void)
{
	return 0;
}

void add_tests_recordsuite(CU_pSuite suite)
{
	CU_add_test(suite, "Record set size is correctly computed", test_recordset_size_is_correctly_computed);
}

void test_recordset_size_is_correctly_computed()
{
	ihex_record_t r[2] = {{.ihr_length = 10, .ihr_type = IHEX_DATA,
	                       .ihr_address = 0x0100, .ihr_data = NULL,
	                       .ihr_checksum = 0x00},
	                      {.ihr_length = 20, .ihr_type = IHEX_DATA,
	                       .ihr_address = 0x0100, .ihr_data = NULL,
	                       .ihr_checksum = 0x00}};
	ihex_recordset_t rs = {.ihrs_count = 2, .ihrs_records = (ihex_record_t*) &(r)};
	
	CU_ASSERT_EQUAL(ihex_rs_get_size(&rs), 30);
}
