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
    Point3D position;
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
    Point3D * center;
    double xLength;
    double yLength;
    double zLength;
    
    double xAxisRotation;
    double yAxisRotation;
    double zAxisRotation;
} RectPrism;

typedef union{
    Sphere sphere;
    Polygon polygon;
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
    
    Point3D viewPoint;
    Polygon viewPlane;
    
    Boolean ambientDiffuse;
    Boolean reflections;
    Boolean refractions;
    Boolean transparency;
} GlobalVars;

/*OpenGL related functions*/
void init (void);
void display (void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

/*Shape functions*/
Shape sphere_create(Point3D pos, double radius);
Shape polygon_create(Point3D * points, int numberOfPoints);
Shape rectPrism_create(Point3D pos, double xLen, double yLen, double zLen,
                       double xRotation, double yRotation, double zRotation);
Boolean testIntersection(Shape theShape, Vector3D line);
Boolean sphereIntersection(Sphere theSphere, Vector3D line);
Boolean polygonIntersection(Polygon thePoly, Vector3D line);


