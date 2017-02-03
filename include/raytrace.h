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

#define LOADBAR_WIDTH 256
#define LOADBAR_HEIGHT 50
#define LOADBAR_SEGMENT_PIXELS 2

#define AIR_REFRACTION_INDEX 1

