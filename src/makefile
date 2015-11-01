IDIR = ../include
ODIR = obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(IDIR) -ansi -pedantic -m32 -Wall -g -pthread

_DEPS = hardware.h driver.h mbr.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = driver.o mbr.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -L$(LIBDIR) -lhardware

OUT = mkvol.out dvol.out

all: $(OUT)

%.out: $(OBJ) %.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

odir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS) odir
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 