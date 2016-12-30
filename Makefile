CC=g++
SHELL=/bin/bash
INCLUDE_DIR="include/"
LFLAGS="-lGLU -lGL -lm -lpthread"
TARGET=bike

all: bike

bike:
	@$(CC) main.cpp -c -I $(INCLUDE_DIR)  && $(SHELL) libtool --tag=CXX --mode=link $(CC) -O3 -o $(TARGET) main.o libdrawstuff.la libode.la $(LFLAGS)

clean:
	rm -f bike *.o