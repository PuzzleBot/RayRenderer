#include "raytrace.h"

extern GlobalVars globals;

/*Lens intersection. Basically the same as a sphere except with another check for concavity*/
Point3D lensIntersection(LensData lens, Vector3D ray){
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
    
    
    quadraticB = 2 * ((ray.direction.x * (ray.position.x - lens.position.x))
                      + (ray.direction.y * (ray.position.y - lens.position.y))
                      + (ray.direction.z * (ray.position.z - lens.position.z)));
    
    quadraticC = pow((ray.position.x - lens.position.x), 2)
    + pow((ray.position.y - lens.position.y), 2)
    + pow((ray.position.z - lens.position.z), 2)
    - pow(lens.radius, 2);
    
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
        
        /*Check concavity, take the correct intersection*/
        if(lens.isConvex == true){
            /*The closest intersection is the convex one*/
            if(inter1Length < inter2Length){
                if((isInRayPath(ray, intersection1)) && (inter1Length > 0.01)){
                    return intersection1;
                }
                else{
                    return nullPoint();
                }
            }
            else if(inter2Length < inter1Length){
                if((isInRayPath(ray, intersection2)) && (inter2Length > 0.01)){
                    return intersection2;
                }
                else{
                    return nullPoint();
                }
            }
        }
        else{
            /*Concave - the farthest intersection is the concave one*/
            if(inter1Length > inter2Length){
                if((isInRayPath(ray, intersection1)) && (inter1Length > 0.01)){
                    return intersection1;
                }
                else{
                    return nullPoint();
                }
            }
            else if(inter2Length > inter1Length){
                if((isInRayPath(ray, intersection2)) && (inter2Length > 0.01)){
                    return intersection2;
                }
                else{
                    return nullPoint();
                }
            }
            
        }
        
    }
    
    return nullPoint();
}

/*Find the normal on a point of the lens*/
Vector3D lensNormal(LensData lens, Point3D pointOnLens){
    Vector3D normalVector;
    
    normalVector.position = pointOnLens;
    
    normalVector.direction.x = (pointOnLens.x - lens.position.x) / lens.radius;
    normalVector.direction.y = (pointOnLens.y - lens.position.y) / lens.radius;
    normalVector.direction.z = (pointOnLens.z - lens.position.z) / lens.radius;
    
    return normalVector;
}
