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


union{
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
    Boolean ambientDiffuse;
    Boolean reflections;
    Boolean refractions;
    Boolean transparency;
} Globals;

