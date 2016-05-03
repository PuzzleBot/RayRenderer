#include "raytrace.h"

Globals globals;

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize (START_WIDTH, START_HEIGHT);
    glutCreateWindow (argv[0]);
    init();
    //glutReshapeFunc(reshape);
    //glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return(0);
}