SRC=main.c qwerty.c
OBJS=$(SRC:.c=.o)
WITH_COMPILE_TIME_MAP=-DCOMPILE_TIME_MAP

.PHONY: all

all: a.out

a.out: ${OBJS}
	${CC} $^ -o $@

%.o: %.c
	${CC} -c $< -o $@ ${WITH_COMPILE_TIME_MAP}

