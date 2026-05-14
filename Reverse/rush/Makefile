CC = gcc
CFLAGS = -O0 -g -pthread

all: chall

chall: hack_func.c
	$(CC) $(CFLAGS) main.o hack_func.c -o chall

run:
	./chall

clean:
	rm -f chall
