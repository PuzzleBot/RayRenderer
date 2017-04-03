#include "raytrace.h"

void gaussianBlur(GLfloat ** pixelSource, int pixelRadius){
    int i, j;
    int x, y;
    double ** normalDistribution = calloc((pixelRadius * 2) + 1, sizeof(double *));
    double standardDeviation = 1;
    int currentCount = 0;
    double currentRed;
    double currentGreen;
    double currentBlue;
    double currentAlpha;
    double edgeModifier;

    GLfloat * blurredPixels = calloc(START_WIDTH * START_HEIGHT * 4, sizeof(GLfloat));


    if((normalDistribution == NULL) || (blurredPixels == NULL)){
        printf("Not enough memory!\n");
        exit(1);
    }

    for(i = 0; i < ((pixelRadius * 2) + 1); i++){
        normalDistribution[i] = calloc((pixelRadius * 2) + 1, sizeof(double));
        if(normalDistribution[i] == NULL){
            printf("Not enough memory!\n");
            exit(1);
        }
        else{
            for(j = 0; j < ((pixelRadius * 2) + 1); j++){
                /*Calculate the 2D gaussian distribution for blurring
                  http://www.pixelstech.net/article/1353768112-Gaussian-Blur-Algorithm*/
                x = j - pixelRadius;
                y = i - pixelRadius;
                normalDistribution[i][j] = pow(M_E, -((pow(x, 2) + pow(y, 2)) / (2*pow(standardDeviation, 2)))) / (2 * M_PI * (pow(standardDeviation, 2)));
                //printf("%.2f ", normalDistribution[i][j]);
            }
        }
        //printf("\n");
    }

    /*Blur the source, writing the blurred result to the destination*/
    for(i = 0; i < START_HEIGHT; i++){
        for(j = 0; j < START_WIDTH; j++){

            currentCount = 0;
            currentRed = 0;
            currentGreen = 0;
            currentBlue = 0;
            currentAlpha = 0;
            for(y = (-pixelRadius); y <= (pixelRadius); y++){
                for(x = (-pixelRadius); x <= (pixelRadius); x++){
                    /*Pixel array bound check*/
                    if(((y + i) >= 0) && ((y + i) < START_HEIGHT) && ((x + j) >= 0) && ((x + j) < START_WIDTH)){
                        currentCount++;
                        currentRed = currentRed + (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, x + j, y + i, COL_INDEX_RED) * normalDistribution[y + pixelRadius][x + pixelRadius]);
                        currentGreen = currentGreen + (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, x + j, y + i, COL_INDEX_GREEN) * normalDistribution[y + pixelRadius][x + pixelRadius]);
                        currentBlue = currentBlue + (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, x + j, y + i, COL_INDEX_BLUE) * normalDistribution[y + pixelRadius][x + pixelRadius]);
                        //currentAlpha = currentAlpha + (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, x + j, y + i, COL_INDEX_ALPHA) * normalDistribution[y + pixelRadius][x + pixelRadius]);
                    }
                }
            }
            //printf("%d, %d\n", currentCount, pixelRadius);
            edgeModifier = ((double)currentCount / (double)(pixelRadius * pixelRadius * 4));
            currentRed = currentRed / edgeModifier;
            currentGreen = currentGreen / edgeModifier;
            currentBlue = currentBlue / edgeModifier;
            //currentAlpha = currentAlpha / edgeModifier;

            currentAlpha = currentAlpha + ((currentRed * 0.33333) + (currentGreen * 0.33333) + (currentBlue * 0.33333));
            if(currentAlpha > 0.99){
                currentAlpha = 1;
            }
            insertOverlayPixel(blurredPixels, START_WIDTH, START_HEIGHT, j, i, currentRed, currentGreen, currentBlue, currentAlpha);
        }
    }

    free(*pixelSource);
    *pixelSource = blurredPixels;
}

/*Specialized blur that only blurs the opacity of the image*/
void gaussianBlurGhost(GLfloat ** pixelSource, int pixelRadius){
    int i, j;
    int x, y;
    double ** normalDistribution = calloc((pixelRadius * 2) + 1, sizeof(double *));
    double standardDeviation = 1;
    int currentCount = 0;
    double currentRed;
    double currentGreen;
    double currentBlue;
    double currentAlpha;
    double edgeModifier;

    GLfloat * blurredPixels = calloc(START_WIDTH * START_HEIGHT * 4, sizeof(GLfloat));


    if((normalDistribution == NULL) || (blurredPixels == NULL)){
        printf("Not enough memory!\n");
        exit(1);
    }

    for(i = 0; i < ((pixelRadius * 2) + 1); i++){
        normalDistribution[i] = calloc((pixelRadius * 2) + 1, sizeof(double));
        if(normalDistribution[i] == NULL){
            printf("Not enough memory!\n");
            exit(1);
        }
        else{
            for(j = 0; j < ((pixelRadius * 2) + 1); j++){
                /*Calculate the 2D gaussian distribution for blurring
                  http://www.pixelstech.net/article/1353768112-Gaussian-Blur-Algorithm*/
                x = j - pixelRadius;
                y = i - pixelRadius;
                normalDistribution[i][j] = pow(M_E, -((pow(x, 2) + pow(y, 2)) / (2*pow(standardDeviation, 2)))) / (2 * M_PI * (pow(standardDeviation, 2)));
                //printf("%.2f ", normalDistribution[i][j]);
            }
        }
        //printf("\n");
    }

    /*Blur the source, writing the blurred result to the destination*/
    for(i = 0; i < START_HEIGHT; i++){
        for(j = 0; j < START_WIDTH; j++){

            currentCount = 0;
            currentRed = (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, j, i, COL_INDEX_RED));
            currentGreen = (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, j, i, COL_INDEX_GREEN));
            currentBlue = (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, j, i, COL_INDEX_BLUE));
            currentAlpha = 0;
            for(y = (-pixelRadius); y <= (pixelRadius); y++){
                for(x = (-pixelRadius); x <= (pixelRadius); x++){
                    /*Pixel array bound check*/
                    if(((y + i) >= 0) && ((y + i) < START_HEIGHT) && ((x + j) >= 0) && ((x + j) < START_WIDTH)){
                        currentCount++;
                        currentAlpha = currentAlpha + (getOverlayPixel(*pixelSource, START_WIDTH, START_HEIGHT, x + j, y + i, COL_INDEX_ALPHA) * normalDistribution[y + pixelRadius][x + pixelRadius]);
                    }
                }
            }
            //printf("%d, %d\n", currentCount, pixelRadius);
            edgeModifier = ((double)currentCount / (double)(pixelRadius * pixelRadius * 4));
            currentAlpha = (currentAlpha / edgeModifier) * 1.2;
            if(currentAlpha > 0.01){
                /*Any previously invisible pixels should be given the ghost's colour*/
                currentRed = BRIGHTSPOT_LOWER_BOUND;
                currentGreen = BRIGHTSPOT_LOWER_BOUND;
                currentBlue = BRIGHTSPOT_LOWER_BOUND;
                if(currentAlpha > 0.99){
                    currentAlpha = 1;
                }
            }

            insertOverlayPixel(blurredPixels, START_WIDTH, START_HEIGHT, j, i, currentRed, currentGreen, currentBlue, currentAlpha);
        }
    }

    free(*pixelSource);
    *pixelSource = blurredPixels;
}
