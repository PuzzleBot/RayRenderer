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

/*Intersection test between a ray and a shape (algorithm dependent on shape type)*/
Boolean testIntersection(ShapeData shape, Vector3D ray){
    switch(shape.type){
        case SPHERE:
            return sphereIntersection(shape.theShape.sphere, ray);
        case TRIANGLE:
            return triangleIntersection(shape.theShape.triangle, ray);
        case POLYGON:
            return polygonIntersection(shape.theShape.polygon, ray);
        default:
            return false;
    }
}

/*Intersection test for a sphere*/
Boolean sphereIntersection(Sphere sphere, Vector3D ray){
    double quadraticA = 1;
    
    double quadraticB = 2 * ((ray.direction.x * (ray.position.x - sphere.position.x))
                              + (ray.direction.y * (ray.position.y - sphere.position.y))
                                 + (ray.direction.z * (ray.position.z - sphere.position.z)));
                                 
    double quadraticC = pow((ray.position.x - sphere.position.x), 2)
                        + pow((ray.position.y - sphere.position.y), 2)
                        + pow((ray.position.z - sphere.position.z), 2)
                        - pow(sphere.radius, 2);

                                 
    /*Test discriminant, if not negative an intersection exists*/
    if((pow(quadraticB, 2) - (4 * quadraticA * quadraticC)) >= 0){
        return true;
    }
    
    return false;
}


Boolean triangleIntersection(Triangle triangle, Vector3D ray){
    return false;
}

Boolean polygonIntersection(Polygon poly, Vector3D ray){
    return false;
}