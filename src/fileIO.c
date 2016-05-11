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
    
    do{
        fgets(inputBuffer, 2048, fp);
        if(strlen(inputBuffer) > 1){
            shapeToken = strtok(inputBuffer, ", \t\n");
        
            if(strcmp(shapeToken, "triangle") == 0){
                parseTriangle();
            }
            else if(strcmp(shapeToken, "sphere") == 0){
                parseSphere();
            }
        }
    }
    while((feof(fp) == 0) && (ferror(fp) == 0));
    
}

void parseTriangle(){
    char * token;
    ShapeData parsedShape;
    
    parsedShape.type = TRIANGLE;
    
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
    printShape(globals.shapes[globals.numberOfShapes]);
    globals.numberOfShapes++;
}

void parseSphere(){
    char * token;
    ShapeData parsedShape;
    
    parsedShape.type = SPHERE;
    
    /*Center coordinates*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.position.x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): x position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.position.y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): y position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.position.z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): z position.\n", globals.numberOfShapes);
        return;
    }
    
    /*Sphere radius*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.radius = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): radius.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Colour*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.r = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Red colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.g = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Green colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.b = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Blue colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    globals.shapes = realloc(globals.shapes, sizeof(ShapeData) * (globals.numberOfShapes + 1));
    globals.shapes[globals.numberOfShapes] = parsedShape;
    printShape(globals.shapes[globals.numberOfShapes]);
    globals.numberOfShapes++;
}
