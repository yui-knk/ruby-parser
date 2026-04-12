SHELL = /bin/sh

# V=0 quiet, V=1 verbose.  other values don't work.
V = 1
V0 = $(V:0=)
Q1 = $(V:1=)
Q = $(Q1:0=@)
ECHO1 = $(V:1=@ :)
ECHO = $(ECHO1:0=@ echo)

VPATH         = src
CC            = gcc
CFLAGS        = -O0 -g -Wall -DUNIVERSAL_PARSER=1
INCFLAGS      = -I./ -I../ext/ruby-parser/include -I../ext/ruby-parser/ruby
LDFLAGS       = -L/usr/local/lib -L./ -L../../lib
LIBS          = -lruby-parser.bundle
OBJS          = main.o
DEPENDS       =
DLDFLAGS      = -Wl,-rpath,'\$$ORIGIN/'
PROGRAM       = sample

.SUFFIXES: .c .m .cc .mm .cxx .cpp .o .S

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(DEPENDS)
	$(Q) $(CC) $(OBJS) $(LDFLAGS) $(LIBS) $(DLDFLAGS) -o $(PROGRAM)

.c.o:
	$(ECHO) compiling $(<)
	$(Q) $(CC) $(INCFLAGS) $(CPPFLAGS) $(CFLAGS) -o $@ -c $(CSRCFLAG)$<


.PHONY: lldb
lldb: $(PROGRAM)
	lldb $(PROGRAM)
