CC = gcc
CFLAGS = -Wall -g
LIBS = -lsqlite3

manifest_typing: manifest_typing.c
	$(CC) $(CFLAGS) -o manifest_typing manifest_typing.c $(LIBS)

clean:
	rm -f manifest_typing

.PHONY: clean
