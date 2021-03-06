#include "raytrace.h"

extern GlobalVars globals;

ColourRGB traceRay(Vector3D ray, int currentIteration, double currentRefractIndex){
    int i;
    double angle;

    ShapeData * closestShape = NULL;
    Point3D intersection;

    /*Shape-to-light vector - how much light reaches the shape from the source*/
    double passThroughRatio;

    ColourRGB finalColour;

    ColourRGB pointColour;
    ColourRGB diffuseComponent;
    ColourRGB specularComponent;

    Vector3D shapeNormal;          // N
    Vector3D shapeToLightVector;   // L
    Vector3D lightReflectVector;   // R
    Vector3D shapeToViewVector;    // V

    ColourRGB reflectColour;
    Vector3D viewReflectVector;

    ColourRGB refractColour;
    Vector3D viewRefractVector;
    double newRefractionIndex;


    if(currentIteration >= globals.maxTraceIterations){
        /*Limit number of reflections/refractions processed*/
        finalColour.red = 0.0;
        finalColour.green = 0.0;
        finalColour.blue = 0.0;

        return(finalColour);
    }

    /*Trace through the lens system, if real lenses are on*/
    if(globals.realLenses == true){
        ray = traceLenses(ray, 0);
    }

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
            shapeToLightVector.position = intersection;
            shapeToLightVector.direction.x = globals.lights[i].position.x - intersection.x;
            shapeToLightVector.direction.y = globals.lights[i].position.y - intersection.y;
            shapeToLightVector.direction.z = globals.lights[i].position.z - intersection.z;
            shapeToLightVector = normalize(shapeToLightVector);

            /*Shadow check*/

            /*Check for shapes between the reflected ray and the light source, and scale down
              the effect of the light based on the shape's opacity*/
            passThroughRatio = getBlockedLightPassRatio(shapeToLightVector);
            if(passThroughRatio > 0.01){

                lightReflectVector = getReflection(shapeToLightVector, shapeNormal, intersection);
                lightReflectVector = normalize(lightReflectVector);

                /*Diffuse component*/
                diffuseComponent.red = (diffuseComponent.red
                + (globals.diffuseCoefficient
                   * closestShape->colour.red
                   * dotProduct(shapeNormal, shapeToLightVector))) * passThroughRatio;

                diffuseComponent.green = (diffuseComponent.green
                + (globals.diffuseCoefficient
                   * closestShape->colour.green
                   * dotProduct(shapeNormal, shapeToLightVector))) * passThroughRatio;

                diffuseComponent.blue = (diffuseComponent.blue
                + (globals.diffuseCoefficient
                   * closestShape->colour.blue
                   * dotProduct(shapeNormal, shapeToLightVector))) * passThroughRatio;

                /*Specular component*/
                angle = angleBetween(lightReflectVector, shapeToViewVector);
                if((angle >= (M_PI/2)) && (closestShape->reflectivity > 0.01)){
                    specularComponent.red = (specularComponent.red
                    + (globals.specularCoefficient
                       * globals.lights[i].colour.red
                       * pow(dotProduct(lightReflectVector, shapeToViewVector), globals.specularFiness))) * passThroughRatio;

                    specularComponent.green = (specularComponent.green
                    + (globals.specularCoefficient
                       * globals.lights[i].colour.green
                       * pow(dotProduct(lightReflectVector, shapeToViewVector), globals.specularFiness))) * passThroughRatio;

                    specularComponent.blue = (specularComponent.blue
                    + (globals.specularCoefficient
                       * globals.lights[i].colour.blue
                       * pow(dotProduct(lightReflectVector, shapeToViewVector), globals.specularFiness))) * passThroughRatio;
                }

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

            }/*Shadow check end*/
        }/*Light involvement end*/

        finalColour.red = pointColour.red;
        finalColour.green = pointColour.green;
        finalColour.blue = pointColour.blue;

        /*Reflection*/
        if((globals.reflections == true) && (closestShape->reflectivity > 0.001)){
            viewReflectVector = getReflection(ray, shapeNormal, intersection);
            viewReflectVector = normalize(viewReflectVector);
            reflectColour = traceRay(viewReflectVector, currentIteration + 1, currentRefractIndex);

            finalColour.red = finalColour.red + (reflectColour.red * closestShape->reflectivity);
            finalColour.green = finalColour.green + (reflectColour.green * closestShape->reflectivity);
            finalColour.blue = finalColour.blue + (reflectColour.blue  * closestShape->reflectivity);
        }

        /*Transparency / Refraction*/
        if((globals.transparency == true) && (closestShape->opacity < 0.999)){
            /*If a ray passes through an object with a refraction index equal to
              the current refraction index the ray is passing through, that object
              acts as an exit point into air instead (ie. the new refraction index is
              now air's index)*/
            if(fabs(currentRefractIndex - closestShape->refractionIndex) < 0.001){
                newRefractionIndex = AIR_REFRACTION_INDEX;
            }
            else{
                newRefractionIndex = closestShape->refractionIndex;
            }

            viewRefractVector = getRefraction(ray, shapeNormal, currentRefractIndex, newRefractionIndex, intersection);
            viewRefractVector = normalize(viewRefractVector);
            refractColour = traceRay(viewRefractVector, currentIteration + 1, newRefractionIndex);

            finalColour.red = finalColour.red + (refractColour.red * (1.0 - closestShape->opacity));
            finalColour.green = finalColour.green + (refractColour.green * (1.0 - closestShape->opacity));
            finalColour.blue = finalColour.blue + (refractColour.blue * (1.0 - closestShape->opacity));
        }

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
            if(((currentIntersectionLength < closestIntersectionLength) || (closestIntersectionLength < -1)) &&
                (currentIntersectionLength > 0.01)){
                closestIntersectionLength = currentIntersectionLength;
                closestShape = &globals.shapes[i];
            }
        }
    }

    return(closestShape);
}

/*Calculates how much light (as a ratio from 0 to 1) reaches the ray's positition
  after passing through transparent objects.*/
double getBlockedLightPassRatio(Vector3D ray){
    int i;

    Point3D currentIntersection;
    double passRatio = 1.0;

    for(i = 0; i < globals.numberOfShapes; i++){
        currentIntersection = getIntersection(globals.shapes[i], ray);

        /*Determine if each shape is intersected or not. If intersected,
          block out some of the light depending on opacity*/
        if(isNullPoint(currentIntersection) == false){
            if((globals.shapes[i].opacity < 1.0)){
                passRatio = passRatio * (1.0 - globals.shapes[i].opacity);
            }
            else{
                return(0.0);
            }
        }
    }

    if(passRatio < 0.0){
        passRatio = -passRatio;
    }

    return(passRatio);
}
