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
typedef enum{SPHERE, RECTANGLE} ShapeType;

typedef struct{
    Point3D position;
    double radius;
} Sphere;

typedef struct{
    Point3D point1;
    Point3D point2;
    Point3D point3;
    Point3D point4;
    Vector3D normal;
} Rectangle;

union{
    Sphere sphere;
    Rectangle rectangle;
} Shape;

typedef struct{
    Shape theShape;
    ShapeType type;
} ShapeData;

/*Global variable structure (Singleton pattern)*/
typedef struct{
    ShapeData * shapes;
} Globals;

