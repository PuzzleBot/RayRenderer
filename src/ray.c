#include "raytrace.h"

extern GlobalVars globals;

ColourRGB traceRay(Vector3D ray){
    ShapeData * closestShape = NULL;
    Point3D intersection;
    
    ColourRGB finalColour;
    
    ColourRGB pointColour;
    ColourRGB reflectColour;
    ColourRGB refractColour;
    
    /*Get the first intersected shape*/
    closestShape = getFirstIntersectedShape(ray);
    
    /*If there is an intersection, process the illumination model*/
    if(closestShape == NULL){
        finalColour.red = 0.0;
        finalColour.green = 0.0;
        finalColour.blue = 0.0;
    }
    else{
        intersection = getIntersection(*closestShape, ray);
        
        /*Ambient model*/
        pointColour.red = globals.ambientColour.red * globals.ambientCoefficient * closestShape->colour.red;
        pointColour.green = globals.ambientColour.green * globals.ambientCoefficient * closestShape->colour.green;
        pointColour.blue = globals.ambientColour.blue * globals.ambientCoefficient * closestShape->colour.blue;
        
        finalColour = pointColour;
    }
    
    return(finalColour);
}


ShapeData * getFirstIntersectedShape(Vector3D ray){
    int i;
    
    double closestIntersectionLength = -2;
    ShapeData * closestShape = NULL;
    
    double currentIntersectionLength;
    Point3D currentIntersection;
    
    /*Find the first ray-shape intersection, if it exists*/
    for(i = 0; i < globals.numberOfShapes; i++){
        currentIntersection = getIntersection(globals.shapes[i], ray);
        
        if(isNullIntersection(currentIntersection) == false){
            currentIntersectionLength = getLength(ray.position, currentIntersection);
            
            /*If the newly found intersection is closer than the current closest intersection, or there
             is no current closest intersection, mark the new intersected shape as the closest
             (Dev note: When implementing transparency, keep a structure sorting the shapes
             from closest to farthest instead of doing this)*/
            if((currentIntersectionLength < closestIntersectionLength) || (closestIntersectionLength < -1)){
                closestIntersectionLength = currentIntersectionLength;
                closestShape = &globals.shapes[i];
            }
        }
    }
    
    return(closestShape);
}
