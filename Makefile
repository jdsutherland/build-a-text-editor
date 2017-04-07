CFLAGS=-g -O3 -Wall -Wextra $(SET_DEBUG) $(OPTFLAGS)
SET_DEBUG = -DNDEBUG

src = $(wildcard *.c)
obj = $(src:.c=.o)

all: kilo

kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99

debug: all
debug: SET_DEBUG = -DDEBUG
debug: CFLAGS += -ggdb3 -O0

.PHONY: clean
clean:
	rm -f $(obj)
	rm -rf *.out.?*
