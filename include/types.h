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

/*All custom data types*/

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

typedef struct{
    GLfloat red;
    GLfloat green;
    GLfloat blue;
} ColourRGB;


/*Shape structures*/
typedef enum{SPHERE, POLYGON, TRIANGLE} ShapeType;

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
    double r;
    double g;
    double b;
} ShapeData;


/*Global variable structure (Singleton pattern)*/
typedef struct{
    /*Both store the same shapes, but one is an array and the other is a list*/
    ShapeData * shapes;
    int numberOfShapes;
    
    GLfloat * pixels;
    
    /*Width and height of the viewing plane*/
    double planeWidth;
    double planeHeight;
    
    /*Viewpoint position: Rays will start from this point*/
    Point3D viewPoint;
    
    /*A square defining the viewing plane. Should always be aligned with two axes.*/
    Point3D viewPlane[2][2];
    
    Boolean ambientDiffuse;
    Boolean reflections;
    Boolean refractions;
    Boolean transparency;
} GlobalVars;