#include "raytrace.h"

#define AVERAGING_ITERATIONS 1
#define WAVELENGTH_STEP 10
#define ITERATION_PORTION (double)1.0 / (double)AVERAGING_ITERATIONS

extern GlobalVars globals;

/*http://www.iue.tuwien.ac.at/phd/minixhofer/node60.html - Equation 66
Model (Rectangular aperture fourier transform):
D' = distance from vp to screen
D = distance from screen to light source
Primed terms represent viewplane variables
Bar terms are aperture plane variables*/
void computeStarburstTexture(LightData light, int x, int y, int lightPixelX, int lightPixelY){
    GLfloat red = 0.0;
    GLfloat green = 0.0;
    GLfloat blue = 0.0;
    GLfloat alpha = 0.0;

    int i;

    Vector3D vpToLightRay;
    ShapeData * intersectedShape;
    Point3D intersection;

    Point3D viewPlaneCoordinates = getViewPlaneCoordinates(x, y);
    Point3D lightViewPlaneCoordinates = getViewPlaneCoordinates(lightPixelX, lightPixelY);

    double redWavelength = 650;
    double greenWavelength = 510;
    double blueWavelength = 450;

    /*Model variables*/
    long double xTermNumerator;
    long double yTermNumerator;
    long double xTerm;
    long double yTerm;
    double planePointLength;        //r0'

    double xPrime;
    double yPrime;

    double apertureWidth = START_WIDTH;
    double apertureHeight = START_HEIGHT;

    double thetaX;
    double thetaY;

    xPrime = x - lightPixelX;
    yPrime = y - lightPixelY;

    planePointLength = (getLength(viewPlaneCoordinates, globals.viewPoint) * 10);
    thetaX =  sin(atan(xPrime/planePointLength));
    thetaY =  sin(atan(yPrime/planePointLength));

    /*xBar and yBar are half of the viewplane width and height respectively*/

    xTermNumerator = M_PI * (apertureWidth);
    yTermNumerator = M_PI * (apertureHeight);

    for(i = 0; i < AVERAGING_ITERATIONS; i++){

        /*Red calculation*/
        xTerm = (xTermNumerator / (redWavelength)) * thetaX;
        xTerm = pow(sincOperation(xTerm), 2);

        yTerm = (yTermNumerator / (redWavelength)) * thetaY;
        yTerm = pow(sincOperation(yTerm), 2);

        red = red + (light.colour.red * xTerm * yTerm * ITERATION_PORTION);

        if(red < 0.0){
            red = 0;
        }


        /*Green Calculation*/
        xTerm = (xTermNumerator / (greenWavelength)) * thetaX;
        xTerm = pow(sincOperation(xTerm), 2);

        yTerm = (yTermNumerator / (greenWavelength)) * thetaY;
        yTerm = pow(sincOperation(yTerm), 2);

        green = green + (light.colour.green * xTerm * yTerm * ITERATION_PORTION);

        if(green < 0.0){
            green = 0;
        }


        /*Blue Calculation*/
        xTerm = (xTermNumerator / (blueWavelength)) * thetaX;
        xTerm = pow(sincOperation(xTerm), 2);

        yTerm = (yTermNumerator / (blueWavelength)) * thetaY;
        yTerm = pow(sincOperation(yTerm), 2);

        blue = blue + (light.colour.blue * xTerm * yTerm * ITERATION_PORTION);

        if(blue < 0.0){
            blue = 0;
        }

        alpha = alpha + ((red * 0.33333) + (green * 0.33333) + (blue * 0.33333)) * ITERATION_PORTION;
        if(alpha > 0.99){
            alpha = 1;
        }

        redWavelength = redWavelength - WAVELENGTH_STEP;
        greenWavelength = greenWavelength - WAVELENGTH_STEP;
        blueWavelength = blueWavelength - WAVELENGTH_STEP;
    }

    /*if((x == 0 || x > 1022) && (y == 0 || y > 766)){
    printf("Terms: %.7Lf, %.7Lf\n", xTerm, yTerm);
    printf("\tRGBA: %.2f, %.2f, %.2f, %.2f\n", red, green, blue, alpha);
    printf("\tCoordinates: %d, %d\n", x, y);
    }*/

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
