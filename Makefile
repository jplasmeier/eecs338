CC = gcc 
CFLAGS = -g -Wall  
LIBS = # None yet...
OUTPUT = main depositer.bin withdrawer.bin 
HEADERS = semex.h list.h

all: $(OUTPUT)

withdrawer.bin: withdrawer.c list.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

depositer.bin: depositer.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

main: semex.c list.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)

