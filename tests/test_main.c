#include "CUnit/Basic.h"
#include <CUnit/CUCurses.h>
#include "test_parse.c"

int init_parsingsuite(void)
{
	return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_parsingsuite(void)
{
	return 0;
}

int main()
{
	CU_pSuite pSuite = NULL;

 	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	pSuite = CU_add_suite("Parsing", init_parsingsuite, clean_parsingsuite);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_add_test(pSuite, "Record list can be read from string #1", test_can_read_ihex_from_string_1);
	CU_add_test(pSuite, "Correct checksum can be verified", test_checksum_is_verified_when_correct);
	CU_add_test(pSuite, "Incorrect checksum can not be verified", test_checksum_is_not_verified_when_incorrect);
	CU_add_test(pSuite, "No error is set when checksum is correct", test_no_error_on_correct_checksum);
	CU_add_test(pSuite, "Error number is set when checksum is incorrect", test_error_on_incorrect_checksum);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	//CU_basic_run_tests();
	CU_curses_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
