CC=gcc
CFLAGS = -O3 -fopenmp -Wall
LDLIBS=-lm

LIBS=-lm
RM=rm -vf
MV=mv
BINDIR=./bin/
SRCDIR=./src/
PROG=teste

vpath %.c ./src/

SRCFILES= $(wildcard src/omp_*.c)
OBJFILES= $(patsubst src/%.c, %.o, $(SRCFILES))
_PROGS= $(patsubst src/%.c, %, $(SRCFILES))
PROGFILES=$(addprefix $(BINDIR),$(_PROGS))

.PHONY: all clean run

all: $(PROGFILES)

$(BINDIR)%: $(SRCDIR)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)
run:  
	$(BINDIR)$(PROG)

list:
	ls $(BINDIR)*
clean:
	$(RM) $(PROGFILES) *~
## eof Makefile