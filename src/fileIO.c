#include "raytrace.h"

extern GlobalVars globals;

void parseFile(char * inputFilePath){
    FILE * fp = fopen(inputFilePath, "r");
    char inputBuffer[2048];
    char * shapeToken = NULL;
    int lineCounter = 0;
    
    GLfloat currentReflection = 0.0;
    GLfloat currentOpacity = 1.0;
    GLfloat currentRefractIndex = 1.0;
    
    if(fp == NULL){
        printf("Error could not open the file %s", inputFilePath);
        deallocExit(0);
    }
    
    do{
        fgets(inputBuffer, 2048, fp);
        if(strlen(inputBuffer) > 1){
            shapeToken = strtok(inputBuffer, ",; \t\n");
        
            if(strcmp(shapeToken, "triangle") == 0){
                parseTriangle(currentRefractIndex);
            }
            else if(strcmp(shapeToken, "sphere") == 0){
                parseSphere(currentRefractIndex);
            }
            else if(strcmp(shapeToken, "light") == 0){
                parseLight();
            }
            else if(strcmp(shapeToken, "lens") == 0){
                parseLens();
            }
            else{
                /*Lines starting with '#' are comments, and are ignored*/
                if(shapeToken[0] != '#'){
                    printf("Warning: Ignoring unrecognized input on line %d.\n", lineCounter);
                }
            }
        }
        lineCounter++;
    }
    while((feof(fp) == 0) && (ferror(fp) == 0));
    
}

void parseTriangle(GLfloat refractIndex){
    char * token;
    ShapeData parsedShape;
    
    parsedShape.type = TRIANGLE;
    
    /*Coordinates of vertex 1*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[0].x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 1 'x' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[0].y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 1 'y' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[0].z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 1 'z' position.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Coordinates of vertex 2*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[1].x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 2 'x' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[1].y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 2 'y' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[1].z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 2 'z' position.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Coordinates of vertex 3*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[2].x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 3 'x' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[2].y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 3 'y' position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.triangle.points[2].z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Vertex 3 'z' position.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Colour*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.colour.red = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Red colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.colour.green = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Green colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.colour.blue = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Blue colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.reflectivity = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Reflectivity.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.opacity = atof(token);
        
        token = strtok(NULL, ",; \t\n");
        if(token != NULL){
            parsedShape.refractionIndex = atof(token);
        }
        else{
            printf("Warning: Missing parameter for shape %d (triangle): Index of Refraction.\n", globals.numberOfShapes);
            printf("Setting to default: Air refraction index\n");
            parsedShape.refractionIndex = AIR_REFRACTION_INDEX;
        }
    }
    else{
        printf("Parse error: Missing parameter for shape %d (triangle): Opacity.\n", globals.numberOfShapes);
        printf("Setting opacity to default: 1.0\n");
        parsedShape.opacity = 1.0;
        parsedShape.refractionIndex = AIR_REFRACTION_INDEX;
    }
    
    parsedShape.theShape.triangle.normal.direction = nullPoint();
    parsedShape.theShape.triangle.normal = getNormal(parsedShape, parsedShape.theShape.triangle.points[0]);
    
    globals.shapes = realloc(globals.shapes, sizeof(ShapeData) * (globals.numberOfShapes + 1));
    globals.shapes[globals.numberOfShapes] = parsedShape;
    printShape(globals.shapes[globals.numberOfShapes]);
    globals.numberOfShapes++;
}

void parseSphere(GLfloat refractIndex){
    char * token;
    ShapeData parsedShape;
    
    parsedShape.type = SPHERE;
    
    /*Center coordinates*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.position.x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): x position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.position.y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): y position.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.position.z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): z position.\n", globals.numberOfShapes);
        return;
    }
    
    /*Sphere radius*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.theShape.sphere.radius = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): radius.\n", globals.numberOfShapes);
        return;
    }
    
    
    /*Colour*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.colour.red = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Red colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.colour.green = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Green colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.colour.blue = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Blue colour amount.\n", globals.numberOfShapes);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.reflectivity = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Reflectivity.\n", globals.numberOfShapes);
        return;
    }
    
    /*Opacity and index of refraction*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedShape.opacity = atof(token);
        
        token = strtok(NULL, ",; \t\n");
        if(token != NULL){
            parsedShape.refractionIndex = atof(token);
        }
        else{
            printf("Warning: Missing parameter for shape %d (sphere): Index of Refraction.\n", globals.numberOfShapes);
            printf("Setting to default: Air refraction index\n");
            parsedShape.refractionIndex = AIR_REFRACTION_INDEX;
        }
    }
    else{
        printf("Parse error: Missing parameter for shape %d (sphere): Opacity.\n", globals.numberOfShapes);
        printf("Setting opacity to default: 1.0\n");
        parsedShape.opacity = 1.0;
        parsedShape.refractionIndex = AIR_REFRACTION_INDEX;
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
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLight.position.x = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: x position.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLight.position.y = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: y position.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLight.position.z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: z position.\n", globals.numberOfLights);
        return;
    }
    
    /*Colour*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLight.colour.red = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: Red colour amount.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLight.colour.green = (GLfloat)atof(token);
    }
    else{
        printf("Parse error: Missing parameter for light %d: Green colour amount.\n", globals.numberOfLights);
        return;
    }
    
    token = strtok(NULL, ",; \t\n");
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


void parseLens(){
    char * token;
    LensData parsedLens;
    
    /*Z-Offset from the view plane (automatically centered at x = 0 and y = 0)*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLens.position.z = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for lens %d: x position.\n", globals.numberOfLenses);
        return;
    }
    
    parsedLens.position.x = globals.viewPlane[0][0].x + (globals.planeWidth / 2);
    parsedLens.position.y = globals.viewPlane[0][0].y - (globals.planeHeight / 2);
    
    
    /*Radius - to define the curvature (think: sphere)*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLens.radius = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for lens %d: Radius.\n", globals.numberOfLenses);
        return;
    }
    
    /*Concave part or convex part?*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        if(strcmp(token, "convex") == 0){
            parsedLens.isConvex = true;
            
            /*Position the lens properly, rather than at the center of a would-be sphere
              (i.e. the middle of the lens actually being at the specified coords)*/
            parsedLens.position.z = parsedLens.position.z + parsedLens.radius;
        }
        else if(strcmp(token, "concave") == 0){
            parsedLens.isConvex = false;
            
            /*Position the lens properly, rather than at the center of a would-be sphere*/
            parsedLens.position.z = parsedLens.position.z - parsedLens.radius;
        }
        else{
            printf("Parse error: Invalid parameter for lens %d: Concavity.\n", globals.numberOfLenses);
            printf("\tAccepted strings (no quotes): \"concave\", \"convex\"\n");
            printf("\tDefaulting to convex.\n");
            parsedLens.isConvex = true;
        }
    }
    else{
        printf("Parse error: Missing parameter for lens %d: Concavity.\n", globals.numberOfLenses);
        return;
    }
    
    /*Refractive index*/
    token = strtok(NULL, ",; \t\n");
    if(token != NULL){
        parsedLens.refractionIndex = atof(token);
    }
    else{
        printf("Parse error: Missing parameter for lens %d: Index of Refraction.\n", globals.numberOfLenses);
        return;
    }
    
    globals.lenses = realloc(globals.lenses, sizeof(LensData) * (globals.numberOfLenses + 1));
    globals.lenses[globals.numberOfLenses] = parsedLens;
    printLens(globals.lenses[globals.numberOfLenses]);
    globals.numberOfLenses++;
}


GLfloat parseRefractionSetting(){
    char * token = strtok(NULL, ",; \t\n");
    
    if(token != NULL){
        return((GLfloat)atof(token));
    }
    else{
        printf("Parse error: Unrecognized index of refraction value.\n");
        return(1.0);
    }
}
