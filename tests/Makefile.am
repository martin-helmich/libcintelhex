#
# (C) 2014  Andre Colomb <src@andre.colomb.de>
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

AM_CPPFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = test
TESTS = test

# These source files are included in test_main.c, so we have to
# distribute them although they should rather be in test_SOURCES
EXTRA_DIST = test_copy.c test_parse.c test_record.c

test_SOURCES = test_main.c
EXTRA_test_DEPENDENCIES = res/big-a.hex res/hex1.dat
test_LDADD = $(top_builddir)/src/libcintelhex.la -lcunit
