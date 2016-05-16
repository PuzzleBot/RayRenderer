#include "raytrace.h"

extern GlobalVars globals;

void printShape(ShapeData shape){
    printf("Shape contents:\n");
    
    if(shape.type == TRIANGLE){
        printf("\tTriangle\n");
        printf("\tPoint 1: %.2f, %.2f, %.2f\n", shape.theShape.triangle.points[0].x, shape.theShape.triangle.points[0].y, shape.theShape.triangle.points[0].z);
        printf("\tPoint 2: %.2f, %.2f, %.2f\n", shape.theShape.triangle.points[1].x, shape.theShape.triangle.points[1].y, shape.theShape.triangle.points[1].z);
        printf("\tPoint 3: %.2f, %.2f, %.2f\n", shape.theShape.triangle.points[2].x, shape.theShape.triangle.points[2].y, shape.theShape.triangle.points[2].z);
    }
    else if(shape.type == SPHERE){
        printf("\tSphere\n");
        printf("\tPosition: %.2f, %.2f, %.2f\n", shape.theShape.sphere.position.x, shape.theShape.sphere.position.y, shape.theShape.sphere.position.z);
        printf("\tRadius: %.2f\n", shape.theShape.sphere.radius);
    }
    
    printf("\tColour: %.2f, %.2f, %.2f\n", shape.r, shape.g, shape.b);
    printf("\n");
}

/*Intersection test between a ray and a shape (wrapper containing separate calls for different types of shapes)*/
Boolean testIntersection(ShapeData shape, Vector3D line){
    switch(shape.type){
        case SPHERE:
            return sphereIntersection(shape.theShape.sphere, line);
        case TRIANGLE:
            return triangleIntersection(shape.theShape.triangle, line);
        case POLYGON:
            return polygonIntersection(shape.theShape.triangle, line);
        default:
            return false;
    }
}

Boolean sphereIntersection(Sphere sphere, Vector3D line){
    return false;
}

Boolean triangleIntersection(Triangle triangle, Vector3D line){
    return false;
}

Boolean polygonIntersection(Polygon poly, Vector3D line){
    return false;
}