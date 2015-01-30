CFLAGS += -Wall -Werror -Wextra -pedantic
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

CC = clang

HEADER_PATH = -I./greatest/ -I../

all: clean test.run

test: clean test.run
	./test.run -v | ./greatest/greenest

test.run: test_main.c test_basic.o ctmemory.o
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -pthread -g -o $@ test_main.c test_basic.o ctmemory.o
                                                       
clean:                                                 
	rm -f test *.o *.core *.out *.run                  
                                                       
test_basic.o:
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -g -o $@ -c test_basic.c
                                                       
ctmemory.o: ../CTMemory.h
	${CC} ${HEADER_PATH} ${CFLAGS} ${LDFLAGS} -pthread -g -o $@ -c ../CTMemory.c
