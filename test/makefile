CFLAGS += -Wall -Werror -Wextra -pedantic
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

CC = clang

HEADER_PATH = -I./greatest/ -I../

LINKED_OBJECTS = test_basic.o ctmemory.o test_avltree.o ctrbtree.o

all: clean test.run

test: clean test.run
	./test.run -v | ./greatest/greenest

test.run: test_main.c ${LINKED_OBJECTS}
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -pthread -g -o $@ test_main.c ${LINKED_OBJECTS}

clean:
	rm -f test *.o *.core *.out *.run

test_basic.o:
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -g -o $@ -c test_basic.c

test_avltree.o:
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -g -o $@ -c test_avltree.c

ctmemory.o:
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -pthread -g -o $@ -c ../CTMemory.c

ctrbtree.o:
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -pthread -g -o $@ -c ../CTAVLTree.c
