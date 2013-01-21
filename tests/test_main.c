
#define UNIT_TEST 1

#define DECLARE_AND_INIT_SUITE(name, desc) \
	CU_pSuite name = CU_add_suite(desc, init_##name, clean_##name); \
	if (name == NULL) \
	{ \
		CU_cleanup_registry(); \
		return CU_get_error(); \
	} \
	add_tests_##name(name);
	
#ifndef CU_FAIL_STRING
#define CU_FAIL_STRING(str) \
	{ CU_assertImplementation(CU_FALSE, LINE, (str), FILE, "", CU_FALSE); }
#endif

#include "CUnit/Basic.h"
#include <CUnit/CUCurses.h>
#include "test_parse.c"
#include "test_record.c"

int main(int argc, char* argv[])
{
 	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
	
	DECLARE_AND_INIT_SUITE(recordsuite, "Core record model");
	DECLARE_AND_INIT_SUITE(parsingsuite, "Input file parsing");

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
