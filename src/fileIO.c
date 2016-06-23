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
            else if(strcmp(shapeToken, "light") == 0){
                parseLight();
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
        parsedShape.colour.red = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Red colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.colour.green = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Green colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.colour.blue = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Blue colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.reflectivity = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Reflectivity.\n", globals.numberOfShapes);
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
        parsedShape.colour.red = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Red colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.colour.green = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Green colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.colour.blue = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Blue colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedShape.reflectivity = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Reflectivity.\n", globals.numberOfShapes);
        return;
    }
    
    globals.shapes = realloc(globals.shapes, sizeof(ShapeData) * (globals.numberOfShapes + 1));
    globals.shapes[globals.numberOfShapes] = parsedShape;
    printShape(globals.shapes[globals.numberOfShapes]);
    globals.numberOfShapes++;
}


void parseLight(){
    char * token;
    LightData parsedLight;
    
    /*Center coordinates*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedLight.position.x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: x position.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedLight.position.y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: y position.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedLight.position.z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: z position.\n", globals.numberOfLights);
        return;
    }
    
    /*Colour*/
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedLight.colour.red = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: Red colour amount.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedLight.colour.green = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: Green colour amount.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ", \t\n");
    if(token != NULL){
        parsedLight.colour.blue = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: Blue colour amount.\n", globals.numberOfLights);
        return;
    }
    
    globals.lights = realloc(globals.lights, sizeof(LightData) * (globals.numberOfLights + 1));
    globals.lights[globals.numberOfLights] = parsedLight;
    printLight(globals.lights[globals.numberOfLights]);
    globals.numberOfLights++;
}

