CC = clang
CFLAGS = -g -Wall
LIBS = # None yet...
OUTPUT = main 
HEADERS = semex.h

all: $(OUTPUT)

main: semex.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)
