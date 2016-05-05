#include "raytrace.h"

extern GlobalVars globals;

/*Shape constructors*/
Shape sphere_create(Point3D pos, double radius){
    
}

Shape polygon_create(Point3D * points, int numberOfPoints){
    
}

Shape rectPrism_create(Point3D pos, double xLen, double yLen, double zLen,
                       double xRotation, double yRotation, double zRotation){
    
}

/*Intersection test between a ray and a shape*/
Boolean testIntersection(Shape theShape, Vector3D line){
    
}

Boolean sphereIntersection(Sphere theSphere, Vector3D line){
    
}

Boolean polygonIntersection(Polygon thePoly, Vector3D line){
    
}