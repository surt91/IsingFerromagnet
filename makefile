OBJ 	= graph.o list.o spin.o

TARGET	= test
LINK	= gcc
CC	= gcc
#CFLAGS	= -g -pg -DTIME
CFLAGS	= -O2
LFLAGS	= -lm -lgsl -lgslcblas -pg
WARNLEVEL= -Wall

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c $(WARNLEVEL) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(LFLAGS) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET)
