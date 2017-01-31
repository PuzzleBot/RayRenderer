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

double getLengthFromOrigin(Point3D end){
    return(sqrt(pow(end.x, 2) + pow(end.y, 2) + pow(end.z, 2)));
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
    
    if(radianAngle < (M_PI / 2)){
        return true;
    }
    else{
        return false;
    }
}


double angleBetween(Vector3D v1, Vector3D v2){
    double dotValue = dotProduct(v1, v2);
    
    if(dotValue > 1){
        dotValue = 1;
    }
    else if(dotValue < -1){
        dotValue = -1;
    }
    
    return acos(dotValue / (getLengthFromOrigin(v1.direction) * getLengthFromOrigin(v1.direction)));
}


Point3D nullPoint(){
    /*An point at z < -999 is very far behind the camera, and probably won't
      come into play, since it will likely be too far away to be seen in a
      reflection, and can't be seen under normal circumstances. We will use
      this as a "null" value.*/
    Point3D nullPoint;
    
    nullPoint.x = -4.0;
    nullPoint.y = -4.0;
    nullPoint.z = -999.0;
    
    return(nullPoint);
}

Boolean isNullPoint(Point3D point){
    if(point.z < -998){
        return true;
    }
    else{
        return false;
    }
}
