#include "raytrace.h"

extern GlobalVars globals;

void drawPixels(){
    
}

void insertPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b){
    globals.pixels[(y * START_WIDTH * 3) + (x * 3)] = r;
    globals.pixels[(y * START_WIDTH * 3) + (x * 3) + 1] = g;
    globals.pixels[(y * START_WIDTH * 3) + (x * 3) + 2] = b;
}
