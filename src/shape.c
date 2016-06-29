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
    
    printf("\tColour: %.2f, %.2f, %.2f\n", shape.colour.red, shape.colour.green, shape.colour.blue);
    printf("\n");
}

void printLight(LightData light){
    printf("Light contents:\n");
    printf("\tPosition: %.2f, %.2f, %.2f\n", light.position.x, light.position.y, light.position.z);
    printf("\tColour: %.2f, %.2f, %.2f\n", light.colour.red, light.colour.green, light.colour.blue);
    printf("\n");
}


Vector3D getNormal(ShapeData shape, Point3D pointOnShape){
    Vector3D nullVector;
    
    switch(shape.type){
        case SPHERE:
            return sphereNormal(shape.theShape.sphere, pointOnShape);
        case TRIANGLE:
            return triangleNormal(shape.theShape.triangle, pointOnShape);
        case POLYGON:
            return polygonNormal(shape.theShape.polygon, pointOnShape);
        default:
            nullVector.position = pointOnShape;
            nullVector.direction = nullPoint();
            return nullVector;
    }
}

Vector3D sphereNormal(Sphere sphere, Point3D pointOnShape){
    Vector3D normalVector;
    
    normalVector.position = pointOnShape;
    
    normalVector.direction.x = (pointOnShape.x - sphere.position.x) / sphere.radius;
    normalVector.direction.y = (pointOnShape.y - sphere.position.y) / sphere.radius;
    normalVector.direction.z = (pointOnShape.z - sphere.position.z) / sphere.radius;
    
    return normalVector;
}

Vector3D triangleNormal(Triangle triangle, Point3D pointOnShape){
    Vector3D planeVector1;
    Vector3D planeVector2;
    
    if(isNullPoint(triangle.normal.direction) == true){
        planeVector1.position = triangle.points[0];
        planeVector1.direction.x = triangle.points[1].x - triangle.points[0].x;
        planeVector1.direction.y = triangle.points[1].y - triangle.points[0].y;
        planeVector1.direction.z = triangle.points[1].z - triangle.points[0].z;
        planeVector1 = normalize(planeVector1);
        
        planeVector2.position = triangle.points[0];
        planeVector2.direction.x = triangle.points[2].x - triangle.points[0].x;
        planeVector2.direction.y = triangle.points[2].y - triangle.points[0].y;
        planeVector2.direction.z = triangle.points[2].z - triangle.points[0].z;
        planeVector2 = normalize(planeVector2);
        
        return(normalize(crossProduct(planeVector1, planeVector2)));
    }
    else{
        return(triangle.normal);
    }
}

Vector3D polygonNormal(Polygon poly, Point3D pointOnShape){
    Vector3D nullVector;
    
    nullVector.position = pointOnShape;
    nullVector.direction = nullPoint();
    return nullVector;
}

Vector3D getReflection(Vector3D lightToIntersection, Vector3D normal){
    Vector3D reflectedRay;
    double dotValue = -(dotProduct(normal, lightToIntersection));
    
    reflectedRay.direction.x = lightToIntersection.direction.x + (2 * normal.direction.x * dotValue);
    reflectedRay.direction.y = lightToIntersection.direction.y + (2 * normal.direction.y * dotValue);
    reflectedRay.direction.z = lightToIntersection.direction.z + (2 * normal.direction.z * dotValue);
    
    return(reflectedRay);
}
