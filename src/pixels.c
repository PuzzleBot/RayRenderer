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
    sortLensList();
    for(i = 0; i < START_HEIGHT; i++){
        for(j = 0; j < START_WIDTH; j++){
            currentPlanePosition = getViewPlaneCoordinates(j, i);

            currentRay.direction.x = currentPlanePosition.x - globals.viewPoint.x;
            currentRay.direction.y = currentPlanePosition.y - globals.viewPoint.y;
            currentRay.direction.z = currentPlanePosition.z - globals.viewPoint.z;
            currentRay = normalize(currentRay);

            pixelColour = traceRay(currentRay, 0, AIR_REFRACTION_INDEX);
            insertPixel(globals.objectPixels, START_WIDTH, START_HEIGHT, j, i, pixelColour.red, pixelColour.green, pixelColour.blue);

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

    computeOverlayEffects();

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

GLfloat getPixel(GLfloat * pixelArray, int width, int height, int x, int y, int rgbOption){
    /*rgba = 0 -> red, 1-> green, 2-> blue*/
    return pixelArray[(y * width * 3) + (x * 3) + rgbOption];
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

void blendOverlayPixel(GLfloat * pixelArray, int width, int height, int x, int y, GLfloat r, GLfloat g, GLfloat b, GLfloat a){
    /*Treating a 1D array like a 2D one*/
    pixelArray[(y * width * 4) + (x * 4)] = r;
    pixelArray[(y * width * 4) + (x * 4) + 1] = g;
    pixelArray[(y * width * 4) + (x * 4) + 2] = b;
    pixelArray[(y * width * 4) + (x * 4) + 3] = a;
}

GLfloat getOverlayPixel(GLfloat * pixelArray, int width, int height, int x, int y, int rgbaOption){
    /*rgba = 0 -> red, 1-> green, 2-> blue, 3-> alpha*/
    return pixelArray[(y * width * 4) + (x * 4) + rgbaOption];
}





void getIntersectedScreenPixel(Vector3D v, int * pixelXstorage, int * pixelYstorage){
    double zScale;
    Point3D intersection;

    double verticalMoveAmount = globals.planeHeight / START_HEIGHT;
    double horizontalMoveAmount = globals.planeWidth / START_WIDTH;

    zScale = globals.viewPlane[0][0].z - v.position.z;

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

void computeOverlayEffects(){
    int i;
    int j;
    int k;

    int pixelX;
    int pixelY;

    /*Loading bar variables*/
    int loadbarSegments = (LOADBAR_WIDTH - 20) / LOADBAR_SEGMENT_PIXELS;
    int totalPixels = START_WIDTH * START_HEIGHT;
    int nextLoadSegment = 1;
    int nextIncrementPixel = totalPixels / loadbarSegments;

    Vector3D vpToLightRay;
    ShapeData * intersectedShape;
    Point3D intersection;

    vpToLightRay.position.x = globals.viewPoint.x;
    vpToLightRay.position.y = globals.viewPoint.y;
    vpToLightRay.position.z = globals.viewPoint.z;

    initLoadingBar();

    if(globals.lensFlares == true){
        for(i = 0; i < globals.numberOfLights; i++){
            vpToLightRay.direction.x = globals.lights[i].position.x - globals.viewPoint.x;
            vpToLightRay.direction.y = globals.lights[i].position.y - globals.viewPoint.y;
            vpToLightRay.direction.z = globals.lights[i].position.z - globals.viewPoint.z;
            vpToLightRay = normalize(vpToLightRay);

            /*Compute the starburst texture*/
            for(j = 0; j < START_HEIGHT; j++){
                for(k = 0; k < START_WIDTH; k++){
                    getIntersectedScreenPixel(vpToLightRay, &pixelX, &pixelY);
                    computeStarburstTexture(globals.lights[i], k, j, pixelX, pixelY);

                    /*Update the loading bar accordingly, based on the number of pixels
                     calculated so far out of total pixels*/
                    if((j * START_WIDTH) + k >= nextIncrementPixel){
                        incrementLoadingBar();
                        nextLoadSegment++;
                        nextIncrementPixel = nextLoadSegment * (totalPixels / loadbarSegments);
                    }
                }
            }

            intersectedShape = getFirstIntersectedShape(vpToLightRay);
            if(intersectedShape == NULL){
                /*No shapes between the viewpoint and shape - draw the texture on top
                  of the scene*/
                for(j = 0; j < START_HEIGHT; j++){
                    for(k = 0; k < START_WIDTH; k++){
                        combinePixelColours(globals.objectPixels, globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, k, j);
                    }
                }

            }
            else{
                /*Is the shape in front or behind the light?*/
                intersection = getIntersection(*intersectedShape, vpToLightRay);

                if(getLength(vpToLightRay.position, globals.lights[i].position) < getLength(vpToLightRay.position, intersection)){
                    /*Light is in front of the object - draw the texture on top
                     of the scene*/
                    for(j = 0; j < START_HEIGHT; j++){
                        for(k = 0; k < START_WIDTH; k++){
                            combinePixelColours(globals.objectPixels, globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, k, j);
                        }
                    }

                }
                else{
                    /*Light is behind the object...*/
                    for(j = 0; j < START_HEIGHT; j++){
                        for(k = 0; k < START_WIDTH; k++){
                            combinePixelColours(globals.objectPixels, globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, k, j);
                        }
                    }
                }
            }
        }

        /*Generate ghosts*/

        if(globals.ghosts == true){
            generateGhostTexture();

            for(i = 0; i < BLUR_ITERATIONS; i++){
                gaussianBlurGhost(&globals.ghostTexturePixels, BLUR_SAMPLE_RADIUS);
            }
            for(j = 0; j < START_HEIGHT; j++){
                for(k = 0; k < START_WIDTH; k++){
                    combinePixelColours(globals.pixels, globals.ghostTexturePixels, START_WIDTH, START_HEIGHT, k, j);
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
            if(intersectedShape != NULL){
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

            for(j = 0; j < START_HEIGHT; j++){
                for(k = 0; k < START_WIDTH; k++){
                    combinePixelColours(globals.objectPixels, globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, k, j);
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
            insertOverlayPixel(globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, pixelX+j, pixelY+i, light.colour.red, light.colour.green, light.colour.blue, 0.0);

            insertOverlayPixel(globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, pixelX-j, pixelY-i, light.colour.red, light.colour.green, light.colour.blue, 0.0);

            insertOverlayPixel(globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, pixelX-j, pixelY+i, light.colour.red, light.colour.green, light.colour.blue, 0.0);

            insertOverlayPixel(globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, pixelX+j, pixelY-i, light.colour.red, light.colour.green, light.colour.blue, 0.0);
        }
    }
}


void combinePixelColours(GLfloat * objectPixels, GLfloat * texturePixels, int width, int height, int x, int y){
    GLfloat finalRed;
    GLfloat finalGreen;
    GLfloat finalBlue;
    GLfloat textureTransparency = texturePixels[(y * width * 4) + (x * 4) + 3] * 1.0;


    finalRed = (objectPixels[(y * width * 3) + (x * 3)] * (1.0 - textureTransparency))
             + (texturePixels[(y * width * 4) + (x * 4)] * (textureTransparency));
    finalGreen = (objectPixels[(y * width * 3) + (x * 3) + 1] * (1.0 - textureTransparency))
               + (texturePixels[(y * width * 4) + (x * 4) + 1] * (textureTransparency));
    finalBlue = (objectPixels[(y * width * 3) + (x * 3) + 2] * (1.0 - textureTransparency))
              + (texturePixels[(y * width * 4) + (x * 4) + 2] * (textureTransparency));

    insertPixel(globals.pixels, width, height, x, y, finalRed, finalGreen, finalBlue);
}

void combineAllGlobalPixels(int x, int y){
    combinePixelColours(globals.objectPixels, globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, x, y);
    combinePixelColours(globals.objectPixels, globals.ghostTexturePixels, START_WIDTH, START_HEIGHT, x, y);
}
