//  main.c
//  CSV_File_Data_Set_Analysis


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "CommonDefinitions.h"
#include "GeneralUtilities.h"
#include "StringUtilities.h"
#include "FileUtilities.h"
#include "AnalysisUtilities.h"
#include "Integrators.h"
#include "StatisticalMethods.h"
#include "PlottingMethods.h"
#include "DebuggingUtilities.h"


// This function encapsulates the entire workflow from reading the file contents, preprocessing and formatting the data, to writing the parsed data into structured files.
void run_data_set(const char* dataSetFilePathName, char **fileContents, int lineCount, const char *delimiter); 




int main(int argc, const char * argv[])
{
	printf("Entering: 'main' function.\n");
	/*-----------   User-Provide Pathname(Hardcoded for now)   -----------*/
	const char *particleDataSetFilePathName = "/Users/98dav/Desktop/Xcode/C-Programs/CSV_File_Data_Set_Analysis/physics_particles.txt"; // Pathname to physics particles data set
	const char *weatherDataSetFilePathName =  "/Users/98dav/Desktop/Xcode/C-Programs/CSV_File_Data_Set_Analysis/weather_measurements.csvv"; // Pathname to weather data set
																																		   
	
	
	
	/*-----------   Capture File Contents in an Array of Strings   -----------*/
	int lineCount = count_file_lines(particleDataSetFilePathName, MAX_NUM_FILE_LINES);
	char **fileContents = read_file_contents(particleDataSetFilePathName, lineCount);
	const char *delimiter = identify_delimiter(fileContents, lineCount);
	
	
	
	/*-----------   Run Data Set   -----------*/
	run_data_set(particleDataSetFilePathName, fileContents, lineCount, delimiter);
	
	
	
	
	
	
	
	
	/// TESTING DATA SET ANALYSIS STRUCTURES
	/*
	 DataSetAnalysis particleDataSet = configure_data_set_analysis(particleDataSetFilePathName);
	 DataSetProperties dataSetProperties = analyze_data_set_properties(particleDataSetFilePathName);
	 //*/
	
	
	
	
	
	
	
	
	/// TESTING UNIT EXTRACTION
	/*
	/*-----------   Begin Preprocessing File Contents to Standardize the Format and Achieve/Maintain Compatibility of the Contents   -----------* /
	int fieldCount = count_data_fields(fileContents[0]);
	char **formattedFileContents = fileContents;
	
	char *headerLine = prune_and_trim_problematic_characters_from_string(fileContents[1], delimiter, fieldCount);
	for(int i = 1; i < lineCount; i++)
	{
		formattedFileContents[i] = prune_and_trim_problematic_characters_from_string(fileContents[i], delimiter, fieldCount);
	}
	
	
	/// Individually preprocess, format, and capture each line of the data set in order to achieve a fully preprocessed/formatted data set
	for(int i = 1; i < lineCount; i++)
	{
		/// Examine each data entry and filter out problematic characters, omit or replace disruptive aspects(such as repeated delimiters, whitespaces,
		/// unexpected date/time formats, etc.), and standardize the arrangement of characters representing information for each data entry
		char *dataEntry =  prune_and_trim_problematic_characters_from_string(fileContents[i], delimiter, fieldCount);
		
		
		/// Pass the now preprocessed 'dataEntry' to the utility function, 'format_data_entry_for_plotting', where it will be formatted to ensure each one of it's
		/// field's is of the correct data type, and correctly handling the case(s) when it they are not(i.e., if the field missing, mismatched data type, etc.), in order to
		/// ensure compatibility with the rest of the program before being captured as an element of the 'formattedFileContents'
		formattedFileContents[i] = format_data_entry_for_plotting(headerLine, dataEntry, fieldCount, delimiter);
	}
	
	
	char **extractedUnits = extract_units_from_fields(formattedFileContents[1], delimiter, fieldCount);
	//char *extractedUnitsString = concatenate_string_array(extractedUnits, fieldCount, delimiter);
	//printf("\n Extracted Units: %s", extractedUnitsString);
	//print_string_array(extractedUnits, fieldCount, "extract_units_from_fields");
	//*/
	
	return 0;
}





/**
 * The function performs the following major steps:
 * 1. Preprocesses the file contents to standardize the format.
 * 2. Formats each data entry for plotting.
 * 3. Writes the formatted data set to a new directory.
 * 4. Parses the entire file to categorize data and writes categorized data into separate files.
 */
void run_data_set(const char* dataSetFilePathName, char **fileContents, int lineCount, const char *delimiter)
{
	/*-----------   Begin Preprocessing File Contents to Standardize the Format and Achieve/Maintain Compatibility of the Contents   -----------*/
	int fieldCount = count_data_fields(fileContents[0]);
	char **formattedFileContents = fileContents;
	
	
	char *headerLine = prune_and_trim_problematic_characters_from_string(fileContents[1], delimiter, fieldCount);
	for(int i = 1; i < lineCount; i++)
	{
		formattedFileContents[i] = prune_and_trim_problematic_characters_from_string(fileContents[i], delimiter, fieldCount);
	}
	
	
	/// Individually preprocess, format, and capture each line of the data set in order to achieve a fully preprocessed/formatted data set
	for(int i = 1; i < lineCount; i++)
	{
		/// Examine each data entry and filter out problematic characters, omit or replace disruptive aspects(such as repeated delimiters, whitespaces,
		/// unexpected date/time formats, etc.), and standardize the arrangement of characters representing information for each data entry
		char *dataEntry =  prune_and_trim_problematic_characters_from_string(fileContents[i], delimiter, fieldCount);
		
		
		/// Pass the now preprocessed 'dataEntry' to the utility function, 'format_data_entry_for_plotting', where it will be formatted to ensure each one of it's
		/// field's is of the correct data type, and correctly handling the case(s) when it they are not(i.e., if the field missing, mismatched data type, etc.), in order to
		/// ensure compatibility with the rest of the program before being captured as an element of the 'formattedFileContents'
		formattedFileContents[i] = format_data_entry_for_plotting(headerLine, dataEntry, fieldCount, delimiter);
	}
	print_file_contents(formattedFileContents, lineCount);
	
	/*-----------   Capture Plottable Data from 'fileContents' and write to directory at the same level as the original file. (too long to explain here)   -----------*/
	write_data_set(formattedFileContents, dataSetFilePathName, delimiter);
	
	
	
	
	

	
	
	
	// ------------- Data Extraction Method 2 -------------
	
	
	/*-----------   Create the Directory for Parsed Data files to Go Inrto  -----------*/
	char *dataSetFileDirectory = find_file_directory_path(dataSetFilePathName);
	char *dataSetFileName = find_name_from_path(dataSetFilePathName);
	char *dataSetFileExtension = identify_file_extension(dataSetFilePathName);
	dataSetFileDirectory = combine_strings(dataSetFileDirectory, dataSetFileName);
	
	const char *parsedDataDirectory = create_directory(dataSetFileDirectory, "_Parsed");
	
	
	const char *dataSetDirectory = combine_strings(dataSetFileDirectory, "");
	dataSetDirectory = combine_strings(dataSetDirectory, dataSetFileExtension);
	
	
	
	/*-----------   Parse, Categorize, and Capture the File Contents Array of Strings With a Generalized Parsing Function   -----------*/
	int parameterCount = 0; // The number of fields defining any given data entry, as read from the header line of the csv data set file
	char*** separatedData = parse_entire_file(formattedFileContents, lineCount, &parameterCount, delimiter);
	//printf("\n\n\n\n\n\n\nrun_data_set parsed  ==============================================================================================\n\n");
	for (int i = 0; i < parameterCount; i++)
	{
		/// Capture the Current Fields's Data Entries and Write them to the Parsed Data Directory
		char **dataSetParameter = separatedData[i];
		// Path to the directory in which the plottable data fields will be located, the full pathnames of the data fields file's will be this string + the actual name of the file
		//char *plottableDataFieldsDirectoryFilePath = combine_char_ptr("/", combine_char_ptr(fileName, "_Plottable_Field"));
		//const char *plottableFieldsPathName = combine_char_ptr(dataDirectory, plottableDataFieldsDirectoryFilePath); // Full path for plottable data fields.
		
		
		/// Create a Parsed Data Fields File to Store the Current Fields's Header and Data Entries
		const char *parsedDataFilePathName = combine_strings(parsedDataDirectory, combine_strings("/", dataSetFileName));
		
		
		char *parameterParsed = allocate_memory_char_ptr(parameterCount);
		sprintf(parameterParsed, "_Parsed_Data_Field_%d.txt", i);
		parsedDataFilePathName = combine_strings(parsedDataFilePathName, parameterParsed);
		
		FILE *parsedDataFile = fopen(parsedDataFilePathName, "w+");
		write_file_contents(parsedDataFilePathName, dataSetParameter);
		fclose(parsedDataFile);
		
		print_file(parsedDataFilePathName);
		
		
		
		free(parsedDataFilePathName);
		free(parameterParsed);
		//printf("\n\n\n\n%s\n", separatedData[i][0]); // Print the key, aka the fields name
		for (int j = 1; j < lineCount; j++)
		{
			//printf(" %s\n", separatedData[i][j]); // Print the values
		}
	}
	//printf("\n\n\n==============================================================================================");
	
	
	
	print_string_array_array(separatedData, parameterCount, lineCount, "separatedData print_string_array_array");
	deallocate_memory_char_ptr_ptr(fileContents, lineCount);
}

