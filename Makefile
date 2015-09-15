CC = clang
CFLAGS = -g -Wall 
LIBS = # None yet...
OUTPUT = main ll.bin 
HEADERS = semex.h list.h

all: $(OUTPUT)

withdrawer.bin: withdrawer.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

depositer.bin: depositer.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

ll.bin: list.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

main: semex.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)

