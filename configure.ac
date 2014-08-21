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

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([libcintelhex], [1.0], [martin.helmich@hs-osnabrueck.de])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([src/ihex_parse.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MKDIR_P
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h unistd.h sys/mman.h])
AC_CHECK_HEADERS([CUnit/Basic.h CUnit/CUCurses.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([munmap memset])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 tests/Makefile])
# Make the resource files for the test suite available in the build tree
AC_CONFIG_LINKS([tests/res/big-a.hex:tests/res/big-a.hex
                 tests/res/hex1.dat:tests/res/hex1.dat])
AC_OUTPUT
