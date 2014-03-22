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


#include "cintelhex.h"

#include <stdio.h>


#define IHEX_SET_ERROR(errno, error, ...) \
	{ char *e = malloc(512); \
	  snprintf(e, 512, error, ##__VA_ARGS__); \
	  ihex_set_error(errno, e);}
#define IHEX_SET_ERROR_RETURN(errno, error, ...) \
	{ IHEX_SET_ERROR(errno, error, ##__VA_ARGS__); \
	  return errno;}


void ihex_set_error(ihex_error_t errno, char* error);

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

int ihex_rs_get_address_range(ihex_recordset_t* rs, uint32_t *min, uint32_t *max)
{
	uint_t   i;
	uint32_t offset = 0x00, address = 0x00, dummy_min, dummy_max;
	
	ihex_record_t *x;
	
	// Initialize range boundaries
	if (min == NULL) min = &dummy_min;
	if (max == NULL) max = &dummy_max;
	*min = UINT32_MAX;
	*max = 0x00;
	
	for (i = 0; i < rs->ihrs_count; i ++)
	{
		x       = (rs->ihrs_records + i);
		address = (offset + x->ihr_address);
		
		if (address < *min) *min = address;
		
		switch (x->ihr_type)
		{
			case IHEX_DATA:
				if (address + x->ihr_length > *max) *max = address + x->ihr_length;
				break;
			case IHEX_EOF:
				if (i < rs->ihrs_count - 1)
				{
					IHEX_SET_ERROR_RETURN(IHEX_ERR_PREMATURE_EOF,
						"Premature EOF in record %i", i + 1);
				}
				else
				{
					return 0;
				}
			case IHEX_ESA:
				offset = *(x->ihr_data) << 4;
				
				#ifdef IHEX_DEBUG
				printf("Switched offset to 0x%08x.\n", offset);
				#endif
				
				break;
			case IHEX_ELA:
				offset = (x->ihr_data[0] << 24) + (x->ihr_data[1] << 16);
				
				#ifdef IHEX_DEBUG
				printf("Switched offset to 0x%08x.\n", offset);
				#endif
				
				break;
			case IHEX_SSA:
				break;
			default:
				IHEX_SET_ERROR_RETURN(IHEX_ERR_UNKNOWN_RECORD_TYPE,
					"Unknown record type in record %i: 0x%02x",
					i+1, x->ihr_type);
		}
	}
	
	return 0;
}

void ihex_rs_free(ihex_recordset_t* rs)
{
	uint_t i = 0;
	
	if (rs == NULL) return;
	
	if (rs->ihrs_records != NULL)
	{
		for (i = 0; i < rs->ihrs_count; i++)
		{
			free(rs->ihrs_records[i].ihr_data);
		}
	}
	
	free(rs->ihrs_records);
	free(rs);
}
