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

Vector3D getNormal(ShapeData shape, Point3D pointOnShape);
Vector3D sphereNormal(Sphere sphere, Point3D pointOnShape);
Vector3D triangleNormal(Triangle triangle, Point3D pointOnShape);
Vector3D polygonNormal(Polygon poly, Point3D pointOnShape);
Point3D planeIntersection(Vector3D planeNormal, Vector3D ray);

Vector3D getReflection(Vector3D lightToIntersection, Vector3D normal);

/*Vector and point functions*/
Vector3D crossProduct(Vector3D v1, Vector3D v2);
double dotProduct(Vector3D v1, Vector3D v2);
Vector3D normalize(Vector3D v);
double getLength(Point3D start, Point3D end);
Vector3D pointToVector(Point3D point);
Point3D nullPoint();
Boolean isNullPoint(Point3D point);

/*Ray tracing-specific functions*/
ColourRGB traceRay(Vector3D ray, int currentIteration);
ShapeData * getFirstIntersectedShape(Vector3D ray);
Boolean isInRayPath(Vector3D ray, Point3D testPoint);

/*Pixel drawing functions*/
void drawPixels();
void insertPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);
