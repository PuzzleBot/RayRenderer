#include "raytrace.h"

GlobalVars globals;

void testRay(Triangle triangle, double rayPosX, double rayPosY, double rayPosZ, Boolean expected){
    Point3D intersection;
    Vector3D ray;
    
    ray.position.x = rayPosX;
    ray.position.y = rayPosY;
    ray.position.z = rayPosZ;
    
    ray.direction.x = 0;
    ray.direction.y = 0;
    ray.direction.z = 1;
    
    intersection = triangleIntersection(triangle, ray);
    
    if(isNullPoint(intersection) == true){
        printf("Test: No intersection. ");
        if(expected == false){
            printf("PASS\n");
        }
        else{
            printf("FAIL\n");
        }
    }
    else{
        printf("Test: Intersection present. ");
        if(expected == true){
            printf("PASS\n");
        }
        else{
            printf("FAIL\n");
        }
        
        printf("\tx=%.2f, y=%.2f, z=%.2f\n", intersection.x, intersection.y, intersection.z);
    }
}

int main(){
    Triangle triangle;
    
    /*Basic isoseles triangle*/
    triangle.points[0].x = 10;
    triangle.points[0].y = -5;
    triangle.points[0].z = 10;
    
    triangle.points[1].x = -10;
    triangle.points[1].y = -5;
    triangle.points[1].z = 10;
    
    triangle.points[2].x = 0;
    triangle.points[2].y = 10;
    triangle.points[2].z = 10;
    
    testRay(triangle, 0, 0, 0, true);
    testRay(triangle, 100, 0, 0, false);
    testRay(triangle, -100, 0, 0, false);
    testRay(triangle, 0, 100, 0, false);
    testRay(triangle, 0, -100, 0, false);
    testRay(triangle, 0, 5, 0, true);
    
    return(0);
}
