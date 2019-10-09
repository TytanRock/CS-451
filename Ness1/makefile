IDIR =include
CC=gcc
CFlags=-I$(IDIR)

ODIR=bin

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

assignment1: $(OBJ)
	$(CC) -o 5ps $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm 5ps -f
