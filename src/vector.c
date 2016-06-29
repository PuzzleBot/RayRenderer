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
    double length = (sqrt(pow(v.direction.x, 2) + pow(v.direction.y, 2) + pow(v.direction.z, 2)));
    
    v.direction.x = v.direction.x / length;
    v.direction.y = v.direction.y / length;
    v.direction.z = v.direction.z / length;
    
    return v;
}

double getLength(Point3D start, Point3D end){
    return(sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2) + pow(end.z - start.z, 2)));
}

Vector3D pointToVector(Point3D point){
    Vector3D vector;
    
    vector.position.x = 0;
    vector.position.y = 0;
    vector.position.z = 0;
    
    vector.direction.x = point.x;
    vector.direction.y = point.y;
    vector.direction.z = point.z;
    
    return(vector);
}

Point3D nullPoint(){
    /*Any point with a negative z can't be seen by the viewplane, and will be considered
     as a null point.*/
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
