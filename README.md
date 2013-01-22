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

After installation, include `cintelhex.h` and add `-lcintelhex` to your linker flags.

### Reading Intel HEX data

This library can parse Intel HEX data either from an input file or directly
from a given string:

    ihex_records_t* r1 = ihex_from_file("my_filename");
    
    char* input = ":1a01000021A4B2Fe...";
    ihex_records_t* r2 = ihex_from_string(input);

### Applying Intel HEX data

Intel HEX data can be copied to any region in memory (either somewhere in RAM,
but also to mapped memory regions of hardware devices):

    void* dst = malloc(8192); // Allocate target region.
    int   r   = ihex_mem_copy(rs, dst, 8192); // Copy data to target region.

    if (r == 0) printf("All is well.\n");
