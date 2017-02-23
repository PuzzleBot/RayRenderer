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
    
    addOverlayEffects();
    
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


/*Inserts a pixel colour with alpha value into an array that will be used for
  glDrawPixels(). Unlike the above function, this one also takes in an alpha
  value (transparency), and expects that the array has enough space for an alpha value.*/
void insertOverlayPixel(GLfloat * pixelArray, int width, int height, int x, int y, GLfloat r, GLfloat g, GLfloat b, GLfloat a){
    /*Treating a 1D array like a 2D one*/
    pixelArray[(y * width * 4) + (x * 4)] = r;
    pixelArray[(y * width * 4) + (x * 4) + 1] = g;
    pixelArray[(y * width * 4) + (x * 4) + 2] = b;
    pixelArray[(y * width * 4) + (x * 4) + 3] = a;
}


void getIntersectedScreenPixel(Vector3D v, int * pixelXstorage, int * pixelYstorage){
    double zScale = globals.viewPlane[0][0].z - v.position.z;
    Point3D intersection;
    
    double verticalMoveAmount = globals.planeHeight / START_HEIGHT;
    double horizontalMoveAmount = globals.planeWidth / START_WIDTH;
    
    /*Scale the direction so that z equals 1*/
    v.direction.x = v.direction.x / v.direction.z;
    v.direction.y = v.direction.y / v.direction.z;
    v.direction.z = 1.0;
    
    /*Use this new direction to find the screen intersection by "closing the z gap"
      between the viewpoint and screen*/
    intersection.x = v.position.x + (v.direction.x * zScale);
    intersection.y = v.position.y + (v.direction.y * zScale);
    intersection.z = globals.viewPlane[0][0].z;
    
    *pixelXstorage = (int)((intersection.x + (globals.planeWidth / 2)) * (START_WIDTH / globals.planeWidth));
    *pixelYstorage = (int)((intersection.y + (globals.planeHeight / 2)) * (START_WIDTH / globals.planeWidth));
}

void addOverlayEffects(){
    int i;
    int j;
    int k;
    
    int pixelX;
    int pixelY;
    
    Vector3D vpToLightRay;
    ShapeData * intersectedShape;
    Point3D intersection;
    
    vpToLightRay.position.x = globals.viewPoint.x;
    vpToLightRay.position.y = globals.viewPoint.y;
    vpToLightRay.position.z = globals.viewPoint.z;
    
    if(globals.lensFlares == true){
        for(i = 0; i < globals.numberOfLights; i++){
            for(j = 0; j < START_HEIGHT; j++){
                for(k = 0; k < START_WIDTH; k++){
                    
                }
            }
        }
    }
    else{
        /*Put a few white pixels over the light*/
        for(i = 0; i < globals.numberOfLights; i++){
            vpToLightRay.direction.x = globals.lights[i].position.x - globals.viewPoint.x;
            vpToLightRay.direction.y = globals.lights[i].position.y - globals.viewPoint.y;
            vpToLightRay.direction.z = globals.lights[i].position.z - globals.viewPoint.z;
            vpToLightRay = normalize(vpToLightRay);
            
            intersectedShape = getFirstIntersectedShape(vpToLightRay);
            if(intersectedShape == NULL){
                /*The light is visible*/
                if(globals.lensFlares == false){
                    getIntersectedScreenPixel(vpToLightRay, &pixelX, &pixelY);
                    addLightBlot(globals.lights[i], pixelX, pixelY);
                }
            }
            else{
                /*Is the shape in front or behind the light?*/
                intersection = getIntersection(*intersectedShape, vpToLightRay);
                
                if(getLength(vpToLightRay.position, globals.lights[i].position) < getLength(vpToLightRay.position, intersection)){
                    /*Light is in front of the object*/
                    if(globals.lensFlares == false){
                        getIntersectedScreenPixel(vpToLightRay, &pixelX, &pixelY);
                        addLightBlot(globals.lights[i], pixelX, pixelY);
                    }
                }
            }
        }
    }
    
}

/*Puts white pixels in a 3-pixel radius of the specified pixel coordinates.*/
void addLightBlot(LightData light, int pixelX, int pixelY){
    int i, j;
    
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            insertOverlayPixel(globals.overlayPixels, START_WIDTH, START_HEIGHT, pixelX+j, pixelY+i, light.colour.red, light.colour.green, light.colour.blue, 0.0);
            
            insertOverlayPixel(globals.overlayPixels, START_WIDTH, START_HEIGHT, pixelX-j, pixelY-i, light.colour.red, light.colour.green, light.colour.blue, 0.0);
            
            insertOverlayPixel(globals.overlayPixels, START_WIDTH, START_HEIGHT, pixelX-j, pixelY+i, light.colour.red, light.colour.green, light.colour.blue, 0.0);
            
            insertOverlayPixel(globals.overlayPixels, START_WIDTH, START_HEIGHT, pixelX+j, pixelY-i, light.colour.red, light.colour.green, light.colour.blue, 0.0);
        }
    }
}

