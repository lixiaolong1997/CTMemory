CFLAGS += -Wall -Werror -Wextra -pedantic
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

CC = gcc

HEADER_PATH = -I./greatest/ -I../

test.run: test_main.c test_basic.o
	${CC} ${HEADER_PATH} -g -o $@ -pthread -c test_main.c test_basic.o ${CFLAGS} ${LDFLAGS}

clean:
	rm -f test *.o *.core *.out *.run

test_basic.o: ctmemory.o
	${CC} ${HEADER_PATH} -g -o $@ -pthread -c test_basic.c ctmemory.o

ctmemory.o:
	${CC} ${HEADER_PATH} -g -o $@ -pthread -c ../CTMemory.c
