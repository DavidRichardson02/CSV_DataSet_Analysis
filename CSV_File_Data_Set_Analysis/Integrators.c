//  Integrators.c
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#include "Integrators.h"
#include "GeneralUtilities.h"
#include "CommonDefinitions.h"






/**
 * trapezoidal_rule_integration
 *
 * This function takes in arrays of x-values and their corresponding function values f(x) and
 * computes samples of the function F(x), the integral, using the Trapezoidal rule.
 * The Trapezoidal rule approximates the area under the curve as a trapezoid by dividing the
 * area under the curve into trapezoids and summing their areas, hence the name.
 * It is more accurate than the Riemann sum approximation, but less accurate than Simpson's rule.
 *
 * @param n The number of intervals for the Trapezoidal rule.
 * @param x Array storing the 'x' values of the intervals.
 * @param f Array storing the corresponding function 'f' values at each 'x'.
 *
 * @return Array storing the result of the Trapezoidal rule integration for each interval.
 */
double* trapezoidal_rule_integration(int n, double x[], double f[])
{
	/// Allocate memory for the result array
	double *F = (double*)malloc(n * sizeof(double));
	
	/// Initialize the first element of the result array
	F[0] = 0;
	
	/// Loop over the intervals
	for (int i = 1; i < n; i++)
	{
		/// Calculate the width of the interval
		double dx = x[i] - x[i-1];
		
		/// Apply the Trapezoidal rule: the area under the curve for this interval is approximated as the area of a trapezoid
		/// with base dx and heights f[i] and f[i-1]
		F[i] = F[i-1] + 0.5 * (f[i] + f[i-1]) * dx; // Update the integral value
	}
	
	/// Return the result array
	return F;
}




/**
 * simpsons_rule_integration
 *
 * This function takes in arrays of x-values and their corresponding function values f(x) and
 * computes samples of the function F(x), the integral, using Simpson's rule.
 * Simpson's rule approximates the area under the curve by fitting a second-degree polynomial
 * to the function over each interval, hence providing a more accurate approximation than
 * the Trapezoidal rule and the Midpoint rule.
 *
 * @param n The number of intervals for Simpson's rule.
 * @param x Array storing the 'x' values of the intervals.
 * @param f Array storing the corresponding function 'f' values at each 'x'.
 * @return Array storing the result of Simpson's rule integration for each interval.
 */
double* simpsons_rule_integration(int n, double x[], double f[])
{
	/// Check if the number of intervals is odd, as required by Simpson's rule
	if (n % 2 == 0)
	{
		// Print an error message and return NULL if the number of intervals is not odd
		printf("Simpson's rule requires an odd number of points\n");
		return NULL;
	}
	
	double *F = (double*)malloc(n * sizeof(double));
	F[0] = 0;
	
	/// Loop over the intervals, incrementing by 2 each time because Simpson's rule uses two intervals at a time
	for (int i = 2; i < n; i += 2)
	{
		double dx = x[i] - x[i-2]; // Calculate the width of the interval
		
		/// Apply Simpson's rule: the area under the curve for these two intervals is approximated as the area under
		/// a second-degree polynomial that passes through the points (x[i-2], f[i-2]), (x[i-1], f[i-1]), and (x[i], f[i])
		F[i] = F[i-2] + (dx/6) * (f[i-2] + 4*f[i-1] + f[i]); // Update the integral value
	}
	
	/// Return the result array
	return F;
}




/**
 * midpoint_rule_integration
 *
 * This function takes in arrays of x-values and their corresponding function values f(x) and
 * computes samples of the function F(x), the integral, using the Midpoint rule.
 * The Midpoint rule approximates the area under the curve by taking the average of the function's value
 * at the midpoint of each interval, multiplied by the width of the interval.
 *
 * @param n The number of intervals for the Midpoint rule.
 * @param x Array storing the 'x' values of the intervals.
 * @param f Array storing the corresponding function 'f' values at each 'x'.
 *
 * @return Array storing the result of the Midpoint rule integration for each interval.
 */
double* midpoint_rule_integration(int n, double x[], double f[])
{
	/// Allocate memory for the result array
	double *F = (double*)malloc(n * sizeof(double));
	
	/// Initialize the first element of the result array
	F[0] = 0;
	
	/// Loop over the intervals
	for (int i = 1; i < n; i++)
	{
		
		/// Calculate the width of the interval
		double dx = x[i] - x[i-1];
		
		
		/// Calculate the midpoint of the interval
		double midPoint = (x[i] + x[i-1]) / 2;
		
		
		/// Apply the Midpoint rule: the area under the curve for this interval is approximated
		/// as the function value at the midpoint, multiplied by the width of the interval
		F[i] = F[i-1] + f[(int)midPoint] * dx; // Update the integral value
	}
	
	/// Return the result array
	return F;
}
















/**
 * @brief This code snippet contains two functions that perform numerical integration using Romberg's method.
 * Romberg's method is an application of Richardson extrapolation to numerical integration.
 * The first function, romberg_core, computes the integral using the trapezoidal rule with a progressively increasing number of intervals.
 * The second function, rombergs_method_integration, uses the first function to fill a Romberg table and then applies Richardson extrapolation to the table
 * to compute the final approximation of the integral.
 *
 *
 * @note Romberg's method needs to compute values of the integrand function at dynamically determined points, which requires the
 *       integrand function itself (as a function pointer), not an array of pre-computed values
 */
/**
 * romberg_core
 *
 * This function is a helper for 'rombergs_method_integration'. It computes the integral of a function using the trapezoidal rule.
 * The trapezoidal rule works by approximating the region under the graph of the function as a trapezoid and calculating its area.
 * It is a method of numerical integration, a way to calculate the definite integral of a function when the antiderivative is unknown or difficult to find.
 *
 * @param f The function to integrate.
 * @param a The lower limit of integration.
 * @param b The upper limit of integration.
 * @param n The number of intervals to use in the approximation.
 * @return The approximate value of the integral from a to b of f(x) dx.
 */
double romberg_core(double (*f)(double), double a, double b, int n)
{
	/// The width of each interval.
	double h = b - a;
	
	
	/// The initial sum is the area of the trapezoid with height (f(a) + f(b)) / 2 and base h.
	double sum = 0.5 * h * (f(a) + f(b));
	
	
	// Loop over each interval, adding the area of the trapezoid in that interval to the sum.
	for (int i = 1; i < n; i++)
	{
		// The height of the trapezoid in this interval is f(a + i * h).
		// The base of the trapezoid in this interval is h.
		// So, the area of the trapezoid in this interval is h * f(a + i * h).
		// This area is added to the sum.
		sum += h * f(a + i * h);
	}
	return sum; // The sum is now an approximation of the integral from a to b of f(x) dx.
}




/**
 * rombergs_method_integration
 *
 * This function performs numerical integration using Romberg's method.
 * Romberg's method is an application of Richardson extrapolation to numerical integration.
 * The function takes an array of x values and integrates the function over the range [x[0], x[n-1]].
 * The function first checks if there are at least two points for integration, if not, it returns NULL.
 * Then, it allocates memory for the Romberg table and fills the first column of the table using the helper function 'romberg_core'.
 * After that, it applies Richardson extrapolation to the table.
 * Finally, it extracts the result from the last row of the Romberg table and frees the allocated memory.
 *
 * This implementation assumes that function f is a pointer to a function that takes a single double argument and returns a double.
 *
 * @param n Represents the number of points or the depth of the Romberg integration table.
 * @param a The lower limit of integration.
 * @param b The upper limit of integration.
 * @param f The function to integrate.
 * @return A pointer to the array of results.
 */
double rombergs_method_integration(double (*f)(double), double a, double b, int n)
{
	// Check if there are at least two points for integration
	if (n < 2)
	{
		printf("Romberg's method requires at least two points\n");
		//return NULL;
	}
	
	/// Allocate memory for the Romberg table
	double **R = (double**)malloc(n * sizeof(double*));
	for (int i = 0; i < n; i++)
	{
		R[i] = (double*)malloc((i + 1) * sizeof(double));
	}
	
	/// Fill the first column of the Romberg table
	for (int i = 0; i < n; i++)
	{
		int m = pow(2, i);  // Number of intervals doubles each time
							//R[i][0] = romberg_core(f, x[0], x[n-1], m);
		R[i][0] = romberg_core(f, a, b, m);
	}
	
	
	
	
	/// Apply Richardson extrapolation to the table
	for (int j = 1; j < n; j++)
	{
		for (int i = j; i < n; i++)
		{
			R[i][j] = R[i][j-1] + (R[i][j-1] - R[i-1][j-1]) / (pow(4, j) - 1);
		}
	}
	
	/// Extract the result from the last row of the Romberg table
	//double *result = (double*)malloc(n * sizeof(double));
	//for (int i = 0; i < n; i++)
	//{
	//    result[i] = R[n-1][i];  // The last row of the Romberg table
	//}
	double result = R[n - 1][n - 1];
	
	/// Free the allocated memory
	for (int i = 0; i < n; i++)
	{
		free(R[i]);
	}
	free(R);
	
	return result;
}
































/**
 * euler_numerical_integration
 *
 * This function implements the Euler's method for numerical integration. It takes as input the number of steps, initial values of the independent and dependent variables, the step size, and a function pointer to the ODE (y' = f(x, y)). It returns an array storing the result of the Euler's method for each step.
 *
 * The Euler's method is a first-order numerical procedure for solving ordinary differential equations (ODEs) with a given initial value. It is the most basic explicit method for numerical integration of ordinary differential equations and is the simplest Runge–Kutta method.
 *
 * @param n The number of steps for Euler's method.
 * @param x Initial value of the independent variable.
 * @param y Initial value of the dependent variable.
 * @param h Step size.
 * @param f Function pointer to the ODE (y' = f(x, y)).
 * @return Array storing the result of the Euler's method for each step.
 */
double* euler_numerical_integration(int n, double x, double y, double h, double (*f)(double, double))
{
	/// Allocate memory for the result array
	double *result = (double*)malloc(n * sizeof(double));
	result[0] = y;  // Initialize the first element of the result array with the initial value of the dependent variable
	
	/// Main loop for Euler's method
	for (int i = 1; i < n; i++)
	{
		y += h * f(x, y);  // Update the dependent variable using the Euler's method formula
		x += h;  // Update the independent variable by adding the step size
		result[i] = y;  // Store the updated dependent variable in the result array
	}
	
	return result;  // Return the result array
}






/**
 * verlet_numerical_integration
 *
 * This function implements the Verlet's method for numerical integration. It is particularly useful for systems where the acceleration is a function of position and velocity. The function takes as input the number of steps, initial values of the independent variable and velocity, the step size, and a function pointer to the acceleration function. It returns an array storing the result of the Verlet's method for each step.
 *
 * The Verlet's method is a second-order numerical method for solving ordinary differential equations (ODEs). It is especially well-suited for problems in classical mechanics, such as the motion of planets and moons.
 *
 * @param n The number of steps for Verlet's method.
 * @param x Initial value of the independent variable.
 * @param v Current velocity of the system.
 * @param h Step size.
 * @param a Function pointer to the acceleration (second derivative of the system) as a function of position x and velocity v.
 * @return Array storing the result of the Verlet's method for each step.
 */
double* verlet_numerical_integration(int n, double x, double v, double h, double (*a)(double, double))
{
	/// Allocate memory for the position array
	double *position = (double*)malloc(n * sizeof(double));
	position[0] = x;  // Initialize the first element of the position array with the initial value of the independent variable
	
	/// Calculate the initial position at t = -h
	double old_x = x - v * h;
	
	/// Main loop for Verlet's method
	for (int i = 1; i < n; i++)
	{
		/// Update the position using the Verlet's method formula
		double new_x = 2 * x - old_x + h * h * a(x, v);
		old_x = x;  // Store the current position
		x = new_x;  // Update the current position
		position[i] = x;  // Store the updated position in the position array
	}
	
	return position;  // Return the position array
}






/**
 * runge_kutta_numerical_integration
 *
 * This function performs numerical integration using the Runge-Kutta method. The Runge-Kutta method is a powerful technique for approximating solutions to ordinary differential equations (ODEs).
 * Given an initial value problem and a number of steps, the function calculates the approximate solution at each step and returns an array of these values.
 * The Runge-Kutta method involves calculating four estimates (k1 to k4) of the derivative of y at different points within the current step.
 * These estimates are then combined to calculate the change in y for the current step.
 *
 * The Runge-Kutta method works by using a weighted average of the slopes of the tangent lines at different points to estimate the next point.
 * Then, the derivative function (f) is used to compute the slopes of the tangent lines at each point.
 *
 * @param n The number of steps for the Runge-Kutta method.
 * @param x Initial value of the independent variable.
 * @param y Initial value of the dependent variable.
 * @param h Step size.
 * @param f Function pointer to the ODE (y' = f(x, y)).
 * @return Array storing the result of the Runge-Kutta method for each step.
 */
double* runge_kutta_numerical_integration(int n, double x, double y, double h, double (*f)(double, double))
{
	double *result = (double*)malloc(n * sizeof(double));
	result[0] = y;
	
	/// Loop over each step, calculating the approximate solution at each one using the Runge-Kutta method.
	for (int i = 1; i < n; i++)
	{
		double k1 = h * f(x, y);                        // Estimate of the derivative at the start of the step.
		double k2 = h * f(x + 0.5 * h, y + 0.5 * k1);    // Estimate of the derivative at the midpoint of the step, using k1 to estimate y.
		double k3 = h * f(x + 0.5 * h, y + 0.5 * k2);    // Another estimate of the derivative at the midpoint, using k2 to estimate y.
		double k4 = h * f(x + h, y + k3);                // Estimate of the derivative at the end of the step, using k3 to estimate y.
		
		// Combine the estimates to calculate the change in y for the current step, then add this to the current value of y to get the next value.
		y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		
		
		x += h; // The independent variable is incremented by the step size.
		result[i] = y;  // Store the new value of y in the results array.
	}
	return result;
}







