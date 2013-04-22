OBJ 	= graph.o stack.o list.o spin.o

TARGET	= test
LINK	= gcc
CC	= gcc
#~ CFLAGS	= -g -pg #-DTHREADED
#~ LFLAGS	= -lm -lgsl -lgslcblas -lpthread -pg
CFLAGS	= -O2 -march=native -pipe # -DTHREADED
LFLAGS	= -lm -lgsl -lgslcblas # -lpthread
WARNLEVEL= -Wall # -pedantic

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c $(WARNLEVEL) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(LFLAGS) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET)
