IDIR=../include
ODIR=obj

CC=gcc
CFLAGS=-I$(IDIR) -m32 -Wall -g

_DEPS = ctx.h hw.h sem.h object.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ctx.o hw.o sem.o object.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

odir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS) odir
	$(CC) -c -o $@ $< $(CFLAGS)

prodcons.out: $(OBJ) prodcons.c
	$(CC) -o $@ $^ $(CFLAGS)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 