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
void parseTriangle(GLfloat refractIndex);
void parseSphere(GLfloat refractIndex);
void parseLight();
void parseLens();
GLfloat parseRefractionSetting();

void deallocExit(int exitCode);

/*OpenGL related functions (glListeners.c)*/
void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

/*Shape functions (shape.c, intersection.c)*/
void printShape(ShapeData shape);
void printLight(LightData light);
void printLens(LensData lens);
Point3D getIntersection(ShapeData theShape, Vector3D ray);
Point3D sphereIntersection(Sphere theSphere, Vector3D ray);
Point3D triangleIntersection(Triangle theSphere, Vector3D ray);
Point3D polygonIntersection(Polygon thePoly, Vector3D ray);
Point3D planeIntersection(Vector3D planeNormal, Vector3D ray);
Point3D getViewPlaneCoordinates(int x, int y);

Vector3D getNormal(ShapeData shape, Point3D pointOnShape);
Vector3D sphereNormal(Sphere sphere, Point3D pointOnShape);
Vector3D triangleNormal(Triangle triangle, Point3D pointOnShape);
Vector3D polygonNormal(Polygon poly, Point3D pointOnShape);


/*Lens-specific functions (lens.c)*/
Point3D lensIntersection(LensData lens, Vector3D ray);
Vector3D lensNormal(LensData lens, Point3D pointOnLens);
void sortLensList();
Vector3D traceLenses(Vector3D ray, int iterationLevel);

/*Math functions for the starburst effect (starburst.c)*/
void computeStarburstTexture(LightData light, int x, int y, int lightPixelX, int lightPixelY);
double sincOperation(long double x);


/*Pseudo-ghost computation functions (pseudoGhost.c)*/
void generateGhostTexture();
IntegerList * integerList_addToFront(IntegerList * list, int x, int y);
void integerList_destroyList(IntegerList * list);
IntegerList * sampleAllBrightSpots(GLfloat * pixels, int * minX, int * minY, int * maxX, int * maxY);
void copyAndRescaleBrightSpots(GLfloat * starburstPixels, GLfloat * ghostPixels, IntegerList * brightPixelIndexList, double newXcenter, double newYcenter, double horizontalScale, double verticalScale, double opacity);


/*Reflections and refractions (shape.c)*/
Vector3D getReflection(Vector3D lightToIntersection, Vector3D normal, Point3D intersection);
Vector3D getRefraction(Vector3D rayToIntersection, Vector3D normal, double oldRefractIndex, double newRefractIndex, Point3D intersection);

/*Vector and point functions (vector.c)*/
Vector3D crossProduct(Vector3D v1, Vector3D v2);
double dotProduct(Vector3D v1, Vector3D v2);
Vector3D normalize(Vector3D v);
double getLength(Point3D start, Point3D end);
double getLengthFromOrigin(Point3D end);

Vector2D getPerpendicular(Vector2D v);
double dotProduct2D(Vector2D v1, Vector2D v2);
Vector2D normalize2D(Vector2D v);
double getLength2D(Point2D start, Point2D end);

Vector3D pointToVector(Point3D point);
Boolean isInRayPath(Vector3D ray, Point3D testPoint);
double angleBetween(Vector3D v1, Vector3D v2);
void printVector(Vector3D vector);

/*Define a "NULL" for points and a check for it (vector.c)*/
Point3D nullPoint();
Boolean isNullPoint(Point3D point);

/*Ray tracing-specific functions (ray.c)*/
ColourRGB traceRay(Vector3D ray, int currentIteration, double currentRefractIndex);
ShapeData * getFirstIntersectedShape(Vector3D ray);

/*Pixel drawing functions (pixels.c)*/
void drawPixels();
void insertPixel(GLfloat * pixelArray, int width, int height, int x, int y, GLfloat r, GLfloat g, GLfloat b);
GLfloat getOverlayPixel(GLfloat * pixelArray, int width, int height, int x, int y, int rgbOption);
void insertOverlayPixel(GLfloat * pixelArray, int width, int height, int x, int y, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
GLfloat getOverlayPixel(GLfloat * pixelArray, int width, int height, int x, int y, int rgbaOption);
void getIntersectedScreenPixel(Vector3D v, int * pixelXstorage, int * pixelYstorage);
void computeOverlayEffects();
void addLightBlot(LightData light, int pixelX, int pixelY);
void combinePixelColours(GLfloat * objectPixels, GLfloat * texturePixels, int width, int height, int x, int y);
void combineAllGlobalPixels(int x, int y);

/*Interface drawing functions (interface.c)*/
void initLoadingBar();
void incrementLoadingBar();
