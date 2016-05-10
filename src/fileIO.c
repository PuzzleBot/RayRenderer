#include "raytrace.h"

extern GlobalVars globals;

void parseFile(char * inputFilePath){
    FILE * fp = fopen(inputFilePath, "r");
    char inputBuffer[2048];
    char * shapeToken = NULL;
    
    if(fp == NULL){
        printf("Error could not open the file %s", inputFilePath);
        deallocExit(0);
    }
    
    fgets(inputBuffer, 2048, fp);
    while((feof(fp) == 0) && (ferror(fp) == 0)){
        shapeToken = strtok(inputBuffer, ", \t\n");
        
        if(strcmp(shapeToken, "triangle") == 0){
            parseTriangle();
        }
        else if(strcmp(shapeToken, "sphere") == 0){
            parseSphere();
        }
        
        fgets(inputBuffer, 2048, fp);
    }
    
}

void parseTriangle(){
    char * token;
    ShapeData parsedShape;
    
    /*Coordinates of vertex 1*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[0].x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 1 'x' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[0].y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 1 'y' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[0].z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 1 'z' position.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Coordinates of vertex 2*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[1].x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 2 'x' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[1].y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 2 'y' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[1].z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 2 'z' position.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Coordinates of vertex 3*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[2].x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 3 'x' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[2].y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 3 'y' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[2].z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 3 'z' position.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Colour*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.r = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Red colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.g = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Green colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.b = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Blue colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    globals.shapes = realloc(globals.shapes, sizeof(ShapeData) * (globals.numberOfShapes + 1));
    globals.shapes[globals.numberOfShapes] = parsedShape;
    globals.numberOfShapes++;
}

void parseSphere(){
    
}
