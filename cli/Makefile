#---------
#
# cli Makefile
#
#
#----------
IDIR = -I. \
       -I../applib

exampleParser: exampleParser.c
	gcc -o exampleParser exampleParser.c -ltlv -L../lib $(IDIR)

.PHONY: clean

clean:
	rm -f exampleParser
