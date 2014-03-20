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
