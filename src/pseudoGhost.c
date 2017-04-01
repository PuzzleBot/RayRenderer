#include "raytrace.h"

extern GlobalVars globals;

/*http://john-chapman-graphics.blogspot.ca/2013/02/pseudo-lens-flare.html
  Generates a pseudo-lens-flare texture using John Chapman's algorithm.*/
void generateGhostTexture(){
    IntegerList * brightPixels = NULL;
    int maxX, maxY, minX, minY;
    int i, j;

    Vector2D centerToLightCenter;
    /*Vector2D centerToLightSide1;
    Vector2D centerToLightSide2;
    Vector2D centerToLightPerpendicular;*/

    Point2D lightSide1;
    Point2D lightSide2;

    Point2D lightCenter;
    double starburstCoreRadius;

    double flareDisplacement;
    double sizeMultiplier;
    static double opacityModifier = 1.0;

    /*Find out where the light is*/
    brightPixels = sampleAllBrightSpots(globals.starburstTexturePixels, &minX, &minY, &maxX, &maxY);
    if(brightPixels == NULL){
        return;
    }

    if(maxX - minX > maxY - minY){
        starburstCoreRadius = maxX - minX;
    }

    else{
        starburstCoreRadius = maxY - minY;
    }

    lightCenter.x = (maxX + minX) / 2;
    lightCenter.y = (maxY + minY) / 2;

    /*Compute the vectors through the center of the screen to the light source for positioning flares*/
    centerToLightCenter.position.x = START_WIDTH / 2;
    centerToLightCenter.position.y = START_HEIGHT / 2;

    centerToLightCenter.direction.x = lightCenter.x - centerToLightCenter.position.x;
    centerToLightCenter.direction.y = lightCenter.y - centerToLightCenter.position.y;
    centerToLightCenter = normalize2D(centerToLightCenter);

    /*centerToLightPerpendicular = normalize2D(getPerpendicular(centerToLightCenter));
    centerToLightPerpendicular.position.x = lightCenter.x;
    centerToLightPerpendicular.position.y = lightCenter.y;

    lightSide1.x = centerToLightPerpendicular.position.x + (centerToLightPerpendicular.direction.x * starburstCoreRadius);
    lightSide1.y = centerToLightPerpendicular.position.y + (centerToLightPerpendicular.direction.y * starburstCoreRadius);

    lightSide2.x = centerToLightPerpendicular.position.x - (centerToLightPerpendicular.direction.x * starburstCoreRadius);
    lightSide2.y = centerToLightPerpendicular.position.y - (centerToLightPerpendicular.direction.y * starburstCoreRadius);


    centerToLightSide1.position.x = 0;
    centerToLightSide1.position.y = 0;
    centerToLightSide1.direction.x = lightSide1.x - centerToLightSide1.position.x;
    centerToLightSide1.direction.y = lightSide1.y - centerToLightSide1.position.y;
    centerToLightSide1 = normalize2D(centerToLightSide1);

    centerToLightSide2.position.x = 0;
    centerToLightSide2.position.y = 0;
    centerToLightSide2.direction.x = lightSide2.x - centerToLightSide2.position.x;
    centerToLightSide2.direction.y = lightSide2.y - centerToLightSide2.position.y;
    centerToLightSide2 = normalize2D(centerToLightSide2);*/

    /*Copy and paste bright spots*/
    flareDisplacement = getLength2D(centerToLightCenter.position, lightCenter) / 8;
    sizeMultiplier = 0.25;
    j = centerToLightCenter.position.x;
    for(i = centerToLightCenter.position.y + (centerToLightCenter.direction.y * flareDisplacement); ((i < START_HEIGHT) && (i >= 0)) && ((j < START_WIDTH) && (j >= 0)); i = i + (centerToLightCenter.direction.y * flareDisplacement)){
        j = j + (centerToLightCenter.direction.x * flareDisplacement);
        copyAndRescaleBrightSpots(globals.starburstTexturePixels, globals.ghostTexturePixels, brightPixels, j, i, sizeMultiplier, sizeMultiplier, opacityModifier);
        flareDisplacement = flareDisplacement * 2;
        sizeMultiplier = sizeMultiplier * 2;
    }

    flareDisplacement = getLength2D(centerToLightCenter.position, lightCenter) / 8;
    sizeMultiplier = 0.25;
    j = centerToLightCenter.position.x;
    for(i = centerToLightCenter.position.y - (centerToLightCenter.direction.y * flareDisplacement); ((i < START_HEIGHT) && (i >= 0)) && ((j < START_WIDTH) && (j >= 0)); i = i - (centerToLightCenter.direction.y * flareDisplacement)){
        j = j - (centerToLightCenter.direction.x * flareDisplacement);
        copyAndRescaleBrightSpots(globals.starburstTexturePixels, globals.ghostTexturePixels, brightPixels, j, i, sizeMultiplier, sizeMultiplier, opacityModifier);
        flareDisplacement = flareDisplacement * 2;
        sizeMultiplier = sizeMultiplier * 2;
    }

}


IntegerList * integerList_addToFront(IntegerList * list, int x, int y){
    IntegerList * newElement;
    newElement = calloc(1, sizeof(IntegerList));

    if(newElement == NULL){
        printf("Not enough memory!\n");
        exit(1);
    }

    newElement->x = x;
    newElement->y = y;
    newElement->next = list;
    return(newElement);
}


void integerList_destroyList(IntegerList * list){
    IntegerList * current = list;

    while(current != NULL){
        list = list->next;
        free(current);
        current = list;
    }
}


/*Gets a list of all pixel indices in a texture with an alpha value of 0.9 or higher*/
IntegerList * sampleAllBrightSpots(GLfloat * pixels, int * minX, int * minY, int * maxX, int * maxY){
    IntegerList * pixelIndexList = NULL;
    int i, j;

    *maxX = -1;
    *maxY = -1;
    *minX = START_WIDTH + 1;
    *minY = START_HEIGHT + 1;

    for(i = 0; i < START_HEIGHT; i++){
        for(j = 0; j < START_WIDTH; j++){
            if(getOverlayPixel(pixels, START_WIDTH, START_HEIGHT, j, i, COL_INDEX_ALPHA) >= BRIGHTSPOT_LOWER_BOUND){
                pixelIndexList = integerList_addToFront(pixelIndexList, j, i);

                /*Record minimum and maximum xy values for calculating the size of the bright spots*/
                if(j > *maxX){
                    *maxX = j;
                }

                if(j < *minX){
                    *minX = j;
                }

                if(i > *maxY){
                    *maxY = i;
                }

                if(i < *minY){
                    *minY = i;
                }
            }
        }
    }

    return(pixelIndexList);
}

void copyAndRescaleBrightSpots(GLfloat * starburstPixels, GLfloat * ghostPixels, IntegerList * brightPixelIndexList, double newXcenter, double newYcenter, double horizontalScale, double verticalScale, double opacity){
    int newPixelX, newPixelY;
    GLfloat currentPixel[4];
    IntegerList * currentPixelIndex = brightPixelIndexList;

    /*Track the max and min x and y values, for scaling purposes*/
    int minX = START_WIDTH, maxX = 0;
    int minY = START_HEIGHT, maxY = 0;
    int imageWidth = 0.0;
    int imageHeight = 0.0;
    double imageCenterX;
    double imageCenterY;

    /*Get the max and min values*/
    while(currentPixelIndex != NULL){
        if(currentPixelIndex->x < minX){ minX = currentPixelIndex->x; }
        if(currentPixelIndex->x > maxX){ maxX = currentPixelIndex->x; }
        if(currentPixelIndex->y < minY){ minY = currentPixelIndex->y; }
        if(currentPixelIndex->y > maxY){ maxY = currentPixelIndex->y; }
        currentPixelIndex = currentPixelIndex->next;
    }

    imageWidth = maxX - minX;
    imageHeight = maxY - minY;
    imageCenterX = (imageWidth / 2) + minX;
    imageCenterY = (imageHeight / 2) + minY;

    currentPixelIndex = brightPixelIndexList;
    /*Copy all the scaled bright spots over to the ghost array*/
    while(currentPixelIndex != NULL){
        /*Get the colour of the pixel to copy over*/
        currentPixel[0] = getOverlayPixel(starburstPixels, START_WIDTH, START_HEIGHT, currentPixelIndex->x, currentPixelIndex->y, COL_INDEX_RED);
        currentPixel[1] = getOverlayPixel(starburstPixels, START_WIDTH, START_HEIGHT, currentPixelIndex->x, currentPixelIndex->y, COL_INDEX_GREEN);
        currentPixel[2] = getOverlayPixel(starburstPixels, START_WIDTH, START_HEIGHT, currentPixelIndex->x, currentPixelIndex->y, COL_INDEX_BLUE);
        currentPixel[3] = opacity * getOverlayPixel(starburstPixels, START_WIDTH, START_HEIGHT, currentPixelIndex->x, currentPixelIndex->y, COL_INDEX_ALPHA);

        /*Translate the image to the origin, resize, then translate to where it's supposed to be*/
        newPixelX = ((currentPixelIndex->x - imageCenterX) * horizontalScale) + newXcenter;
        newPixelY = ((currentPixelIndex->y - imageCenterY) * verticalScale) + newYcenter;

        if((newPixelX >= 0) && (newPixelY >= 0) && (newPixelX < START_WIDTH) && (newPixelY < START_HEIGHT)){
            insertOverlayPixel(ghostPixels, START_WIDTH, START_HEIGHT, newPixelX, newPixelY, currentPixel[0], currentPixel[1], currentPixel[2], currentPixel[3]);
        }
        currentPixelIndex = currentPixelIndex->next;
    }
}
