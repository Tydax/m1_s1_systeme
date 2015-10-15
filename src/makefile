IDIR=../include
ODIR=obj
LIBDIR  = $(ROOTDIR)/lib

CC=gcc
CFLAGS=-I$(IDIR) -m32 -Wall -g

_DEPS = hardware.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = dmps.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS    = -L$(LIBDIR) -lhardware

odir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS) odir
	$(CC) -c -o $@ $< $(CFLAGS)

dmps.out: dmps.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 