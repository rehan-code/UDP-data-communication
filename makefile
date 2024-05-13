CC = gcc
CFLAGS = -Wall -pedantic

all: sender receiver

sender: sender.o header.h
	$(CC) $(CFLAGS) -o sender sender.o

sender.o: sender.c header.h
	$(CC) $(CFLAGS) -c sender.c -o sender.o

receiver: receiver.o header.h
	$(CC) $(CFLAGS) -o receiver receiver.o

receiver.o: receiver.c header.h
	$(CC) $(CFLAGS) -c receiver.c -o receiver.o

clean:
	rm *.o sender receiver