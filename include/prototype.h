#include "types.h"

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#endif

/*All function prototypes*/

/*Parsing and allocation functions*/
void parseFile(char * inputFilePath);
void parseTriangle();
void parseSphere();
void parseLight();

void deallocExit(int exitCode);

/*OpenGL related functions*/
void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

/*Shape functions*/
void printShape(ShapeData shape);
void printLight(LightData light);
Point3D getIntersection(ShapeData theShape, Vector3D ray);
Point3D sphereIntersection(Sphere theSphere, Vector3D ray);
Point3D triangleIntersection(Triangle theSphere, Vector3D ray);
Point3D polygonIntersection(Polygon thePoly, Vector3D ray);
Point3D nullIntersection();
Boolean isNullIntersection(Point3D intersection);

/*Vector functions*/
Vector3D crossProduct(Vector3D v1, Vector3D v2);
double dotProduct(Vector3D v1, Vector3D v2);
Vector3D normalize(Vector3D v);
double getLength(Point3D start, Point3D end);

/*Ray tracing-specific functions*/
ColourRGB traceRay(Vector3D ray);
ShapeData * getFirstIntersectedShape(Vector3D ray);

/*Pixel drawing functions*/
void drawPixels();
void insertPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);
