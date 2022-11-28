
CXX=gcc
CXXFLAGS=-Wall -gcc
AS=nasm
ASFlags = -f elf64 -g
OBJECTS=fs_skeleton.o 


all: fs_skeleton.o 
		gcc -g -Wall -o make_dragon *.c
.PHONY: clean
clean:
	-rm -f *.o
# update this rule (change rev#) when it is time to
# make a new revision.
tar:
	rm -rf /tmp/xv6
	mkdir -p /tmp/xv6
	cp dist/* dist/.gdbinit.tmpl /tmp/xv6
	(cd /tmp; tar cf - xv6) | gzip >xv6-rev10-uva1.tar.gz  

SUBMIT_FILENAME=xv6-submission-$(shell date +%Y%m%d%H%M%S).tar.gz

submit:
	@tar -zcf $(SUBMIT_FILENAME) *.c *.h *.S *.ld Makefile $(wildcard *.txt) $(wildcard *.md) $(EXTRA) $(FILES)
	@echo "Created $(SUBMIT_FILENAME); please upload and submit this file."


.PHONY: dist-test dist

.DELETE_ON_ERROR: 1
