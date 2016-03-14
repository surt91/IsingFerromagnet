TARGET	= ising
MANUAL = manual.pdf

OBJ 	= graph.o graph_modify.o stack.o list.o tree.o spin.o queue.o correlation_by_k_susceptibility.o union_find.o
SOURCE = $(subst .o,.c,$(OBJ)) $(subst .o,.h,$(OBJ))

LINK	= gcc
CC	= gcc
DOXYGEN = doxygen

DOXYCHECK := $(shell which $(DOXYGEN))

#~ CFLAGS	= -g -pg #-DTHREADED
#~ LFLAGS	= -lm -lgsl -lgslcblas #-lpthread -pg
CFLAGS	= -O3 -mtune=native -pipe # -DTHREADED
LFLAGS	= -lm -lgsl -lgslcblas # -lpthread
WARNLEVEL= -Wall # -pedantic

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c $(WARNLEVEL) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(LFLAGS) $(OBJ)

$(MANUAL): doc.cfg $(SOURCE)
ifndef DOXYCHECK
	$(warning No doxygen found! Skip manual generation.)
else
	$(DOXYGEN) $<
	$(MAKE) -C docs/latex
	cp docs/latex/refman.pdf $(MANUAL)
endif

clean:
	rm -rf $(OBJ) $(TARGET) $(MANUAL) docs
