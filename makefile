OBJ 	= graph.o list.o spin.o

TARGET	= test
LINK	= gcc
CC	= gcc
CFLAGS	= -g
LFLAGS	= -lm
WARNLEVEL= -Wall

all: $(TARGET)

%.o: %.c %.h
	$(CC) -c $(WARNLEVEL) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LFLAGS)

clean:
	rm -rf $(OBJ) $(TARGET)
