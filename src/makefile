CC=gcc
CFLAGS=-I$(IDIR) -m32 -s -Wall -g

IDIR=../include
ODIR=obj

_DEPS = try.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = try.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

try_mup: $(OBJ) try_mup.c
	$(CC) -o $@ $^ $(CFLAGS)

display_stack: $(OBJ) display_stack.c
	$(CC) -o $@ $^ $(CFLAGS)

# Clean command

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 