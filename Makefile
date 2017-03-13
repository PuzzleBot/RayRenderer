TARGET = -mmacosx-version-min=10.8
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT
INCLUDES = -Iinclude
FILE = inputs/defaultFile.csv

all: raytracer.o


raytracer.o: include/raytrace.h src/main.c
	clang $(LIBS) $(TARGET) $(INCLUDES) src/*.c -o bin/raytracer.o

run:
	./bin/raytracer.o $(FILE)

runFlare:
	./bin/raytracer.o inputs/triReflect.csv

runLens:
	./bin/raytracer.o inputs/lens.csv


testTriangle:
	clang $(LIBS) $(TARGET) $(INCLUDES) src/intersection.c src/vector.c tests/triangleDriver.c -o bin/test.o


testRun:
	./bin/test.o


testRefract:
	clang $(LIBS) $(TARGET) $(INCLUDES) src/shape.c src/vector.c tests/refractDriver.c -o bin/test.o


testBessel:
	clang $(LIBS) $(TARGET) $(INCLUDES) src/starburst.c tests/besselTest.c -o bin/test.o