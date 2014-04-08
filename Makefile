CC = g++

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iexample_00/glut-3.7.6-bin -w -fmax-errors=4
	LDFLAGS = -lglut -lGL -w
endif

RM = /bin/rm -f 

all: main 

main: main.o bezier.o utility.o
	$(CC) $(CFLAGS) -o main main.o bezier.o utility.o $(LDFLAGS) 

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

bezier.o: bezier.cpp
	$(CC) $(CFLAGS) -c bezier.cpp

utility.o: utility.cpp
	$(CC) $(CFLAGS) -c utility.cpp

clean: 
	$(RM) *.o main
 

