
#define UNIT_TEST 1

#include "CUnit/Basic.h"
#include <CUnit/CUCurses.h>
#include "test_parse.c"

int main(int argc, char* argv[])
{
	CU_pSuite pSuite = NULL;

 	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	pSuite = CU_add_suite("Parsing", init_parsingsuite, clean_parsingsuite);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	add_tests_parsingsuite(pSuite);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	
	if (argc > 1 && strcmp(argv[1], "-i") == 0)
	{
		CU_curses_run_tests();
	}
	else
	{
		CU_basic_run_tests();
	}
	
	CU_cleanup_registry();
	return CU_get_error();
}
