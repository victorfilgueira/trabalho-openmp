CC=gcc
CFLAGS = -O3 -fopenmp -Wall
LDLIBS=-lm

LIBS=-lm
RM=rm -vf
MV=mv
BINDIR=./bin/
SRCDIR=./src/
PROG=omp_primos_bag_of_tasks
RUN_COUNT=5
ARGS=1000000 8

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
	@for i in $(shell seq 1 $(RUN_COUNT)); do \
		echo "Executando $$i vez(es)"; \
		$(BINDIR)$(PROG) $(ARGS); \
	done

list:
	ls $(BINDIR)*
clean:
	$(RM) $(PROGFILES) *~
## eof Makefile
