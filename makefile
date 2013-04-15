OBJ 	= graph.o list.o stack.o spin.o

TARGET	= test
LINK	= gcc
CC	= gcc
CFLAGS	= -g -pg
#~ CFLAGS	= -O2 -march=native -pipe
LFLAGS	= -lm -lgsl -lgslcblas
WARNLEVEL= -Wall

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c $(WARNLEVEL) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(LFLAGS) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET)
