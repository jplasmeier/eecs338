CC = clang
CFLAGS = -g -Wall 
LIBS = # None yet...
OUTPUT = main ll.bin 
HEADERS = semex.h list.h

all: $(OUTPUT)

ll.bin: list.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

main: semex.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)

