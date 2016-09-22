IDIR = ../include
ODIR = obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(IDIR) -ansi -pedantic -m32 -Wall -g -pthread

_DEPS = hardware.h driver.h mbr.h block.h inode.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = driver.o mbr.o block.o inode.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -L$(LIBDIR) -lhardware

OUT = dfs.out dmps.out dvol.out fillinode.out fillvolume.out frmt.out mknfs.out mkvol.out

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