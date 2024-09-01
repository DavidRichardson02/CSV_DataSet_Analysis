//  AnalysisUtilities.h
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02
/**
 * AnalysisUtilities code: The AnalysisUtilities file provides a set of functions for processing, analyzing, and formatting data sets
 * stored in CSV format, primarily for plotting purposes. The functions are designed to facilitate data visualization and plotting by
 * preparing datasets in a format suitable for study via statistical methods and later for graphical representation. It emphasizes data
 * preprocessing to ensure compatibility with plotting tools and data analysis applications.
 *
 * It includes functionalities for parsing CSV files, counting missing values, determining data entry types, and extracting plottable fields.
 * The file also handles formatting data entries for plotting, ensuring correct data types and handling missing values. Additionally, it offers
 * functions for creating and populating formatted files from data sets, separating plottable and non-plottable data for further analysis.
 *
 * NOTE:
 * The functions in this file are primarily designed for datasets where the first line is the header, and the following lines contain data entries.
 * The file assumes CSV format, but the delimiter can be specified to accommodate different formats.
 *
 *
 * Key Features:
 *       - Parsing CSV files into structured arrays for easy manipulation.
 *       - Counting missing values to assess data quality and completeness.
 *       - Determining data types for fields to ensure correct processing and plotting.
 *       - Identifying and extracting plottable (numeric) fields for visualization.
 *       - Formatting data entries and entire datasets for consistent and accurate plotting.
 *       - Writing processed and plottable data to separate files for further analysis and visualization.
 */


#ifndef AnalysisUtilities_h
#define AnalysisUtilities_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>




// ------------- Helper Functions for Capturing and Formatting Data from an Existing File/Data Set -------------
/// \{
char*** parse_entire_file(char **fileContents, int lineCount, int* fieldCount, const char *delimiter);
int* count_missing_values(char** fileContents, int lineCount, int fieldCount, const char *delimiter, const char *headerLine);
char** capture_data_set_header_for_plotting(char* headerContents, char** fileContents, const char *delimiter); // Captures and formats header information for plotting purposes.
char** determine_data_entry_types(const char *dataEntry, int fieldCount, const char *delimiter); // Determines the data types of fields in a data entry.
char** determine_common_data_entry_types(const char **dataEntries, int entryCount, int fieldCount, const char *delimiter); // Determines the data types of entries in a dataset.


int* identify_plottable_fields(char** fieldNameTypePairs, int fieldCount, const char *typeDelimiter); // Identifies the plottable fields in the dataset and captures their indexes with a value of 0 if the field is not plottable, and a value of 1 if the field is plottable.
char** capture_plottable_fields(char *headerLine, char* dataEntry, int fieldCount, const char *delimiter); // Captures fields from a data entry suitable for plotting.
char *format_data_entry_for_plotting(char *headerLine, char* dataEntry, int fieldCount, const char *delimiter); // Formats a data entry for plotting, ensuring correct data type for each field.
char** capture_data_set_for_plotting(char** fileContents, int lineCount, const char *delimiter); // Processes a dataset for plotting by extracting and formatting numeric fields.
/// \}






// ------------- Helper Functions for Extracting Plottable Data Fields -------------
/// \{
double *extract_plottable_data_field(char** dataSetContents, int fieldIndex, int fieldCount, const char *delimiter); // Writes the plottable data extracted from the dataset to files.
double **extract_plottable_data(char** dataSetContents, int fieldCount, const char *dataDirectory, const char *delimiter); // Extracts all plottable data fields from the dataset and writes them into separate files.
																														   /// \}






// ------------- Helper Functions for Creating and Populating a Formatted File from a Data Set -------------
/// \{
const char *write_plottable_data(char** dataSetContents, char *headerLine, const char *filePathName, const char *dataDirectory, const char *delimiter); // Writes the plottable data extracted from the dataset to files.
char *write_data_set(char** fileContents, const char *filePathName, const char *delimiter); // Processes and writes a dataset to files, separating plottable and non-plottable data.
																							/// \}



















/// NOTE: THE 'DataSetProperties Structure' BELOW IS STRICTLY IN TESTING PHASE, NOT RELEVANT FOR CURRENT VERSION


/**
 * DataSetProperties Structure: Encapsulates the key static properties of a data set for analysis and processing.
 *
 * The DataSetProperties structure provides a convenient way to store and pass around the key properties of a data set
 * once they have already been rigorously determined and compatibility with plotting tools has been ensured.
 *
 * Struct for data set properties members:
 *      - int entryCount: The number of data entries in the data set.
 *      - int fieldCount: The number of fields per data entry.
 *      - const char *delimiter: The delimiter character used to separate fields in the data set.
 *      - char *dataSetHeader: Header line of the data set.
 *      - char **fieldNameTypePairs: Array of strings storing pairs of field names and their corresponding types.
 *      - const char* dataSetFilePathName: Path to the data set file.
 */
typedef struct
{
	int entryCount;
	int fieldCount;
	
	
	const char *delimiter;
	char *header;
	char **fieldNameTypePairs;
	const char* filePathName;
} DataSetProperties;
DataSetProperties analyze_data_set_properties(const char *filePathName);

#endif /* AnalysisUtilities_h */
