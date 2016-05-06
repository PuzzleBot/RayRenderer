#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

#define START_WIDTH 1024
#define START_HEIGHT 768

typedef enum{false, true} Boolean;

/*Vector structures*/
typedef struct{
    double x;
    double y;
    double z;
} Point3D;

typedef struct{
    /*Position of the vector's start*/
    Point3D position;
    
    /*Direction of the vector, stored as a point in relation to the origin*/
    Point3D direction;
} Vector3D;


/*Shape structures*/
typedef enum{SPHERE, POLYGON} ShapeType;

typedef struct{
    Point3D position;
    double radius;
} Sphere;

typedef struct{
    Point3D * points;
    Vector3D normal;
    int numberOfPoints;
} Polygon;

typedef struct{
    Point3D points[3];
    Vector3D normal;
} Triangle;

typedef union{
    Sphere sphere;
    Polygon polygon;
    Triangle triangle;
} Shape;


typedef struct{
    Shape theShape;
    ShapeType type;
} ShapeData;

/*Global variable structure (Singleton pattern)*/
typedef struct{
    ShapeData * shapes;
    int numberOfShapes;
    
    GLfloat * pixels;
    
    double planeWidth;
    double planeHeight;
    Point3D viewPoint;
    Point3D viewPlane[4];
    
    Boolean ambientDiffuse;
    Boolean reflections;
    Boolean refractions;
    Boolean transparency;
} GlobalVars;


/*Parsing and allocation functions*/
void parseFile(char * inputFilePath);
void deallocExit(int exitCode);

/*OpenGL related functions*/
void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

/*Shape functions*/
Shape sphere_create(Point3D pos, double radius);
Shape polygon_create(Point3D * points, int numberOfPoints);
Boolean testIntersection(Shape theShape, Vector3D line);
Boolean sphereIntersection(Sphere theSphere, Vector3D line);
Boolean polygonIntersection(Polygon thePoly, Vector3D line);

/*Vector functions*/
Vector3D crossProduct(Vector3D v1, Vector3D v2);
double dotProduct(Vector3D v1, Vector3D v2);
Vector3D normalize(Vector3D v);
double getLength(Vector3D v);

/*Pixel drawing functions*/
void drawPixels();
void insertPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);

