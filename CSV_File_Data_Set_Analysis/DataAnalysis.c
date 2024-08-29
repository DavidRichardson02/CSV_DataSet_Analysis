//  DataAnalysis.c
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#include "DataAnalysis.h"
#include "GeneralUtilities.h"
#include "CommonDefinitions.h"
#include "StringUtilities.h"
#include "FileUtilities.h"
#include "Integrators.h"
#include "StatisticalMethods.h"
#include "DataSetUtilities.h"
#include "PlottingMethods.h"
#include "DebuggingUtilities.h"





/**
 The same basic steps apply regardless of the strategy of implementation, which even still, is only relevant for data preprocessing
 and extraction/parsing, this is because once the data has been sent to the files the remaining steps are relatively straightforward
 in comparison, and then the actual first step, just blindly capturing the data from the file, is trivial.
 
 The only strategy that is even somewhat tennative to change is the one for how data preprocessing is implemented, the current favorite
 
 
 Even then, the strategy of implementation is only relevant u the data has been captured, preprocessed, and extracted/parsed, then the
 I. CAPTURE DATA SET CONTENTS
 
 
 standardize data preprocessing to ensure compatibility throughout the entire program
 
 II. PREPROCESS DATA SET CONTENTS TO ENSURE COMPATIBILITY THROUGHOUT THE ENTIRE PROGRAM
 1. Capture the contents of the file in an array of strings
 2. Format the string array contents to be compatible with my CSV file operations to help standardize file's contents for further use and prune the array of strings to remove unwelcome things(things like: double commas,, lone - or + signs, etc.), ensure the file lines are organized in the order of header line with names of data fields followed by all data entries on subsequent lines. The header line is commonly a required prerequisite(depends on file contents/data set) and while data can still be plotted without any provided fields of data entries, it will likely lack key information, such as units, name, magnitude, categorization, etc.
 3. Parse the file contents to independently capture all entries for each of the data set's fields, so the name of each field, followed by all of it's data entries, will be captured. For example a data set on particles with the charge, mass, and name will have a main array of strings to capture all contents, which will then be used to create new files to capture the contents  and string/double/int/float/etc. array for each(along with the name of the field, which will be the first element in the field array)
 4. Categorize the data paramaters into plottable and unplottable values(ex: string is unplottable and a double value is plottable) and create appropriately named and located directories(based on og file pathname) for these new and place them into. The primary purpose of this step is to isolate the numerical data fields to allow the application of various analytical methods for plotting things like histograms and theoretical distributions. Additionally, these categorizations will be recorded and indexed to be used later to match things like the title of a graph to the data plotted on it.
 
 
 The entries of the data set will all be defined by the same number of fields(both numerical and character based fields are could possibly be NULL)
 The number of fields defining any given data entry, as read from the header line of the csv data set file
 
 Capture and categorize the contents of .csv data sets, continually refining, streamlining, and formatting data to prepare for plotting. Methodically
 
 
 
 
 II. DATA ANALYSIS
 */



void run_data_set(const char* dataSetFilePathName, char **fileContents, int lineCount, const char *delimiter)
{
	// ------------- Extract Data  -------------
	/*-----------   Capture File Contents in an Array of Strings   -----------*/
	//int lineCount = count_file_lines(dataSetFilePathName, MAX_NUM_FILE_LINES);
	//char **fileContents = read_file_contents(dataSetFilePathName, lineCount);
	
	
	
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
		
		
		
		/// Create a Parsed Data File to Store all the Parsed Data Files, Append File Contents at Each Loop Iteration
		
		
		
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


