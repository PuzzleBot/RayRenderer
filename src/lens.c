#include "raytrace.h"

extern GlobalVars globals;


void printLens(LensData lens){
    printf("Lens contents:\n");
    printf("\tPosition: %.2f, %.2f, %.2f\n", lens.truePosition.x, lens.truePosition.y, lens.truePosition.z);
    printf("\tRadius: %.2f\n", lens.radius);
    if(lens.isConvex == true){
        printf("\tConvex\n");
    }
    else{
        printf("\tConcave\n");
    }
    printf("\n");
}


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
            if((isInRayPath(ray, intersection1)) && (inter1Length > 0.01)){
                return intersection1;
            }
            else{
                return nullPoint();
            }
        }
        else{
            /*Concave - the farthest intersection is the concave one*/
            if((isInRayPath(ray, intersection2)) && (inter2Length > 0.01)){
                return intersection2;
            }
            else{
                //printVector(ray);
                return nullPoint();
            }
        }

    }

    return nullPoint();
}

/*Find the normal on a point of the lens*/
Vector3D lensNormal(LensData lens, Point3D pointOnLens){
    Vector3D normalVector;

    normalVector.position = pointOnLens;

    if(lens.isConvex == true){
        normalVector.direction.x = (pointOnLens.x - lens.position.x) / lens.radius;
        normalVector.direction.y = (pointOnLens.y - lens.position.y) / lens.radius;
        normalVector.direction.z = (pointOnLens.z - lens.position.z) / lens.radius;
    }
    else{
        normalVector.direction.x = -(pointOnLens.x - lens.position.x) / lens.radius;
        normalVector.direction.y = -(pointOnLens.y - lens.position.y) / lens.radius;
        normalVector.direction.z = -(pointOnLens.z - lens.position.z) / lens.radius;
    }

    return normalVector;
}

/*Makeshift bubble sort for the lens list*/
void sortLensList(){
    int unsortedLenses = globals.numberOfLenses;
    double highestZdistance = -1;
    int farthestLensIndex = 0;
    LensData lensBuffer;

    int i;

    while(unsortedLenses > 0){
        /*Find the unsorted lens farthest from the viewplane, and move it to the end of
         the array of *unsorted* lenses. Repeat until all lenses are sorted.*/
        highestZdistance = -1;
        for(i = 0; i < unsortedLenses; i++){
            if(globals.lenses[i].truePosition.z > highestZdistance){
                highestZdistance = globals.lenses[i].truePosition.z;
                farthestLensIndex = i;
            }
        }

        /*Swap lens positions if the lenses aren't in the right place*/
        if(farthestLensIndex != (unsortedLenses - 1)){
            lensBuffer = globals.lenses[unsortedLenses-1];
            globals.lenses[unsortedLenses-1] = globals.lenses[farthestLensIndex];
            globals.lenses[farthestLensIndex] = lensBuffer;
        }

        unsortedLenses--;
    }

}


/*Sends a ray through all of the lenses, and gives the ray emerging from the other side.
  Warning: this assumes the lenses do not overlap each other, like an actual
  camera lens system. It also assumes the lenses are sorted.*/
Vector3D traceLenses(Vector3D ray, int iterationLevel){
    Vector3D endVector = ray;
    Point3D rayLensIntersection;
    int i;

    for(i = 0; i < globals.numberOfLenses; i++){
        rayLensIntersection = lensIntersection(globals.lenses[i], endVector);

        if(isNullPoint(rayLensIntersection) == false){
            if(globals.lenses[i].isConvex == true){
                endVector = getRefraction(endVector, lensNormal(globals.lenses[i], rayLensIntersection), 1.0, globals.lenses[i].refractionIndex, rayLensIntersection);
            }
            else{
                endVector = getRefraction(endVector, lensNormal(globals.lenses[i], rayLensIntersection), globals.lenses[i].refractionIndex, 1.0, rayLensIntersection);
            }
            /*printf("Lens %d traced: ", i);
            printVector(endVector);*/
        }
        else{
            /*This shouldn't happen, but if it does, the ray is unaffected by the lens*/
            //printf("Lens %d passed\n", i);
        }
    }

    return(endVector);
}
