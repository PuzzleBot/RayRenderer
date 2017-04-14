#include "raytrace.h"

extern GlobalVars globals;

/*Intersection test between a ray and a shape (algorithm dependent on shape type)
  Sort-of polymorphism*/
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
                if(isInRayPath(ray, intersection1) == true){
                    return(intersection1);
                }
            }
        }


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

    return nullPoint();
}

/*http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf*/
Point3D triangleIntersection(Triangle triangle, Vector3D ray){
    Point3D intersection;

    /*Matrices for the calculation*/
    double rightSideTarget[3];
    double matrixM[3][3];
    double columnBuffer[3];

    double determinantM;
    double determinantMt;
    double determinantMu;
    double determinantMv;

    double rayDistance;
    double uvCoordinate[2];


    /*Moller-Trumbore intersection method*/
    /*Matrix M = [-D, V1 - V0, V2 - V0]
      where D is the direction of the ray, V0, V1, V2 are triangle points*/
    matrixM[0][0] = -ray.direction.x;
    matrixM[1][0] = -ray.direction.y;
    matrixM[2][0] = -ray.direction.z;

    matrixM[0][1] = triangle.points[1].x - triangle.points[0].x;
    matrixM[1][1] = triangle.points[1].y - triangle.points[0].y;
    matrixM[2][1] = triangle.points[1].z - triangle.points[0].z;

    matrixM[0][2] = triangle.points[2].x - triangle.points[0].x;
    matrixM[1][2] = triangle.points[2].y - triangle.points[0].y;
    matrixM[2][2] = triangle.points[2].z - triangle.points[0].z;

    /*Calculate the determinant of the created matrix*/
    determinantM = (matrixM[0][0] * matrixM[1][1] * matrixM[2][2])
                 + (matrixM[0][1] * matrixM[1][2] * matrixM[2][0])
                 + (matrixM[0][2] * matrixM[1][0] * matrixM[2][1])
                 - (matrixM[0][2] * matrixM[1][1] * matrixM[2][0])
                 - (matrixM[0][1] * matrixM[1][0] * matrixM[2][2])
                 - (matrixM[0][0] * matrixM[1][2] * matrixM[2][1]);

    /*Ray Origin point (O) - triangle point (V0) = Q*/
    rightSideTarget[0] = ray.position.x - triangle.points[0].x;
    rightSideTarget[1] = ray.position.y - triangle.points[0].y;
    rightSideTarget[2] = ray.position.z - triangle.points[0].z;

    /*Solve the system of equations resulting from MT = Q,
     where T = [t, u, v], t is the distance from the ray origin to the intersection,
     u and v are the coordinates of the intersection in the transformed basis.
     u + v <= 1, and u, v >= 0 for an intersection.
        Method of solving the system: Cramer's Rule
        http://www.purplemath.com/modules/cramers.htm*/

    /*Solve for u: calculate the determinant of M with the second column replaced by Q, then
      divide the new determinant by the old determinant*/
    /*First, save the column being modified so that it can be restored after*/

    columnBuffer[0] = matrixM[0][1];
    columnBuffer[1] = matrixM[1][1];
    columnBuffer[2] = matrixM[2][1];

    matrixM[0][1] = rightSideTarget[0];
    matrixM[1][1] = rightSideTarget[1];
    matrixM[2][1] = rightSideTarget[2];

    determinantMu = (matrixM[0][0] * matrixM[1][1] * matrixM[2][2])
                    + (matrixM[0][1] * matrixM[1][2] * matrixM[2][0])
                    + (matrixM[0][2] * matrixM[1][0] * matrixM[2][1])
                    - (matrixM[0][2] * matrixM[1][1] * matrixM[2][0])
                    - (matrixM[0][1] * matrixM[1][0] * matrixM[2][2])
                    - (matrixM[0][0] * matrixM[1][2] * matrixM[2][1]);

    matrixM[0][1] = columnBuffer[0];
    matrixM[1][1] = columnBuffer[1];
    matrixM[2][1] = columnBuffer[2];

    uvCoordinate[0] = determinantMu / determinantM;

    /*Solve for v: same process but with the third column*/
    columnBuffer[0] = matrixM[0][2];
    columnBuffer[1] = matrixM[1][2];
    columnBuffer[2] = matrixM[2][2];

    matrixM[0][2] = rightSideTarget[0];
    matrixM[1][2] = rightSideTarget[1];
    matrixM[2][2] = rightSideTarget[2];

    determinantMv = (matrixM[0][0] * matrixM[1][1] * matrixM[2][2])
    + (matrixM[0][1] * matrixM[1][2] * matrixM[2][0])
    + (matrixM[0][2] * matrixM[1][0] * matrixM[2][1])
    - (matrixM[0][2] * matrixM[1][1] * matrixM[2][0])
    - (matrixM[0][1] * matrixM[1][0] * matrixM[2][2])
    - (matrixM[0][0] * matrixM[1][2] * matrixM[2][1]);

    matrixM[0][2] = columnBuffer[0];
    matrixM[1][2] = columnBuffer[1];
    matrixM[2][2] = columnBuffer[2];

    uvCoordinate[1] = determinantMv / determinantM;


    /*Determine if there is an intersection: u + v <= 1, and u, v >= 0 for any point in the triangle*/
    if((uvCoordinate[0] + uvCoordinate[1] > 1.001) || (uvCoordinate[0] < -0.001) || (uvCoordinate[1] < -0.001)){
        return(nullPoint());
    }


    /*Solve for t to get the intersection, same process as u and v but with the first column
      Since t is the length from the vp to the intersection, the intersection's coordinates
      can be found easily using the original vector*/
    columnBuffer[0] = matrixM[0][0];
    columnBuffer[1] = matrixM[1][0];
    columnBuffer[2] = matrixM[2][0];

    matrixM[0][0] = rightSideTarget[0];
    matrixM[1][0] = rightSideTarget[1];
    matrixM[2][0] = rightSideTarget[2];

    determinantMt = (matrixM[0][0] * matrixM[1][1] * matrixM[2][2])
    + (matrixM[0][1] * matrixM[1][2] * matrixM[2][0])
    + (matrixM[0][2] * matrixM[1][0] * matrixM[2][1])
    - (matrixM[0][2] * matrixM[1][1] * matrixM[2][0])
    - (matrixM[0][1] * matrixM[1][0] * matrixM[2][2])
    - (matrixM[0][0] * matrixM[1][2] * matrixM[2][1]);

    matrixM[0][0] = columnBuffer[0];
    matrixM[1][0] = columnBuffer[1];
    matrixM[2][0] = columnBuffer[2];

    rayDistance = determinantMt / determinantM;

    if(rayDistance < 0.01){
        return nullPoint();
    }

    intersection.x = ray.position.x + (ray.direction.x * rayDistance);
    intersection.y = ray.position.y + (ray.direction.y * rayDistance);
    intersection.z = ray.position.z + (ray.direction.z * rayDistance);

    /*Ray direction test: If the intersection is on the side of the ray origin opposite to the ray's
     direction, no intersection can exist*/
    if(isInRayPath(ray, intersection) == false){
        return nullPoint();
    }
    else{
        return(intersection);
    }
}

Point3D polygonIntersection(Polygon poly, Vector3D ray){
    return nullPoint();
}


/*http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection*/
Point3D planeIntersection(Vector3D planeNormal, Vector3D ray){
    Point3D intersection;
    Vector3D planeMinusLine;
    double scalarLength;
    double rayNormalDot = dotProduct(ray, planeNormal);

    if(rayNormalDot < 0.01){
        /*Ray is parallel to the plane - no intersection*/
        return nullPoint();
    }
    else{
        planeMinusLine.position = ray.position;
        planeMinusLine.direction.x = planeNormal.position.x - ray.position.x;
        planeMinusLine.direction.y = planeNormal.position.y - ray.position.y;
        planeMinusLine.direction.z = planeNormal.position.z - ray.position.z;

        scalarLength = dotProduct(planeMinusLine, planeNormal) / rayNormalDot;

        intersection.x = ray.position.x + (ray.direction.x * scalarLength);
        intersection.y = ray.position.y + (ray.direction.y * scalarLength);
        intersection.z = ray.position.z + (ray.direction.z * scalarLength);

        return(intersection);
    }
}

/*Get the point on the plane corresponding to the given x and y*/
Point3D getViewPlaneCoordinates(int x, int y){
    Point3D planePosition;

    double verticalMoveAmount = globals.planeHeight / START_HEIGHT;
    double horizontalMoveAmount = globals.planeWidth / START_WIDTH;

    planePosition.x = globals.viewPlane[1][0].x + (x * horizontalMoveAmount);
    planePosition.y = globals.viewPlane[1][0].y + (y * verticalMoveAmount);
    planePosition.z = globals.viewPlane[1][0].z;

    return planePosition;
}
