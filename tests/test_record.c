/*
 * (C) 2013  Martin Helmich <martin.helmich@hs-osnabrueck.de>
 *           Oliver Erxleben <oliver.erxleben@hs-osnabrueck.de>
 * 
 *           University of Applied Sciences Osnabrück
 * 
 * This file is part of the CIntelHex library (libcintelhex).
 * 
 * libcintelhex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * libcintelhex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with libcintelhex.  If not, see
 * <http://www.gnu.org/licenses/>.
 */


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
