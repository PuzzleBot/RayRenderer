#include "raytrace.h"

void gaussianBlur(GLfloat * pixelSource, GLfloat * pixelDestination, int pixelRadius){
    int i, j;
    double ** normalDistribution = calloc(pixelRadius, sizeof(double *));

    if(normalDistribution == NULL){
        printf("Not enough memory!\n");
        exit(1);
    }

    for(i = 0; i < pixelRadius; i++){
        normalDistribution[i] = calloc(pixelRadius, sizeof(double));
        if(normalDistribution[i] == NULL){
            printf("Not enough memory!\n");
            exit(1);
        }
    }
}
