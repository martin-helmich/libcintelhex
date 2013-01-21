#include "cintelhex.h"

ulong_t ihex_rs_get_size(ihex_recordset_t* rs)
{
	ulong_t s = 0;
	uint_t  i = 0;
	
	for (i = 0; i < rs->ihrs_count; i ++)
	{
		s += rs->ihrs_records[i].ihr_length;
	}
	
	return s;
}
