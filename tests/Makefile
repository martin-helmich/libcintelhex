#
# (C) 2013  Martin Helmich <martin.helmich@hs-osnabrueck.de>
#           Oliver Erxleben <oliver.erxleben@hs-osnabrueck.de>
# 
#           University of Applied Sciences Osnabrück
# 
# This file is part of the CIntelHex library (libcintelhex).
# 
# libcintelhex is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# 
# libcintelhex is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with libcintelhex.  If not, see
# <http://www.gnu.org/licenses/>.
#

CFLAGS = -I ../include -I ..
OBJS = test_main.o

all: $(OBJS)
	$(CC) -o test $(OBJS) ../obj/*.o -lcunit

clean:
	rm -f $(OBJS) test
