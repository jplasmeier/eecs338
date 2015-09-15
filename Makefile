CC = gcc 
CFLAGS = -g -Wall  
LIBS = # None yet...
OUTPUT = main depositer.bin withdrawer.bin 
HEADERS = semex.h 

all: $(OUTPUT)

withdrawer.bin: withdrawer.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

depositer.bin: depositer.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

main: semex.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)

