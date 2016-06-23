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
            return nullPoint();
    }
}

/*Intersection test for a sphere*/
Point3D sphereIntersection(Sphere sphere, Vector3D ray){
    Point3D intersection1;
    Point3D intersection2;
    
    double quadraticA = 1;
    double quadraticB;
    double quadraticC;
    
    double discriminant;
    
    double quadraticNegativeT;
    double quadraticPositiveT;
    
    double inter1Length;
    double inter2Length;
    
    
    quadraticB = 2 * ((ray.direction.x * (ray.position.x - sphere.position.x))
                      + (ray.direction.y * (ray.position.y - sphere.position.y))
                      + (ray.direction.z * (ray.position.z - sphere.position.z)));
    
    quadraticC = pow((ray.position.x - sphere.position.x), 2)
    + pow((ray.position.y - sphere.position.y), 2)
    + pow((ray.position.z - sphere.position.z), 2)
    - pow(sphere.radius, 2);
    
    discriminant = (pow(quadraticB, 2) - (4 * quadraticA * quadraticC));
    
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
        
        inter1Length = getLength(ray.position, intersection1);
        inter2Length = getLength(ray.position, intersection2);
        if(inter1Length < inter2Length){
            /*If the ray is cast from the sphere and is actually intersecting itself 
             (ray origin to intersection distance ~= 0), return a null instead*/
            if(inter1Length < 0.001){
                return nullPoint();
            }
            else{
                /*Ray direction test: If the intersection is on the side of the ray origin opposite to the ray's
                 direction, no intersection can exist*/
                if(isInRayPath(ray, intersection1) == false){
                    return nullPoint();
                }
                else{
                    return(intersection1);
                }
            }
        }
        else{
            if(inter2Length < 0.001){
                return nullPoint();
            }
            else{
                /*Ray direction test: If the intersection is on the side of the ray origin opposite to the ray's
                 direction, no intersection can exist*/
                if(isInRayPath(ray, intersection2) == false){
                    return nullPoint();
                }
                else{
                    return(intersection2);
                }
            }
        }
    }
    
    return nullPoint();
}


Point3D triangleIntersection(Triangle triangle, Vector3D ray){
    return nullPoint();
}

Point3D polygonIntersection(Polygon poly, Vector3D ray){
    return nullPoint();
}


Boolean isInRayPath(Vector3D ray, Point3D testPoint){
    Vector3D rayToPoint;
    double dotValue;
    double radianAngle;
    
    rayToPoint.position = ray.position;
    rayToPoint.direction.x = testPoint.x - rayToPoint.position.x;
    rayToPoint.direction.y = testPoint.y - rayToPoint.position.y;
    rayToPoint.direction.z = testPoint.z - rayToPoint.position.z;
    
    ray = normalize(ray);
    rayToPoint = normalize(rayToPoint);
    
    dotValue = dotProduct(ray, rayToPoint);
    if(dotValue > 1){
        dotValue = 1;
    }
    else if(dotValue < -1){
        dotValue = -1;
    }
    
    radianAngle = acos(dotValue);
    
    if((radianAngle > (-M_PI / 2)) && (radianAngle < (M_PI / 2))){
        return true;
    }
    else{
        return false;
    }
}


Point3D nullPoint(){
    /*Any intersection point with a negative z can't be seen by the viewplane, and will be considered
      as a null intersection.*/
    Point3D nullPoint;
    
    nullPoint.x = -4.0;
    nullPoint.y = -4.0;
    nullPoint.z = -999.0;
    
    return(nullPoint);
}

Boolean isNullPoint(Point3D point){
    if(point.z < -1){
        return true;
    }
    else{
        return false;
    }
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
    Vector3D nullVector;
    
    nullVector.position = pointOnShape;
    nullVector.direction = nullPoint();
    return nullVector;
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
