#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "prototype.h"

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#endif

#define START_WIDTH 1024
#define START_HEIGHT 768

#define PARSE_DELIMITERS ",; \t\n"

#define LOADBAR_WIDTH 256
#define LOADBAR_HEIGHT 50
#define LOADBAR_SEGMENT_PIXELS 2

#define AIR_REFRACTION_INDEX 1


#define COL_INDEX_RED 0
#define COL_INDEX_GREEN 1
#define COL_INDEX_BLUE 2
#define COL_INDEX_ALPHA 3

#define BLUR_ITERATIONS 8
#define BLUR_SAMPLE_RADIUS 3
#define BRIGHTSPOT_LOWER_BOUND 0.95
