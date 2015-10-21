IDIR=../include
ODIR=obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(IDIR) -ansi -pedantic -m32 -Wall -g -pthread

_DEPS = hardware.h driver.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = driver.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -L$(LIBDIR) -lhardware

odir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS) odir
	$(CC) -c -o $@ $< $(CFLAGS)

dmps.out: $(OBJ) dmps.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

frmt.out: $(OBJ) frmt.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 