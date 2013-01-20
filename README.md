cIntelHex - A C library for parsing Intel HEX files
===================================================

Author
------

Martin Helmich <martin.helmich@hs-osnabrueck.de>
University of Applied Sciences Osnabrück

Synopsis
--------

libcintelhex is a C library for parsing Intel HEX files.

Installation
------------

Installation as usual:

	./configure
	make
	make install

Usage
-----

This library can parse Intel HEX data either from an input file or directly
from a given string:

    #include "cintelhex.h"
    
    int main()
    {
		ihex_records_t* r1 = ihex_from_file("my_filename");
		
		char* input = ":1a01000021A4B2Fe...";
		ihex_records_t* r2 = ihex_from_string(input);
		
		void* target = malloc(8192);
		ihex_copy(r1, target);
    }
