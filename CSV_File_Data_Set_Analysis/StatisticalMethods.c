//  StatisticalMethods.c
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#include "StatisticalMethods.h"
#include "CommonDefinitions.h"
#include "GeneralUtilities.h"
#include "StringUtilities.h"
#include "Integrators.h"






/**
 * uniform_samples
 *
 * Generates a uniform distribution of samples between xmin and xmax with a step size.
 * Given a range [xmin, xmax], this function generates 'n' equally spaced points within the range
 * where the spacing between each point is 'step'. This is primarily used for the Gaussian function
 * sampling.
 *
 */
double* get_uniform_samples(int *n, double xmin, double xmax, double step)
{
	*n = (int)((xmax - xmin) / step) + 1;
	double *x = allocate_memory_double_ptr(*n);
	
	for (int i = 0; i < *n; i++)
	{
		x[i] = xmin + i * step;
	}
	return x;
}




/**
 * compute_mean
 *
 * This function calculates the mean value of an array of data.
 * The mean is calculated as the sum of all elements divided by the number of elements.
 */
double compute_mean(double* data, int n)
{
	double sum = 0.0;
	for (int i = 0; i < n; i++)
	{
		sum += data[i];
	}
	return sum / n;
}




/**
 * compute_standard_deviation
 *
 * This function calculates the standard deviation of an array of data.
 * The standard deviation is a measure of the amount of variation or dispersion in a set of values.
 *a measure of how dispersed the data is in relation to the mean
 * The formula for the sample standard deviation is:
 *      s = sqrt((1/n) * Σ(xi - mean)^2)
 */
double compute_standard_deviation(double* data, int n, double mean)
{
	double variance = 0.0;
	
	for (int i = 0; i < n; i++)
	{
		variance += (data[i] - mean) * (data[i] - mean);
	}
	variance/=n;
	return sqrt(variance);
}




/**
 * compute_skewness
 *
 * Pearson's moment coefficient of skewness, which is a measure of the asymmetry of the probability distribution of a real-valued random variable about its mean.
 
 * This function calculates Pearson's moment coefficient of skewness of an array of data.
 * This skewness is a measure of the asymmetry of the probability distribution of a real-valued
 * random variable about its mean.
 * The formula for the sample skewness is:
 *  s =  (n / (n−1)(n−2)) * Σ(xi - mean / std_dev)^3)
 */
double compute_skewness(double *data, int n)
{
	double m = compute_mean(data, n);
	double sd = compute_standard_deviation(data, n, m);
	double sum = 0.0;
	
	for(int i = 0; i < n; i++)
	{
		sum += pow((data[i] - m) / sd, 3);
	}
	
	double skewness = ((double)n / ((n - 1) * (n - 2))) * sum;
	return skewness;
}




/**
 * compute_normal_cdf
 *
 * This function calculates the cumulative distribution function (CDF)
 * for a normal distribution at a given value 'x', with given mean 'mu'
 * and standard deviation 'sigma'.
 */
double compute_normal_cdf(double x, double mu, double sigma)
{
	return 0.5 * (1.0 + erf((x - mu) / (sigma * sqrt(2.0))));
}












/**
 * compute_gaussian
 *
 * Computes the Gaussian function values for given x-values.
 * Gaussian function is given by: f(x) = (2/sqrt(pi)) * exp(-x^2).
 * This function returns an array of Gaussian values for a given array of x-values.
 */
double* compute_gaussian(int n, double x[])
{
	double *f = allocate_memory_double_ptr(n);
	
	for (int i = 0; i < n; i++)
	{
		f[i] = (2.0/sqrt(M_PI)) * exp(-(x[i] * x[i]));
	}
	return f;
}




/**
 * gaussian_riemann_sum_integration
 *
 * Integrates the function f(x) using Riemann approximation.
 * This function takes in arrays of x-values and their corresponding function values f(x) and
 * computes samples of the function F(x), the integral, using a Riemann sum approximation.
 */
double* gaussian_riemann_sum_integration(int n, double x[], double f[])
{
	double *F = allocate_memory_double_ptr(n);
	
	F[0] = 0; // Starting value
	for (int i = 1; i < n; i++)
	{
		double dx = x[i] - x[i-1];
		F[i] = F[i-1] + f[i] * dx; // Rectangle approximation for area
	}
	return F;
}












/**
 * anderson_darling_normality_test
 *
 * Performs the Anderson-Darling test for normality.
 *
 * The test statistic A-squared is a measure of how well the data follow
 * a particular distribution. For the normal distribution, if the A-squared
 * statistic is large, it indicates that the data does not follow a normal distribution.
 *tfr1e
 * The test rejects the hypothesis of normality when the p-value is less than or equal to 0.05.
 * Failing the normality test allows you to state with 95% confidence the data does not fit the normal distribution.
 * Passing the normality test only allows you to state no significant departure from normality was found.
 *
 * The Anderson-Darling test, while having excellent theoretical properties, has a serious flaw when applied to real world data.
 * The Anderson-Darling test is severely affected by ties in the data due to poor precision.
 * When a significant number of ties exist, the Anderson-Darling will frequently reject the data as non-normal,
 * regardless of how well the data fits the normal distribution. A tie is when identical values occur more than once.
 */
double anderson_darling_normality_test(double *data, int n)
{
	// The test requires sorted data
	double *sortedData = allocate_memory_double_ptr(n);
	
	for (int i = 0; i < n; i++)
	{
		sortedData[i] = data[i];
	}
	merge_sort(sortedData, n);
	
	
	double mean = compute_mean(data, n);
	double std_dev = compute_standard_deviation(data, n, mean);
	
	double A_squared = 0.0;
	for (int i = 0; i < n; i++)
	{
		double Fi = compute_normal_cdf(sortedData[i], mean, std_dev);
		A_squared += (2 * (i + 1) - 1) * (log(Fi) + log(1 - Fi));
	}
	A_squared = -n - A_squared * (1.0 / n);
	
	free(sortedData);
	return A_squared;
}












/**
 * compute_IQR
 *
 * Computes the Interquartile Range (IQR) of a dataset.
 * The IQR is a measure of statistical dispersion and is calculated as the difference
 * between the 75th (Q3) and 25th (Q1) percentiles of the dataset. The function sorts
 * the data internally before computing the quartiles. Handles both even and
 * odd-sized datasets. If the computed IQR is zero (indicating all values are the same),
 * the function issues a warning and recalculates the IQR based on the 25th and 75th indices.
 * Assumes that the dataset is large enough to provide meaningful percentile values.
 */
double compute_IQR(double* data, int n)
{
	// Allocate memory for a copy of the data to be sorted
	double *sortedData = allocate_memory_double_ptr(n);
	
	
	// Copy data to the new array for sorting, to preserve the original data order
	for (int i = 0; i < n; i++)
	{
		sortedData[i] = data[i];
	}
	//memcpy(sortedData, data, n);
	
	// Sort the data to prepare for IQR calculation
	merge_sort(sortedData, n); // // Note: Sorting is required for IQR calculation. merge sort is a comparison sort algorithm(good for medium size lists and O(nlogn) in the worst case)
	
	
	//merge_sort(data, n);
	
	// Calculating the Interquartile Range (IQR).
	double iqr = 0.0;
	double q25, q75;
	
	// Calculate the 25th and 75th percentiles based on sorted data
	// If the number of data points is even, calculate the quartiles using the middle two points
	if (n % 2 == 0)
	{
		int mid = n / 2;
		q25 = (sortedData[mid / 2 - 1] + sortedData[mid / 2]) / 2.0;
		q75 = (sortedData[mid + mid / 2 - 1] + sortedData[mid + mid / 2]) / 2.0;
	}
	else  // If the number of data points is odd
	{
		int mid = (n - 1) / 2;
		q25 = sortedData[mid / 2];
		q75 = sortedData[mid + mid / 2];
	}
	
	// Compute the IQR as the difference between the 75th and 25th quartiles
	iqr = q75 - q25;
	
	// Handle special case where IQR is zero, which can skew the bin width calculation
	if (iqr == 0.0)
	{
		perror("\n\nError: Interquartile range is zero. Choose a different binning method, in 'compute_bin_width'.");
		//printf("\nq75: %lf", q75);
		//printf("\nq25: %lf", q25);
		//printf("\n iqr: %lf \n\n\n\n", iqr);
		//free(sortedData);
		
		int q25_index = n / 4;
		int q75_index = 3 * n / 4;
		double q25 = sortedData[q25_index];
		double q75 = sortedData[q75_index];
		return q75 - q25;
	}
	
	// Return the computed IQR
	return iqr;
}










































































/**
 * compute_bin_width
 *
 * Calculates optimal bin width for histogram using the Freedman-Diaconis rule.
 * The Freedman-Diaconis rule provides a robust bin-width calculation based on the
 * Interquartile Range (IQR) of the data set.
 *
 * Formula: binWidth = 2 * (IQR) * n^(-1/3)
 */
double compute_bin_width(double* data, int n)
{
	// Check if the data set is large enough to calculate IQR
	if (n < 2)
	{
		perror("\n\nError: Insufficient data to calculate bin width in 'compute_bin_width'.\n");
		exit(1);
	}
	
	
	// Calculating the Interquartile Range (IQR).
	double iqr = 0.0;
	double q25, q75;
	
	
	double *sortedData = allocate_memory_double_ptr(n);
	
	for (int i = 0; i < n; i++)
	{
		sortedData[i] = data[i];
	}
	memcpy(sortedData, data, n);
	
	merge_sort(sortedData, n); // Sorting is required for IQR calculation. merge sort is a comparison sort algorithm(good for medium size lists and O(nlogn) in the worst case)
	
	
	for (int i = 1; i < n; i++)
	{
		//printf("\n\nsortedData: %.17g", sortedData[i]);
		//printf("\n\ndata: %.17g", (*data)[i]);
		//sortedData[i-1] = (*data)[i-1];
	}
	
	// Calculate the 25th and 75th percentiles based on sorted data
	// Check if data size is even
	if (n % 2 == 0)
	{
		int mid = n / 2;
		q25 = (sortedData[mid / 2 - 1] + sortedData[mid / 2]) / 2.0;
		q75 = (sortedData[mid + mid / 2 - 1] + sortedData[mid + mid / 2]) / 2.0;
	}
	else  // If data size is odd
	{
		int mid = (n - 1) / 2;
		q25 = sortedData[mid / 2];
		q75 = sortedData[mid + mid / 2];
	}
	
	// Compute the Interquartile Range (IQR)
	iqr = q75 - q25;
	
	// Handle cases where IQR is zero (all numbers are the same)
	if (iqr == 0.0)
	{
		perror("\n\nError: Interquartile range is zero. Choose a different binning method, in 'compute_bin_width'.");
		exit(1);
		//printf("\nq75: %lf", q75);
		//printf("\nq25: %lf", q25);
		//printf("\n iqr: %lf \n\n\n\n", iqr);
		
		//free(sortedData);
		//return 0.0;
	}
	
	// Calculate the optimal bin width using the Freedman-Diaconis rule
	double binWidth = 2.0 * iqr / pow((double)(n), 1.0 / 3.0);
	
	
	
	
	//data = allocate_memory_double_ptr_ptr(n);
	//for (int i = 0; i < n; i++)
	//{
	//    (*data)[i] = sortedData[i];
	//printf("\n\nsorted  data ???: %.17g", (*data)[i]);
	//}
	//memcpy(data, sortedData, n);
	free(sortedData);// Clean up allocated memory
	return binWidth;
}




/**
 * calculateOptimalNumBins
 *
 * Calculates the optimal number of bins using a Bayesian approach.
 * This function calculates Bayesian Information Criterion (BIC) based on the bin width
 * to find the optimal number of bins for a histogram.
 *
 * BIC Formula: BIC = n * log(binWidth) + 0.5 * log(n) * numBins
 * The formula is also adjusted for small sample sizes.
 */
int compute_optimal_num_bins(double* data, int n)
{
	double binWidth = compute_bin_width(data, n);
	if (binWidth == 0.0)
	{
		return n;
	}
	
	//merge_sort(data, n);
	
	//for (int i = 1; i < n; i++)
	//{
	//    printf("\n\nsorted  data ??? : %.17g", (*data)[i]);
	//sortedData[i-1] = (*data)[i-1];
	//}
	
	
	// Calculate the range of the data
	//double range = data[n - 1] - data[0]; // The data has already been sorted
	
	
	// Estimate the number of bins based on the range and optimal bin width
	//int numBins = ((int)(range / binWidth));
	
	// Estimate the number of bins based on the range and optimal bin width
	//int numBins = (int)ceil(range / binWidth);
	
	// Calculate the range of the data
	double range = max_element(data, n) - min_element(data, n);
	
	// Estimate the number of bins based on the range and optimal bin width
	int numBins = (int)ceil(range / binWidth);
	
	// Bayesian Information Criterion (BIC) to find the optimal number of bins
	double bic = n * log(binWidth) + 0.5 * log(n) * numBins;
	
	
	// Adjusting for small sample sizes
	//bic += (numBins - 1) * log((double)(n)) / 2.0;
	
	
	
	return numBins;
	//return (int)(bic);
}




/**
 * compute_data_set_binning
 *
 * Computes the optimal binning for a histogram of a dataset.
 * This function determines the number of bins and their width for constructing
 * a histogram, using the Freedman-Diaconis rule which is based on the IQR and
 * the size of the dataset. The function checks for sufficient data size and
 * handles cases with small datasets. Utilizes the IQR and range of the data to
 * calculate bin width and number. Populates the bins based on the calculated binning strategy.
 *
 * @note For very small datasets, the Freedman-Diaconis rule may not be optimal.
 *       The method assumes a reasonably symmetric distribution of data.
 *       Does not consider the Bayesian Information Criterion (BIC) for bin number optimization.
 */
Histogram compute_data_set_binning(double *data, int n)
{
	// Calculate the optimal number of bins
	// Verify that the data set is large enough to calculate IQR
	if (n < 2)
	{
		perror("\n\nError: Insufficient data to calculate bin width in 'compute_bin_width'.\n");
	}
	
	
	// Calculating the Interquartile Range (IQR).
	double iqr = compute_IQR(data, n);
	
	// Calculate bin width using the Freedman-Diaconis rule, which reduces the effect of outliers
	double binWidth = 2.0 * iqr / pow((double)(n), 1.0 / 3.0);
	
	// If binWidth is too small, adjust it to a reasonable minimum value
	if(binWidth < 1)
	{
		binWidth = n;
	}
	
	
	
	//merge_sort(data, n);
	
	// Calculate the range of the data
	double range = 0;
	range = max_element(data, n) - min_element(data, n);
	
	
	
	// Estimate the number of bins based on the range and optimal bin width
	int numBins = 0;
	numBins = ((int)(range / binWidth));
	
	
	// Bayesian Information Criterion (BIC) to find the optimal number of bins
	//double bic = n * log(binWidth) + 0.5 * log(n) * numBins;
	
	
	// Adjusting for small sample sizes
	//bic -= (numBins - 1) * log((double)(n)) / 2.0;
	
	//print_array(n, *data, "Histogram Data");
	
	
	
	// Calculate the optimal number of bins, ensuring it's a positive integer
	int optimalNumBins = numBins > 0 ? numBins : 1;
	
	
	
	
	
	if (optimalNumBins <= 0)
	{
		perror("\n\nError: Unable to calculate the optimal number of bins in 'compute_data_set_binning'.\n");
		//exit(1);
		optimalNumBins = n;
	}
	
	// Calculate the minimum and maximum values in the data set
	double min_value = min_element(data, n);
	double max_value = max_element(data, n);
	//printf("\n\nminValue: %lf, maxValue: %lf\n",  min_value, max_value);
	
	
	
	
	
	// Calculate the width of each bin
	double bin_width = (max_value - min_value) / optimalNumBins;
	
	
	// Initialize bins, holds frequency count
	
	int* dataSetBins = allocate_memory_int_ptr(optimalNumBins); //A pointer to an array of histogram bins is initialized with a size equal to the optimal number of bins.
																//Each bin is initialized to have a frequency count of 0.
	
	
	for(int i=0; i<optimalNumBins;i++)
	{
		dataSetBins[i] = 0;
	}
	
	
	
	
	// Populate the bins with the frequency count of each bin
	for (int i = 0; i < n; i++)
	{
		int bin_index = (int)((data[i] - min_value) / bin_width);
		if (bin_index == optimalNumBins)
		{
			bin_index--;  // Handle the edge case when data[i] == max_value
		}
		dataSetBins[bin_index]++;
	}
	
	
	// Update the data array to hold histogram values for visualization
	
	double *binData = allocate_memory_double_ptr(optimalNumBins);
	// Output bins
	for (int i = 0; i < optimalNumBins; i++)
	{
		double bin_start = min_value + i * bin_width;
		double bin_end = bin_start + bin_width;
		//printf("\nBin %d: [%.17gf, %.17g): %d", i, bin_start, bin_end, dataSetBins[i]);
		
		binData[i] = (bin_start + bin_end) * 0.5;
		//(*data)[i] = (bin_start + bin_end) * 0.5;
	}
	
	//free(*(data));
	//data = allocate_memory_double_ptr_ptr(optimalNumBins);
	//memcpy(data, binData, optimalNumBins);
	
	
	// Return the computed histogram structure
	Histogram data_histogram;
	data_histogram.bins = dataSetBins;
	data_histogram.num_bins = optimalNumBins;
	data_histogram.bin_width = bin_width;
	data_histogram.min_value = min_value;
	data_histogram.max_value = max_value;
	return data_histogram;
}













/**
 * compute_histogram
 *
 * Computes the histogram of the data using optimal bin width and number of bins.
 * This function uses the Bayesian Information Criterion (BIC) to find the optimal number of bins,
 * and then constructs the histogram accordingly.
 */
void compute_histogram(double* data, int n)
{
	// Check if the data set is large enough to calculate IQR
	if (n < 2)
	{
		perror("\n\nError: Insufficient data to calculate bin width in 'compute_bin_width'.\n");
	}
	
	
	// Calculating the Interquartile Range (IQR).
	double iqr = 0.0;
	double q25 = 0, q75 = 0;
	double* sortedData = (double*)malloc(n * sizeof(double));
	
	for (int i = 0; i < n; i++)
	{
		sortedData[i] = data[i];
	}
	
	
	
	merge_sort(sortedData, n); // Sorting is required for IQR calculation. merge sort is a comparison sort algorithm(good for medium size lists and O(nlogn) in the worst case)
	
	
	
	
	// Calculate the 25th and 75th percentiles based on sorted data
	// Check if data size is even
	if (n % 2 == 0)
	{
		int mid = n / 2;
		q25 = (sortedData[mid / 2 - 1] + sortedData[mid / 2]) / 2.0;
		q75 = (sortedData[mid + mid / 2 - 1] + sortedData[mid + mid / 2]) / 2.0;
	}
	else  // If data size is odd
	{
		int mid = (n - 1) / 2;
		q25 = sortedData[mid / 2];
		q75 = sortedData[mid + mid / 2];
	}
	
	// Compute the Interquartile Range (IQR)
	iqr = q75 - q25;
	
	// Handle cases where IQR is zero (all numbers are the same)
	if (iqr == 0.0)
	{
		printf("\n\nError: Interquartile range is zero. Choose a different binning method, in 'compute_bin_width'.");
		printf("\nq75: %lf", q75);
		printf("\nq25: %lf", q25);
		printf("\n iqr: %lf \n\n\n\n", iqr);
		
		free(sortedData);
	}
	
	// Calculate the optimal bin width using the Freedman-Diaconis rule
	double binWidth = 2.0 * iqr / pow((double)(n), 1.0 / 3.0);
	if(binWidth < 1)
	{
		binWidth = n;
	}
	free(sortedData);// Clean up allocated memory
	
	
	
	merge_sort(sortedData, n);
	
	// Calculate the range of the data
	double range = 0;
	range = max_element(data, n) - min_element(data, n);
	
	
	
	// Estimate the number of bins based on the range and optimal bin width
	int numBins = 0;
	numBins = ((int)(range / binWidth));
	
	
	// Bayesian Information Criterion (BIC) to find the optimal number of bins
	//double bic = n * log(binWidth) + 0.5 * log(n) * numBins;
	
	
	// Adjusting for small sample sizes
	//bic -= (numBins - 1) * log((double)(n)) / 2.0;
	
	//print_array(n, *data, "Histogram Data");
	
	
	//printf("\n\nq25: %.17g", q25);
	//printf("\n\nq75: %.17g", q75);
	//printf("\n\nbinWidth: %.17g", binWidth);
	//printf("\n\nrange: %.17g", range);
	//printf("\n\nnumBins: %d", numBins);
	//printf("\n\nbic: %.17g", bic);
	
	
	
	// Calculate the optimal number of bins
	int optimalNumBins = numBins;
	
	
	
	
	
	if (optimalNumBins <= 1)
	{
		perror("\n\nError: Unable to calculate the optimal number of bins in 'compute_histogram'.\n");
		printf("\n\noptimalNumBins: %d", optimalNumBins);
		//exit(1);
		optimalNumBins = n;
	}
	
	// Determine the range of the data
	double min_value = min_element(data, n);
	double max_value = max_element(data, n);
	//printf("\n\nminValue: %lf, maxValue: %lf\n",  min_value, max_value);
	
	
	// Calculate the width of each bin
	double bin_width = (max_value - min_value) / optimalNumBins;
	
	
	// Initialize bins, holds frequency count
	int* histogramBins = allocate_memory_int_ptr(optimalNumBins);  //A pointer to an array of histogram bins is initialized with a size equal to the optimal number of bins.
	
	
	//Each bin is initialized to have a frequency count of 0.
	for(int i=0; i<optimalNumBins;i++)
	{
		histogramBins[i] = 0;
	}
	
	
	
	
	// Populate bins
	for (int i = 0; i < n; i++)
	{
		int bin_index = (int)((data[i] - min_value) / bin_width);
		if (bin_index == optimalNumBins)
		{
			bin_index--;  // Handle the edge case when data[i] == max_value
		}
		histogramBins[bin_index]++;
	}
	
	
	
	
	
	
	// Update the data array to hold histogram values for visualization
	data = (double*)malloc(optimalNumBins * sizeof(double));  // Allocate memory for n pointers to double
	if (data == NULL)
	{
		perror("\n\nError: Failed to allocate memory for data in 'compute_histogram'.");
		exit(1);
	}
	
	
	
	// Output bins
	for (int i = 0; i < optimalNumBins; i++)
	{
		double bin_start = min_value + i * bin_width;
		double bin_end = bin_start + bin_width;
		//printf("\nBin %d: [%lf, %lf): %d", i, bin_start, bin_end, histogramBins[i]);
		
		
		data[i] = (bin_start + bin_end) * 0.5;
	}
}




void print_histogram(Histogram histogram, char *label)
{
	// Print all the details of the data structure
	printf("\n\n\n\n\n\n\n\nprint_histogram %s ==============================================================================================\n", label);
	printf("\n\n num_bins: %d", histogram.num_bins);
	printf("\n bin_width: %.17g", histogram.bin_width);
	printf("\n\n min_value: %.17g", histogram.min_value);
	printf("\n max_value: %.17g", histogram.max_value);
	
	printf("\n\n Histogram Bins: ");
	for(int i = 0; i < histogram.num_bins; i++)
	{
		printf("\n   bin %d: %d", i, histogram.bins[i]);
	}
	
	printf("\n\n\n==============================================================================================");
}

