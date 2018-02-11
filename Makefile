CC=clang
CFLAGS=-Wall -Wextra -Werror -Wno-unused-parameter -I../../src/ -O6 -funroll-loops -fomit-frame-pointer -rdynamic -shared -fPIC
LIBS=

zmq.pd_linux:
	$(CC) $(CFLAGS) hasher.c $(LIBS) -o hasher.pd_linux

clean:
	rm hasher.pd_linux
