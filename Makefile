CARGS=-Wall -O2
SRC=main.c
ifdef COMPILE_TIME_MAP
	CARGS+=-DCOMPILE_TIME_MAP
else
	SRC+=qwerty.c
endif
OBJS=$(SRC:.c=.o)

.PHONY: all

all: a.out

a.out: $(OBJS)
	$(CC) $^ -o $@ $(CARGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CARGS)

