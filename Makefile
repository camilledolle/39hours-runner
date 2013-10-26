CC=gcc
SRCS=src/init.c src/objects.c
OBJS=${SRCS:.c=.o}
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic
LIBFLAGS= -lSDL -lSDL_ttf -lSDL_mixer
all: epirun

epirun: ${SRCS}
	${CC} ${CFLAGS} $(LIBFLAGS) $^ -o $@

check:
	./epirun

clean:
	rm -f $(OBJS) epirun

export: clean
	git archive HEAD --prefix=dolle_c-epirun/ | bzip2 > dolle_c-epirun.tar.bz2

.PHONY: check
