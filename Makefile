.PHONY: all clean
.DEFAULT_GOAL := all

INCLUDES ?= -I/usr/include/freetype2 -I/usr/include/libpng16
CC ?= gcc
CFLAGS ?= -std=c99 -pedantic -Wall $(INCLUDES)
LDFLAGS ?= -lfreetype
SOURCE = ttf-chardump.c

# Windows-specific
EXEEXT ?=
ifeq ($(OS),Windows_NT)
	EXEEXT := .exe
endif

OUTFILE = ttf-chardump$(EXEEXT)

$(OUTFILE): $(SOURCE)
	$(CC) -o $@ $(SOURCE) $(CFLAGS) $(LDFLAGS)

all: $(OUTFILE)
clean:
	rm -f $(OUTFILE)