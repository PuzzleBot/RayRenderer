#include "raytrace.h"

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
