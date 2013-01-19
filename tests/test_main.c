#include "CUnit/Basic.h"
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

	CU_add_test(pSuite, "Test if record list can be read from string #1", test_can_read_ihex_from_string_1);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
