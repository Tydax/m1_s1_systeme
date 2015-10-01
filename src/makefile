CC=gcc
CFLAGS=-I$(IDIR) -m32 -s -Wall -g

IDIR=../include
ODIR=obj

_DEPS = ctx.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ctx.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

odir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(DEPS) odir
	$(CC) -c -o $@ $< $(CFLAGS)

pingpong.out: $(OBJ) pingpong.c
	$(CC) -o $@ $^ $(CFLAGS)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 