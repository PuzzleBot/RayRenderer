#include "raytrace.h"

extern GlobalVars globals;

void parseFile(char * inputFilePath){
    FILE * fp = fopen(inputFilePath, "r");
    
    if(fp == NULL){
        printf("Error could not open the file %s", inputFilePath);
        deallocExit(0);
    }
}
