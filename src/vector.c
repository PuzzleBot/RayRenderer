#include "raytrace.h"

Vector3D crossProduct(Vector3D v1, Vector3D v2){
    Vector3D newVector;
    
    newVector.position = v1.position;
    
    newVector.direction.x = (v1.direction.y * v2.direction.z) - (v1.direction.z * v2.direction.y);
    newVector.direction.y = (v1.direction.z * v2.direction.x) - (v1.direction.x * v2.direction.z);
    newVector.direction.z = (v1.direction.x * v2.direction.y) - (v1.direction.y * v2.direction.x);
    
    return newVector;
}

double dotProduct(Vector3D v1, Vector3D v2){
    return( ((v1.direction.x) * (v2.direction.x)) +
            ((v1.direction.y) * (v2.direction.y)) +
            ((v1.direction.z) * (v2.direction.z)) );
}

Vector3D normalize(Vector3D v){
    double length = getLength(v);
    
    v.direction.x = v.direction.x / length;
    v.direction.y = v.direction.y / length;
    v.direction.z = v.direction.z / length;
    
    return v;
}

double getLength(Vector3D v){
    return(sqrt(pow(v.direction.x, 2) + pow(v.direction.y, 2) + pow(v.direction.z, 2)));
}
