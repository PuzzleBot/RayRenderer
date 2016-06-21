#include "raytrace.h"

GlobalVars globals;

void initGlobals(){
    globals.shapes = NULL;
    globals.numberOfShapes = 0;
    
    globals.lights = NULL;
    globals.numberOfLights = 0;
    
    globals.pixels = calloc(START_WIDTH * START_HEIGHT * 3, sizeof(GLfloat));
    
    globals.planeWidth = 102.4;
    globals.planeHeight = 76.8;
    
    globals.viewPoint.x = 0;
    globals.viewPoint.y = 0;
    globals.viewPoint.z = -globals.planeWidth / 2;
    
    /*Top left corner*/
    globals.viewPlane[0][0].x = -globals.planeWidth / 2;
    globals.viewPlane[0][0].y = globals.planeHeight / 2;
    globals.viewPlane[0][0].z = 0;
    
    /*Top right corner*/
    globals.viewPlane[0][1].x = globals.planeWidth / 2;
    globals.viewPlane[0][1].y = globals.planeHeight / 2;
    globals.viewPlane[0][1].z = 0;
    
    /*Bottom left corner*/
    globals.viewPlane[1][0].x = -globals.planeWidth / 2;
    globals.viewPlane[1][0].y = -globals.planeHeight / 2;
    globals.viewPlane[1][0].z = 0;
    
    /*Bottom right corner*/
    globals.viewPlane[1][1].x = globals.planeWidth / 2;
    globals.viewPlane[1][1].y = -globals.planeHeight / 2;
    globals.viewPlane[1][1].z = 0;
    
    globals.reflections = false;
    globals.refractions = false;
    globals.transparency = false;
    
    globals.ambientCoefficient = 0.2;
    globals.ambientColour.red = 1.0;
    globals.ambientColour.green = 1.0;
    globals.ambientColour.blue = 1.0;
    
    globals.lightAttenuation = 0.3;
}

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize (START_WIDTH, START_HEIGHT);
    glutCreateWindow (argv[0]);
    initGlobals();
    init();
    
    if(argc > 1){
        parseFile(argv[1]);
    }
    else{
        parseFile("./inputs/defaultFile.csv");
    }
    
    display();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return(0);
}

void deallocExit(int exitCode){
    if(globals.shapes != NULL){
        free(globals.shapes);
    }
    
    free(globals.pixels);
    exit(exitCode);
}