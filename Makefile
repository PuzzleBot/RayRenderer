TARGET = -mmacosx-version-min=10.8
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT
INCLUDES = -Iinclude
FILE = inputs/test1

all: raytracer.o


raytracer.o: include/raytrace.h src/main.c
	clang $(LIBS) $(TARGET) $(INCLUDES) src/*.c -o raytracer.o

run:
	./raytracer.o