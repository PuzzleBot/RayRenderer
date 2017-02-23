#include "raytrace.h"

int main(){
    double besselResult;
    double x;
    int kind = 1;
    
    x = 0.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 2.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 3.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 3.90;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 4.50;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 5.60;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 6.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 7.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 8.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 9.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    x = 11.00;
    besselResult = j1(x);
    printf("Result using x = %.2f: %.2f\n", x, besselResult);
    
    return(0);
}
