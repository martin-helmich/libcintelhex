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
#include "test_copy.c"

int main(int argc, char* argv[])
{
 	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
	
	DECLARE_AND_INIT_SUITE(recordsuite, "Core record model");
	DECLARE_AND_INIT_SUITE(parsingsuite, "Input file parsing");
	DECLARE_AND_INIT_SUITE(memcopysuite, "Memory management");

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
