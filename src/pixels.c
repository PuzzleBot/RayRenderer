#include "raytrace.h"

extern GlobalVars globals;

void drawPixels(){
    int i;
    int j;
    
    for(i = 0; i < START_HEIGHT; i++){
        for(j = 0; j < START_WIDTH; j++){
            
        }
    }
}

void insertPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b){
    globals.pixels[(y * START_WIDTH * 3) + (x * 3)] = r;
    globals.pixels[(y * START_WIDTH * 3) + (x * 3) + 1] = g;
    globals.pixels[(y * START_WIDTH * 3) + (x * 3) + 2] = b;
}
