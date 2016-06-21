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
Point3D getIntersection(ShapeData shape, Vector3D ray){
    switch(shape.type){
        case SPHERE:
            return sphereIntersection(shape.theShape.sphere, ray);
        case TRIANGLE:
            return triangleIntersection(shape.theShape.triangle, ray);
        case POLYGON:
            return polygonIntersection(shape.theShape.polygon, ray);
        default:
            return nullIntersection();
    }
}

/*Intersection test for a sphere*/
Point3D sphereIntersection(Sphere sphere, Vector3D ray){
    Point3D intersection1;
    Point3D intersection2;
    
    double quadraticA = 1;
    
    double quadraticB = 2 * ((ray.direction.x * (ray.position.x - sphere.position.x))
                              + (ray.direction.y * (ray.position.y - sphere.position.y))
                                 + (ray.direction.z * (ray.position.z - sphere.position.z)));
                                 
    double quadraticC = pow((ray.position.x - sphere.position.x), 2)
                        + pow((ray.position.y - sphere.position.y), 2)
                        + pow((ray.position.z - sphere.position.z), 2)
                        - pow(sphere.radius, 2);
    
    double discriminant = (pow(quadraticB, 2) - (4 * quadraticA * quadraticC));
    
    double quadraticNegativeT;
    double quadraticPositiveT;

                                 
    /*Test discriminant, if not negative an intersection exists*/
    if(discriminant >= 0){
        quadraticNegativeT = (-quadraticB - sqrt(discriminant)) / 2;
        quadraticPositiveT = (-quadraticB + sqrt(discriminant)) / 2;
        
        intersection1.x = ray.position.x + (ray.direction.x * quadraticNegativeT);
        intersection1.y = ray.position.y + (ray.direction.y * quadraticNegativeT);
        intersection1.z = ray.position.z + (ray.direction.z * quadraticNegativeT);
        
        intersection2.x = ray.position.x + (ray.direction.x * quadraticPositiveT);
        intersection2.y = ray.position.y + (ray.direction.y * quadraticPositiveT);
        intersection2.z = ray.position.z + (ray.direction.z * quadraticPositiveT);
        
        if(getLength(ray.position, intersection1) < getLength(ray.position, intersection2)){
            return(intersection1);
        }
        else{
            return(intersection2);
        }
    }
    
    return nullIntersection();
}


Point3D triangleIntersection(Triangle triangle, Vector3D ray){
    return nullIntersection();
}

Point3D polygonIntersection(Polygon poly, Vector3D ray){
    return nullIntersection();
}


Point3D nullIntersection(){
    /*Any intersection point with a negative z can't be seen by the viewplane, and will be considered
      as a null intersection.*/
    Point3D nullIntersection;
    
    nullIntersection.x = -4.0;
    nullIntersection.y = -4.0;
    nullIntersection.z = -999.0;
    
    return(nullIntersection);
}

Boolean isNullIntersection(Point3D intersection){
    if(intersection.z > 0){
        return true;
    }
    else{
        return false;
    }
}
