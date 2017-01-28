TARGET = -mmacosx-version-min=10.8
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT
INCLUDES = -Iinclude
FILE = inputs/defaultFile.csv

all: raytracer.o


raytracer.o: include/raytrace.h src/main.c
	clang $(LIBS) $(TARGET) $(INCLUDES) src/*.c -o bin/raytracer.o

run:
	./bin/raytracer.o $(FILE)

test:
	clang $(LIBS) $(TARGET) $(INCLUDES) src/intersection.c src/vector.c tests/triangleDriver.c -o bin/test.o

testRun:
	./bin/test.o