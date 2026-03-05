CC = gcc
CFLAGS = -Wall -g
LIBS = -lsqlite3

storage_type_determination: storage_type_determination.c
	$(CC) $(CFLAGS) -o storage_type_determination storage_type_determination.c $(LIBS)

run: storage_type_determination
	./storage_type_determination

clean:
	rm -f storage_type_determination

.PHONY: clean run
