#set PDSRC to location of pd source
#set PD to pd install location
CC=cc
STRIP=strip

NAME=overo_gpioin

Phony: $(NAME).pd_linux

.SUFFIXES: .pd_linux

LINUXCFLAGS = -DPD -DUNIX -O2 -funroll-loops -fomit-frame-pointer \
    -W -Wshadow  -Werror -Wstrict-prototypes \
    -Wno-parentheses -Wno-switch \
    -fPIC
    
#-Wall  -Wno-unused 
    
LINUXINCLUDE =  -I$(PDSRC)/src

.c.pd_linux:
	$(CC) $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	$(CC) -lpthread -export_dynamic  -shared -o $*.pd_linux $*.o -lc -lm -L/usr/local/lib
	$(STRIP) --strip-unneeded $*.pd_linux
	rm -f $*.o ../$*.pd_linux
	ln -s $*/$*.pd_linux ..
	
install:
	cp help-*.pd $(PD)/doc/5.reference

clean:
	rm -f *.o *.pd_* so_locations


