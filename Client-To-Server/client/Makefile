CC 	= gcc
CFLAGS 	= -g

OBJECTS = client.o
EXEC 	= client

.PHONY: client

all: clean client

client: $(OBJECTS) 
	$(CC) $(CFLAGS) $(INCL) $(OBJECTS) -lpthread -o $(EXEC)

clean:
	rm -f $(EXEC) *.o

run: all
	./$(EXEC)

client.o: 
	$(CC) $(CFLAGS) -c client.c
