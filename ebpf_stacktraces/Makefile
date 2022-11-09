ALL :=  hello hello_dbg

CC=${CROSS_COMPILE}gcc
CFLAGS=-Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG

all: ${ALL}

hello: hello.c
	${CC} ${CFLAGS} hello.c -o hello
	${CROSS_COMPILE}strip --strip-all hello
hello_dbg: hello.c
	${CC} ${CFLAGS_DBG} hello.c -o hello_dbg

clean:
	rm -vf ${ALL} core* vgcore* *.o *~ *.c.gcov *.gcda *.gcno

