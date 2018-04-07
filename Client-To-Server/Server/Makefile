CC 	= gcc
CFLAGS 	= -g

OBJECTS = server.o
EXEC 	= Server

.PHONY: Server

all: clean Server

Server: $(OBJECTS) 
	$(CC) $(CFLAGS) $(INCL) $(OBJECTS) -lpthread -o $(EXEC)

clean:
	rm -f $(EXEC) *.o

run: all
	./$(EXEC)

server.o: 
	$(CC) $(CFLAGS) -c server.c
