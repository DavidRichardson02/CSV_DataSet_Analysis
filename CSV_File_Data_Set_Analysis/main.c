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
#include "DataSetUtilities.h"
#include "Integrators.h"
#include "StatisticalMethods.h"
#include "PlottingMethods.h"
#include "DataAnalysis.h"
#include "DebuggingUtilities.h"






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


