OBJ 	= graph.o graph_modify.o stack.o list.o tree.o spin.o

SOURCE = $(subst .o,.c,$(OBJ)) $(subst .o,.h,$(OBJ))

TARGET	= ising
LINK	= gcc
CC	= gcc
#~ CFLAGS	= -g -pg #-DTHREADED
#~ LFLAGS	= -lm -lgsl -lgslcblas #-lpthread -pg
CFLAGS	= -O2 -march=native -pipe # -DTHREADED
LFLAGS	= -lm -lgsl -lgslcblas # -lpthread
WARNLEVEL= -Wall # -pedantic

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c $(WARNLEVEL) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(LFLAGS) $(OBJ)

docs: doc.cfg $(SOURCE)
	doxygen $<

clean:
	rm -rf $(OBJ) $(TARGET) docs
