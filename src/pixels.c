#include "raytrace.h"

extern GlobalVars globals;

void drawPixels(){
    int i;
    int j;
    
    /*Loading bar variables*/
    int loadbarSegments = (LOADBAR_WIDTH - 20) / LOADBAR_SEGMENT_PIXELS;
    int totalPixels = START_WIDTH * START_HEIGHT;
    int nextLoadSegment = 1;
    int nextIncrementPixel = totalPixels / loadbarSegments;
    
    Point3D currentPlanePosition;
    double verticalMoveAmount = globals.planeHeight / START_HEIGHT;
    double horizontalMoveAmount = globals.planeWidth / START_WIDTH;
    ColourRGB pixelColour;
    
    Vector3D currentRay;
    currentRay.position = globals.viewPoint;
    
    /*Draw the loading bar border*/
    glWindowPos2i((START_WIDTH - LOADBAR_WIDTH)/2, (START_HEIGHT - LOADBAR_HEIGHT)/2);
    glDrawPixels(LOADBAR_WIDTH, LOADBAR_HEIGHT, GL_RGB, GL_FLOAT, globals.loadBarPixels);
    glFlush();
    
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
            
            pixelColour = traceRay(currentRay, 0, AIR_REFRACTION_INDEX);
            insertPixel(globals.pixels, START_WIDTH, START_HEIGHT, j, i, pixelColour.red, pixelColour.green, pixelColour.blue);
            
            /*Update the loading bar accordingly, based on the number of pixels
              calculated so far out of total pixels*/
            if((i * START_WIDTH) + j >= nextIncrementPixel){
                incrementLoadingBar();
                nextLoadSegment++;
                nextIncrementPixel = nextLoadSegment * (totalPixels / loadbarSegments);
            }
        }
    }
    //free(globals.loadBarPixels);
    
    glWindowPos2i(0, 0);
    glDrawPixels(START_WIDTH, START_HEIGHT, GL_RGB, GL_FLOAT, globals.pixels);
}

/*Inserts a pixel colour into an array that will be used for glDrawPixels().
  Parameters: the array to insert into, width of the array, height of the array, x position
              to insert to, y position to insert to, r g b colours*/
void insertPixel(GLfloat * pixelArray, int width, int height, int x, int y, GLfloat r, GLfloat g, GLfloat b){
    /*Treating a 1D array like a 2D one*/
    pixelArray[(y * width * 3) + (x * 3)] = r;
    pixelArray[(y * width * 3) + (x * 3) + 1] = g;
    pixelArray[(y * width * 3) + (x * 3) + 2] = b;
}
