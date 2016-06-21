#include "raytrace.h"

extern GlobalVars globals;

ColourRGB traceRay(Vector3D ray){
    ColourRGB finalColour;
    
    ColourRGB pointColour;
    ColourRGB reflectColour;
    ColourRGB refractColour;
    
    finalColour.red = 0.0;
    finalColour.green = 0.0;
    finalColour.blue = 0.0;
    return(finalColour);
}