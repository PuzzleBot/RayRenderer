#include "raytrace.h"

extern GlobalVars globals;

void drawPixels(){
    int i;
    int j;
    
    Point3D currentPlanePosition;
    double verticalMoveAmount = globals.planeHeight / START_HEIGHT;
    double horizontalMoveAmount = globals.planeWidth / START_WIDTH;
    ColourRGB pixelColour;
    
    Vector3D currentRay;
    currentRay.position = globals.viewPoint;
    
    /*For each pixel on the screen, calculate pixel colour (bottom-left to top-right)
          Pixel colour determined by ray reflecting off of the first intersected object to the light source
          (If reflectivity and transparency is on, repeat intersection tests for reflected rays)*/
    for(i = 0; i < START_HEIGHT; i++){
        for(j = 0; j < START_WIDTH; j++){
            currentPlanePosition.x = globals.viewPlane[1][0].x + (j * horizontalMoveAmount);
            currentPlanePosition.y = globals.viewPlane[1][0].y + (i * verticalMoveAmount);
            currentPlanePosition.z = globals.viewPlane[1][0].z;
            
            currentRay.direction.x = currentPlanePosition.x - globals.viewPoint.x;
            currentRay.direction.y = currentPlanePosition.y - globals.viewPoint.y;
            currentRay.direction.z = currentPlanePosition.z - globals.viewPoint.z;
            currentRay = normalize(currentRay);
            
            pixelColour = traceRay(currentRay, 0);
            insertPixel(j, i, pixelColour.red, pixelColour.green, pixelColour.blue);
        }
    }
    
    glDrawPixels(START_WIDTH, START_HEIGHT, GL_RGB, GL_FLOAT, globals.pixels);
}

void insertPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b){
    globals.pixels[(y * START_WIDTH * 3) + (x * 3)] = r;
    globals.pixels[(y * START_WIDTH * 3) + (x * 3) + 1] = g;
    globals.pixels[(y * START_WIDTH * 3) + (x * 3) + 2] = b;
}
