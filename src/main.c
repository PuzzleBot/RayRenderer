#include "raytrace.h"

GlobalVars globals;

void initGlobals(){
    globals.shapes = NULL;
    globals.numberOfShapes = 0;
    
    globals.pixels = calloc(START_WIDTH * START_HEIGHT * 3, sizeof(GLfloat));
    
    globals.viewPoint.x = 0;
    globals.viewPoint.y = 0;
    globals.viewPoint.z = -START_WIDTH / 2;
    
    /*Top left corner*/
    globals.viewPlane[0].x = -START_WIDTH / 2;
    globals.viewPlane[0].y = START_HEIGHT / 2;
    globals.viewPlane[0].z = 0;
    
    /*Top right corner*/
    globals.viewPlane[1].x = START_WIDTH / 2;
    globals.viewPlane[1].y = START_HEIGHT / 2;
    globals.viewPlane[1].z = 0;
    
    /*Bottom left corner*/
    globals.viewPlane[2].x = -START_WIDTH / 2;
    globals.viewPlane[2].y = -START_HEIGHT / 2;
    globals.viewPlane[2].z = 0;
    
    /*Bottom right corner*/
    globals.viewPlane[3].x = -START_WIDTH / 2;
    globals.viewPlane[3].y = -START_HEIGHT / 2;
    globals.viewPlane[3].z = 0;
    
    globals.ambientDiffuse = true;
    globals.reflections = false;
    globals.refractions = false;
    globals.transparency = false;
}

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize (START_WIDTH, START_HEIGHT);
    glutCreateWindow (argv[0]);
    init();
    initGlobals();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return(0);
}