//  Integrators.h
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02
/**
 * Integrators code: This header file defines a suite of functions for numerical integration in C, catering to a variety of methods for approximating
 * definite integrals and solving ordinary differential equations (ODEs), including the Trapezoidal rule, Simpson's rule, Midpoint rule, Romberg's method,
 * Euler's method, Verlet's method, and the Runge-Kutta method. Each function is designed to approximate the integral of a function over a specified range
 * or perform numerical integration of differential equations. It is crucial to note that the accuracy and applicability of these methods vary with the function being integrated and the specific requirements of the problem at hand. For example, Romberg's method assumes the function provided is continuous and differentiable over the
 * integration range. Additionally, users should be aware that the function pointers passed to these integrators must match the expected signature, and care must be
 * taken to ensure numerical stability and convergence, especially for adaptive methods like Romberg's.
 *
 * Important Notes:
 *      - The function pointers passed to these integrators must match the expected signature, and care must be taken to ensure numerical
 *        stability and convergence, especially for adaptive methods like Romberg's.
 *      - The functions assume equally spaced intervals for methods like the Trapezoidal, Simpson's, and Midpoint rules.
 *      - The initial parameters for the integration methods should be carefully chosen to balance accuracy and computational cost, especially for the number of intervals (n).
 *      - Euler, Verlet, and Runge-Kutta methods for numerical integration of ODEs require the computation of derivatives or acceleration to be accurate over the integration range.
 */


#ifndef Integrators_h
#define Integrators_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>




// ------------- Helper Functions for Integrating -------------
/// \{

double* trapezoidal_rule_integration(int n, double x[], double f[]); // Integrates the function f(x) using the Trapezoidal rule, approximates the integral of a function by dividing the area under the curve into trapezoids and summing their areas.
double* simpsons_rule_integration(int n, double x[], double f[]); // Integrates the function f(x) using Simpson's rule, approximates the integration of a function using quadratic polynomials.
double* midpoint_rule_integration(int n, double x[], double f[]); // Integrates the function f(x) using the Midpoint rule, similar to the trapezoidal rule, but it takes the function's value at the midpoint of each interval.




double romberg_core(double (*f)(double), double a, double b, int n); // Helper function for 'rombergs_method_integration, computes the integral using the trapezoidal rule with a progressively increasing number of intervals.
double rombergs_method_integration(double (*f)(double), double a, double b, int n); // Integrates the function f(x) using Romberg's method, an application of Richardson extrapolation to numerical integration, returns the final approximation of the integral, taken from the last row of the Romberg table. NOTE: This implementation assumes that your function f is a pointer to a function that takes a single double argument and returns a double. The integration is performed over the range [x[0], x[n-1]].
																					// dynamically compute the trapezoidal approximations and apply Richardson extrapolation
																					/// \}








// ------------- Helper Functions for Numerical Integration Methods for ODEs -------------
/// \{
/// Function Pointers: The f and a parameters are function pointers. They should point to functions that compute the derivative (for Euler and Runge-Kutta) or acceleration (for Verlet).
double* euler_numerical_integration(int n, double x, double y, double h, double (*f)(double x, double y)); // Performs numerical integration using Euler's method.
double* verlet_numerical_integration(int n, double x, double v, double h, double (*a)(double x, double v)); // Performs numerical integration using Verlet's method.
double* runge_kutta_numerical_integration(int n, double x, double y, double h, double (*f)(double x, double y)); // Performs numerical integration using the Runge-Kutta method.
																												 /// \}






#endif /* Integrators_h */


