AR=ar
ARFLAGS=rcs

CC=cc
CFLAGS=-x c -std=c99
CFLAGS+= -Wall -Werror
CFLAGS+= -fstack-protector-all
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare
CFLAGS+= -O3 -DNDEBUG

PREFIX?=${HOME}/local


all: libut.a libut.pc

libut.a: ut.o
	${AR} ${ARFLAGS} $@ $^

libut.pc: libut.pc.in
	sed s%@@PREFIX@@%${PREFIX}%g $< > $@

unit_test: example.o example_test.o
	${CC} $^ -I. -L. -lut -o example_test
	@./example_test 2>&1 1>/dev/null |grep ^UT

# may be called by root: no dependencies, so install target can never invoke cc or ar.
install:
	install -d -m 0755           ${PREFIX}/include
	install    -m 0644 ut.h      ${PREFIX}/include/ut.h
	install -d -m 0755           ${PREFIX}/lib
	install    -m 0644 libut.a   ${PREFIX}/lib/libut.a
	install -d -m 0755           ${PREFIX}/lib/pkgconfig
	install    -m 0644 libut.pc  ${PREFIX}/lib/pkgconfig/libut.pc

clean:
	rm -f libut.a ut.o libut.pc
	rm -f example.o example_test.o example_test
	rm -f *~ *.o *.core

.PHONY: clean install all unit_test
