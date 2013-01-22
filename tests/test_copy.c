
void test_can_zero_memory();
void test_memory_is_zeroed_before_copy();
void test_memory_is_copied_to_00_address();
void test_memory_is_copied_to_high_address();
void test_memory_is_copied_to_32bit_address();
void test_error_is_set_when_recordset_is_too_large();

void mock_recordset_free(ihex_recordset_t* rs)
{
	uint_t i = 0, j = 0;
	
	for (i = 0; i < rs->ihrs_count; i ++)
	{
		if (rs->ihrs_records[i].ihr_data != NULL)
		{
			free(rs->ihrs_records[i].ihr_data);
		}
	}
	
	free(rs->ihrs_records);
	free(rs);
}

ihex_recordset_t* mock_recordset(ihex_addr_t base, uint_t count, ihex_rlen_t length)
{
	ihex_recordset_t* rs = (ihex_recordset_t*) malloc(sizeof(ihex_recordset_t));
	uint_t            i  = 0, j = 0;
	
	rs->ihrs_records = (ihex_record_t*) calloc(count + 1, sizeof(ihex_record_t));
	rs->ihrs_count   = count + 1;
	
	for (i = 0; i < count; i ++)
	{
		rs->ihrs_records[i].ihr_type = IHEX_DATA;
		rs->ihrs_records[i].ihr_length = length;
		rs->ihrs_records[i].ihr_address = base + (i * length);
		rs->ihrs_records[i].ihr_checksum = 0x00;
		rs->ihrs_records[i].ihr_data = (ihex_rdata_t) malloc(length);
		
		for (j = 0; j < length; j ++)
		{
			rs->ihrs_records[i].ihr_data[j] = j % 16;
		}
	}
	
	rs->ihrs_records[count].ihr_type = IHEX_EOF;
	rs->ihrs_records[count].ihr_length = 0;
	rs->ihrs_records[count].ihr_address = 0x00;
	rs->ihrs_records[count].ihr_checksum = 0x00;
	rs->ihrs_records[count].ihr_data = NULL;
	
	return rs;
}

ihex_recordset_t* mock_recordset32(ihex_addr_t base, ihex_addr_t offset32, uint_t count, ihex_rlen_t length)
{
	ihex_recordset_t* rs = (ihex_recordset_t*) malloc(sizeof(ihex_recordset_t));
	ihex_record_t*    c  = NULL;
	uint_t            i  = 0, j = 0;
	
	rs->ihrs_records = (ihex_record_t*) calloc(count + 2, sizeof(ihex_record_t));
	rs->ihrs_count   = count + 2;
	
	c = &(rs->ihrs_records[0]);
	c->ihr_type     = IHEX_ELA;
	c->ihr_length   = 2;
	c->ihr_address  = 0x0000;
	c->ihr_checksum = 0x00;
	c->ihr_data     = (ihex_rdata_t) malloc(2);
	
	c->ihr_data[0]  = (offset32 >> 8) & 0xFF;
	c->ihr_data[1]  =  offset32       & 0xFF;
	
	for (i = 1; i < count + 1; i ++)
	{
		c = &(rs->ihrs_records[i]);
		c->ihr_type     = IHEX_DATA;
		c->ihr_length   = length;
		c->ihr_address  = base + ((i - 1) * length);
		c->ihr_checksum = 0x00;
		c->ihr_data     = (ihex_rdata_t) malloc(length);
		
		for (j = 0; j < length; j ++)
		{
			c->ihr_data[j] = j % 16;
		}
	}
	
	c = &(rs->ihrs_records[count + 1]);
	c->ihr_type = IHEX_EOF;
	c->ihr_length = 0;
	c->ihr_address = 0x00;
	c->ihr_checksum = 0x00;
	c->ihr_data = NULL;
	
	return rs;
}

int init_memcopysuite(void)
{
	return 0;
}

int clean_memcopysuite(void)
{
	return 0;
}

void add_tests_memcopysuite(CU_pSuite suite)
{
	CU_add_test(suite, "Memory area can be zeroed", test_can_zero_memory);
	CU_add_test(suite, "Memory area is zeroed before copy", test_memory_is_zeroed_before_copy);
	CU_add_test(suite, "Bytes are copied to 0x00 address", test_memory_is_copied_to_00_address);
	CU_add_test(suite, "Bytes are copied to high address", test_memory_is_copied_to_high_address);
	CU_add_test(suite, "Bytes are copied to 32bit address", test_memory_is_copied_to_32bit_address);
	CU_add_test(suite, "Error is set when recordset is too large", test_error_is_set_when_recordset_is_too_large);
}

void test_can_zero_memory()
{
	uint8_t area[16];
	uint_t  i;
	int     r;
	
	for (i = 0; i < 16; i ++)
	{
		area[i] = 0xAB;
	}
	
	r = ihex_mem_zero((void*) &(area), 16);
	
	CU_ASSERT_EQUAL_FATAL(r, 0);
	
	for (i = 0; i < 16; i ++)
	{
		CU_ASSERT_EQUAL(area[i], 0x00);
	}
}

void test_memory_is_zeroed_before_copy()
{
	uint8_t area[16];
	uint_t  i;
	int     r;
	
	ihex_recordset_t* rs = mock_recordset(0x00, 1, 8);
	
	for (i = 0; i < 16; i ++)
	{
		area[i] = 0xAB;
	}
	
	r = ihex_mem_copy(rs, (void*) &(area), 16);
	mock_recordset_free(rs);
	
	CU_ASSERT_EQUAL_FATAL(r, 0);
	
	for (i = 8; i < 16; i ++)
	{
		CU_ASSERT_EQUAL(area[i], 0x00);
	}
}

void test_memory_is_copied_to_00_address()
{
	uint8_t area[16];
	uint_t  i;
	int     r;
	
	ihex_recordset_t* rs = mock_recordset(0x00, 1, 8);
	
	for (i = 0; i < 16; i ++)
	{
		area[i] = 0xAB;
	}
	
	r = ihex_mem_copy(rs, (void*) &(area), 16);
	mock_recordset_free(rs);
	
	CU_ASSERT_EQUAL_FATAL(r, 0);
	
	for (i = 0; i < 8; i ++)
	{
		CU_ASSERT_EQUAL(area[i], i);
	}
}

void test_memory_is_copied_to_high_address()
{
	uint8_t area[16];
	uint_t  i;
	int     r;
	
	ihex_recordset_t* rs = mock_recordset(0x08, 1, 8);
	
	for (i = 0; i < 16; i ++)
	{
		area[i] = 0xAB;
	}
	
	r = ihex_mem_copy(rs, (void*) &(area), 16);
	mock_recordset_free(rs);
	
	CU_ASSERT_EQUAL_FATAL(r, 0);
	
	for (i = 8; i < 16; i ++)
	{
		CU_ASSERT_EQUAL(area[i], i - 8);
	}
}

void test_memory_is_copied_to_32bit_address()
{
	uint8_t* area = (uint8_t*) malloc(262144);
	uint_t  i;
	int     r;
	
	ihex_recordset_t* rs = mock_recordset32(0x0100, 0x0001, 1, 8);
	
	for (i = 0; i < 262144; i ++)
	{
		area[i] = 0xAB;
	}
	
	r = ihex_mem_copy(rs, (void*) area, 262144);
	mock_recordset_free(rs);
	
	CU_ASSERT_EQUAL_FATAL(r, 0);
	
	for (i = 0; i < 8; i ++)
	{
		CU_ASSERT_EQUAL(*(area + (0x0001 << 16) + 0x0100 + i), i);
	}
	
	free(area);
}

void test_error_is_set_when_recordset_is_too_large()
{
	uint8_t area[16];
	uint_t  i;
	int     r;
	
	ihex_recordset_t* rs = mock_recordset(0x20, 1, 8);
	
	for (i = 0; i < 16; i ++)
	{
		area[i] = 0xAB;
	}
	
	r = ihex_mem_copy(rs, (void*) &(area), 16);
	mock_recordset_free(rs);
	
	CU_ASSERT_EQUAL(r, IHEX_ERR_ADDRESS_OUT_OF_RANGE);
	CU_ASSERT_EQUAL(ihex_errno(), IHEX_ERR_ADDRESS_OUT_OF_RANGE);
}
