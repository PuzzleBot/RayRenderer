#include "raytrace.h"

extern GlobalVars globals;

void initLoadingBar(){
    int i, j;
    
    for(i = 0; i < LOADBAR_HEIGHT; i++){
        for(j = 0; j < LOADBAR_WIDTH; j++){
            /*Leave a black rectangle in the center of the loading bar window
              for the actual bar. 
             The bar is LOADBAR_WIDTH - 20 wide.
             The bar's border is 10 thick on all sides.*/
            if((i > 10) && (i < 40) && (j == 11)){
                j = LOADBAR_WIDTH - 11;
            }
            
            insertPixel(globals.loadBarPixels, LOADBAR_WIDTH, LOADBAR_HEIGHT, j, i, 0.7, 0.7, 0.7);
        }
    }
}


void incrementLoadingBar(){
    int i, j;
    static int colouredSegments = 0;
    static int totalSegments = ((LOADBAR_WIDTH - 20) / LOADBAR_SEGMENT_PIXELS) - 1;
    
    int leftX = (colouredSegments * LOADBAR_SEGMENT_PIXELS) + 11;
    int rightX = (colouredSegments * LOADBAR_SEGMENT_PIXELS) + 11 + LOADBAR_SEGMENT_PIXELS;
    
    /*Fill in the next divided segment of the loading bar*/
    if(colouredSegments < totalSegments){
        for(i = 11; i < 40; i++){
            for(j = leftX; j < rightX; j++){
                insertPixel(globals.loadBarPixels, LOADBAR_WIDTH, LOADBAR_HEIGHT, j, i, 0.2, 0.2, 0.7);
            }
        }
        
        colouredSegments++;
        glDrawPixels(LOADBAR_WIDTH, LOADBAR_HEIGHT, GL_RGB, GL_FLOAT, globals.loadBarPixels);
        glFlush();
    }
}
