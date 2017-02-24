#include "raytrace.h"

#define INTEGRAL_SUBDIVISIONS 40
#define ASSUMPTION_BOUND 15

#define RED_WAVELENGTH 650
#define GREEN_WAVELENGTH 510
#define BLUE_WAVELENGTH 475

extern GlobalVars globals;

/*http://www.iue.tuwien.ac.at/phd/minixhofer/node60.html - Equation 66
  Model (Rectangular aperture fourier transform):
    D' = distance from vp to screen
    D = distance from screen to light source
    Primed terms represent viewplane variables
    Bar terms are aperture plane variables*/
void computeStarburstTexture(LightData light, int x, int y, int lightPixelX, int lightPixelY){
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
    
    Vector3D vpToLightRay;
    ShapeData * intersectedShape;
    Point3D intersection;
    
    /*Model variables*/
    long double xTermNumerator;
    long double yTermNumerator;
    long double xTerm;
    long double yTerm;
    long double sincX;
    long double sincY;
    double rZeroPrime = getLength(getViewPlaneCoordinates(x, y), globals.viewPoint);
    
    double xPrime;
    double yPrime;
    
    xPrime = (double)x - lightPixelX;
    yPrime = (double)y - lightPixelY;
    /*xBar and yBar are half of the viewplane width and height respectively*/
    
    xTermNumerator = 2 * M_PI * xPrime * (globals.planeWidth / 2);
    yTermNumerator = 2 * M_PI * yPrime * (globals.planeHeight / 2);
    
    /*Red calculation*/
    xTerm = xTermNumerator / (rZeroPrime * RED_WAVELENGTH);
    xTerm = pow(sincOperation(xTerm), 2);
    
    yTerm = yTermNumerator / (rZeroPrime * RED_WAVELENGTH);
    yTerm = pow(sincOperation(yTerm), 2);
    
    red = light.colour.red * xTerm * yTerm;
    
    if(red < 0.0){
        red = 0;
    }
    
    
    /*Green Calculation*/
    xTerm = xTermNumerator / (rZeroPrime * GREEN_WAVELENGTH);
    xTerm = pow(sincOperation(xTerm), 2);
    
    yTerm = yTermNumerator / (rZeroPrime * GREEN_WAVELENGTH);
    yTerm = pow(sincOperation(yTerm), 2);
    
    green = light.colour.green * xTerm * yTerm;
    
    if(green < 0.0){
        green = 0;
    }
    
    
    /*Blue Calculation*/
    xTerm = xTermNumerator / (rZeroPrime * BLUE_WAVELENGTH);
    xTerm = pow(sincOperation(xTerm), 2);
    
    yTerm = yTermNumerator / (rZeroPrime * BLUE_WAVELENGTH);
    yTerm = pow(sincOperation(yTerm), 2);
    
    blue = light.colour.blue * xTerm * yTerm;
    
    if(blue < 0.0){
        blue = 0;
    }
    
    
    alpha = (red * 0.33) + (green * 0.33) + (blue * 0.33);
    
    if((x == 0 || x > 1022) && (y == 0 || y > 766)){
        printf("Terms: %.7Lf, %.7Lf\n", xTerm, yTerm);
        printf("\tRGBA: %.2f, %.2f, %.2f, %.2f\n", red, green, blue, alpha);
        printf("\tCoordinates: %d, %d\n", x, y);
    }
    
    
    insertOverlayPixel(globals.starburstTexturePixels, START_WIDTH, START_HEIGHT, x, y, red, green, blue, alpha);
}

/*sinc(x) function: calculates sin(x)/x, and returns 1 when x is near 0*/
double sincOperation(long double x){
    long double result;
    
    if((x < 0.0001) && (x > -0.0001)){
        return(1.0);
    }
    else{
        result = sinl(x);
        result = result / x;
        return((double)result);
    }
}
