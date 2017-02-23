#include "raytrace.h"

#define INTEGRAL_SUBDIVISIONS 40
#define ASSUMPTION_BOUND 15

#define RED_WAVELENGTH 0.0000650
#define GREEN_WAVELENGTH 0.0000510
#define BLUE_WAVELENGTH 0.0000475

extern GlobalVars globals;

/*http://www.iue.tuwien.ac.at/phd/minixhofer/node60.html - Equation 66
  Model (Rectangular aperture fourier transform):
    D' = distance from vp to screen
    D = distance from screen to light source
    Primed terms represent viewplane variables
    Bar terms are aperture plane variables*/
void drawStarburstTexture(LightData light, int x, int y){
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
    
    long double xTermNumerator;
    long double yTermNumerator;
    long double xTerm;
    long double yTerm;
    long double sincX;
    long double sincY;
    double rZeroPrime = getLength(globals.viewPlane[1][0], globals.viewPoint);
    
    double xPrime = (double)x;
    double yPrime = (double)y;
    /*xBar and yBar are half of the viewplane width and height respectively*/
    
    xTermNumerator = 2 * M_PI * xPrime * (globals.planeWidth / 2);
    yTermNumerator = 2 * M_PI * yPrime * (globals.planeHeight / 2);
    
    /*Red calculation*/
    xTerm = xTermNumerator / (rZeroPrime * RED_WAVELENGTH);
    xTerm = pow(sincOperation(xTerm), 2);
    
    yTerm = yTermNumerator / (rZeroPrime * RED_WAVELENGTH);
    yTerm = pow(sincOperation(yTerm), 2);
    
    red = light.colour.red * xTerm * yTerm;
    
    
    /*Green Calculation*/
    xTerm = xTermNumerator / (rZeroPrime * GREEN_WAVELENGTH);
    xTerm = pow(sincOperation(xTerm), 2);
    
    yTerm = yTermNumerator / (rZeroPrime * GREEN_WAVELENGTH);
    yTerm = pow(sincOperation(yTerm), 2);
    
    green = light.colour.green * xTerm * yTerm;
    
    
    /*Blue Calculation*/
    xTerm = xTermNumerator / (rZeroPrime * BLUE_WAVELENGTH);
    xTerm = pow(sincOperation(xTerm), 2);
    
    yTerm = yTermNumerator / (rZeroPrime * BLUE_WAVELENGTH);
    yTerm = pow(sincOperation(yTerm), 2);
    
    blue = light.colour.blue * xTerm * yTerm;
    
    alpha = (red * 0.33) + (green * 0.33) + (blue * 0.33);
    
    
    insertOverlayPixel(globals.overlayPixels, START_WIDTH, START_HEIGHT, x, y, red, green, blue, alpha);
}

/*sinc(x) function: calculates sin(x)/x, and returns 1 when x is near 0*/
double sincOperation(long double x){
    long double result;
    
    if((x < 0.001) && (x > -0.001)){
        return(1.0);
    }
    else{
        result = sinl(x);
        result = result / x;
        return((double)result);
    }
}
