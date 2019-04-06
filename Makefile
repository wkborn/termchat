IDIR =./include
CC= gcc
CFLAGS=-I$(IDIR) -Wall

ODIR=./obj
LDIR =./lib

LIBS=\
	-lenet\
	-lpthread\

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(patsubst ./src/%.c,%.o,$(wildcard ./src/*.c))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

termchat: $(OBJ)
	$(CC) -o ./build/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f ./build/termchat
