#include "raytrace.h"

extern GlobalVars globals;

/*  Initialize material property and light source.
 */
void init(void){

}


void display(void){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //drawPixels();
    
    glFlush ();
}


void reshape(int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 10.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}


void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'q':
            exit(0);
            break;
    }
}
