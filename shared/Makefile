# GNU C/C++ compiler and linker:
LINK = g++

# COMPILER OPTIONS:

CFLAGS = -c  -g -O0
CXXFLAGS = $(CFLAGS) -std=c++11 -Wall $(LIBS)

# OpenGL/Mesa libraries for Linux:
LIBS = -fopenmp -lm

#OBJECT FILES
OBJS = src/main.o src/hashtable.o

#HEADER FILES

#EXECUTABLES
EXECS = hash

all: hash

hash: ${OBJS}
	$(LINK) -o $@ $^ $(LIBS)
clean:
	rm -f *.o hash *~ src/*.o inc/*~ src/*~ $(EXECS) *~
edit:
	gedit inc/*.h src/*.cpp Makefile &
dox:
	doxygen doc/config_file
zip:
	tar -czf ../prog4.tgz ../parallel_final_fall14/
read:
	google-chrome doc/html/index.html &
