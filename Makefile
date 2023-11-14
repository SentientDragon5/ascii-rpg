CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -gdwarf-4 -g
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -gdwarf-4 -g
LDFLAGS=-lm

LIBS1=io-$(shell uname -m).a

EXECBIN=rpg
OBJS=\
    main.o \
    enemy.o \

.PHONY: all debug release clean

all: debug

debug: $(EXECBIN)

release: $(EXECBIN)

$(EXECBIN): $(OBJS) $(LIBS1)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN)  $(OBJS)

format:
	clang-format -i --style=file *.[ch]

