CC=gcc
SRCS=src/init.c src/objects.c src/menu.c
OBJS=${SRCS:.c=.o}
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -g -g3
LIBFLAGS= -lSDL -lSDL_ttf -lSDL_mixer -lSDL_image
all: epirun

epirun: ${SRCS}
	${CC} ${CFLAGS} $(LIBFLAGS) $^ -o $@

check:
	./epirun

clean:
	rm -f $(OBJS) epirun epirun.core

export: clean
	git archive HEAD --prefix=dolle_c-runner/ | bzip2 > dolle_c-runner.tar.bz2

.PHONY: check
