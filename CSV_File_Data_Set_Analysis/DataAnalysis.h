//  DataAnalysis.h
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#ifndef DataAnalysis_h
#define DataAnalysis_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>




/// EARLY TESTING DataSetAnalysis Structure
/**
 * DataSetAnalysis Structure: Represents the properties of the data set that are required for data
 * analysis and operations, but not necessarily important for the actual meaning of data. For example, the
 * delimiter a data set uses has basically nothing to do with the contents of the data or their meaning, but
 * analyzing the data in any meaningful way would be impossible without a way to differentiate between various
 * fields and entries.
 *
 * This structure encapsulates the various attributes of a data set's properties that are relevant for
 * the capturing, formatting, and analysis of data, without which, there would be no way to perform
 * operations on data(NOTE: the properties are what's required, this encapsulation structure is just a convenient/efficient way to group them together).
 *
 * Each member variable is associated with a specific property of data set analysis, where each
 * property facilitates operations on the data, such as reading, writing, extraction of values, parsing, processing,
 * statistical operations, interpreting, etc.
 *
 *
 * Struct for data set analysis properties:
 *      - int lineCount: The number of data entries in the data set.
 *      - int fieldCount: The number of fields per data entry.
 *      - const char *delimiter: The delimiter character used to separate fields in the data set.
 *      - char *dataSetHeader: Header line of the data set.
 *      - char **fieldNameTypePairs: Array of strings storing pairs of field names and their corresponding types.
 *      - const char* dataSetFilePathName: Path to the data set file.
 *      - int *missingDataCount: Count of missing values per field.
 *      - char **dataSetFileContents: Full contents of the data set, blindly read in.
 */
typedef struct
{
	int lineCount;
	int fieldCount;
	
	
	const char *delimiter;
	
	char *dataSetHeader;
	char **fieldNameTypePairs;
	
	// expected field data types
	
	
	const char* dataSetFilePathName;
	
	int *missingDataCount;
	
	
	char **dataSetFileContents;
	
	
	
	// members for the plottable data set
	// member for the MATLAB script to be generated
	
} DataSetAnalysis;






void run_data_set(const char* dataSetFilePathName, char **fileContents, int lineCount, const char *delimiter);






#endif /* DataAnalysis_h */




