//  StatisticalMethods.h
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#ifndef StatisticalMethods_h
#define StatisticalMethods_h



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>




// -------------- Histogram Structure Definition --------------
/**
 * Histogram Structure: Represents a histogram with dynamic binning
 *
 * This struct contains all the information that defines a Histogram.
 * Each member variable holds a specific characteristic defining a Histogram, including bin details and range values.
 *
 * - bins: Pointer to an array of integers representing the count in each bin.
 * - num_bins: The total number of bins in the histogram.
 * - bin_width: The width of each bin in the histogram.
 * - min_value: The minimum value in the dataset.
 * - max_value: The maximum value in the dataset.
 */
typedef struct
{
	int *bins;
	int num_bins;
	double bin_width;
	double min_value;
	double max_value;
} Histogram;

void print_histogram(Histogram histogram, char *label);


// ------------- Helper Functions for Generating and Sampling Data Sets  -------------
/// \{
double* get_uniform_samples(int *n, double min, double max, double step);
/// \}






// ------------- Helper Functions For Computing and Handling Data Binning -------------
/// \{
double compute_IQR(double* data, int n);
double compute_bin_width(double *data, int n);
int compute_optimal_num_bins(double *data, int n);
Histogram compute_data_set_binning(double *data, int n);
/// \}






// ------------- Helper Functions to Determine Various Statistical Properties of Data Sets -------------
/// \{
double compute_average(double* data, int n);
double compute_standard_deviation(double* data, int n, double mean);
double compute_skewness(double *data, int n);
double compute_normal_cdf(double x, double mu, double sigma);
/// \}






// ------------- Helper Functions for Computing and Integrating Various Statistical Distributions -------------
/// \{
double* compute_gaussian(int n, double x[]);
double* gaussian_riemann_sum_integration(int n, double x[], double f[]);
/// \}






// ------------- Helper Functions for Performing Statistical Tests to Assess the Normality of Data Sets -------------
/// \{
double anderson_darling_normality_test(double* data, int n);
//one of three general normality tests designed to detect all departures from normality
/// \}






// ------------- Helper Functions to Compute Various Statistical Graphs of Data Sets -------------
/// \{
void compute_histogram(double *data, int n);
/// \}






#endif /* StatisticalMethods_h */
