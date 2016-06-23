#include "raytrace.h"

extern GlobalVars globals;

ColourRGB traceRay(Vector3D ray){
    int i;
    
    ShapeData * closestShape = NULL;
    Point3D intersection;
    
    ColourRGB finalColour;
    
    ColourRGB pointColour;
    ColourRGB diffuseComponent;
    ColourRGB specularComponent;
    
    Vector3D shapeNormal;          // N
    Vector3D lightToShapeVector;   // L
    Vector3D reflectVector;        // R
    Vector3D shapeToViewVector;    // V
    
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
        
        /*Diffuse and specular model - Required vectors not involving a light*/
        shapeNormal = getNormal(*closestShape, intersection);
        
        shapeToViewVector.position = intersection;
        shapeToViewVector.direction.x = globals.viewPoint.x - intersection.x;
        shapeToViewVector.direction.y = globals.viewPoint.y - intersection.y;
        shapeToViewVector.direction.z = globals.viewPoint.z - intersection.z;
        shapeToViewVector = normalize(shapeToViewVector);
        
        /*Diffuse and specular model - Required vectors involving lights*/
        diffuseComponent.red = 0.0;
        diffuseComponent.green = 0.0;
        diffuseComponent.blue = 0.0;
        
        specularComponent.red = 0.0;
        specularComponent.green = 0.0;
        specularComponent.blue = 0.0;
        
        for(i = 0; i < globals.numberOfLights; i++){
            lightToShapeVector.position = globals.lights[i].position;
            lightToShapeVector.direction.x = intersection.x - globals.lights[i].position.x;
            lightToShapeVector.direction.y = intersection.y - globals.lights[i].position.y;
            lightToShapeVector.direction.z = intersection.z - globals.lights[i].position.z;
            lightToShapeVector = normalize(lightToShapeVector);
            
            reflectVector = getReflection(lightToShapeVector, shapeNormal);
            reflectVector = normalize(reflectVector);
            
            /*Diffuse component*/
            diffuseComponent.red = diffuseComponent.red
            + (globals.diffuseCoefficient
               * closestShape->colour.red
               * dotProduct(shapeNormal, lightToShapeVector));
            
            diffuseComponent.green = diffuseComponent.green
            + (globals.diffuseCoefficient
               * closestShape->colour.green
               * dotProduct(shapeNormal, lightToShapeVector));
            
            diffuseComponent.blue = diffuseComponent.blue
            + (globals.diffuseCoefficient
               * closestShape->colour.blue
               * dotProduct(shapeNormal, lightToShapeVector));
            
            /*Specular component*/
            specularComponent.red = specularComponent.red
            + (globals.specularCoefficient
               * globals.lights[i].colour.red
               * pow(dotProduct(reflectVector, shapeToViewVector), globals.specularFiness));
            
            specularComponent.green = specularComponent.green
            + (globals.specularCoefficient
               * globals.lights[i].colour.green
               * pow(dotProduct(reflectVector, shapeToViewVector), globals.specularFiness));
            
            specularComponent.blue = specularComponent.blue
            + (globals.specularCoefficient
               * globals.lights[i].colour.blue
               * pow(dotProduct(reflectVector, shapeToViewVector), globals.specularFiness));
            
            /*All together*/
            pointColour.red = pointColour.red
            + ((globals.lightAttenuation * globals.lights[i].colour.red)
               * (diffuseComponent.red + specularComponent.red));
            
            pointColour.green = pointColour.green
            + ((globals.lightAttenuation * globals.lights[i].colour.green)
               * (diffuseComponent.green + specularComponent.green));
               
            pointColour.blue = pointColour.blue
            + ((globals.lightAttenuation * globals.lights[i].colour.blue)
               * (diffuseComponent.blue + specularComponent.blue));
               
        }
        
        
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
        
        if(isNullPoint(currentIntersection) == false){
            currentIntersectionLength = getLength(ray.position, currentIntersection);
            
            /*If the newly found intersection is closer than the current closest intersection, or there
             is no current closest intersection, mark the new intersected shape as the closest
             (Dev note: When implementing transparency, keep a structure sorting the shapes
             from closest to farthest instead of doing this)*/
            /*IntersectionLength > 0.01 is there to ignore any intersections of rays starting from
              a shape going to the same shape.*/
            if((currentIntersectionLength > 0.01) && ((currentIntersectionLength < closestIntersectionLength) || (closestIntersectionLength < -1))){
                closestIntersectionLength = currentIntersectionLength;
                closestShape = &globals.shapes[i];
            }
        }
    }
    
    return(closestShape);
}
