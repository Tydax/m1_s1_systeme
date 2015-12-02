IDIR = ../include
ODIR = obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(IDIR) -ansi -pedantic -m32 -Wall -g -pthread

_DEPS = hardware.h mi_user.h mi_kernel.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mi_user.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -L$(LIBDIR) -lhardware

OUT = mi_kernel.out

odir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS) odir
	$(CC) -c -o $@ $< $(CFLAGS)

%.out: $(OBJ) %.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $(OUT)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 