//  DataSetUtilities.c
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#include "DataSetUtilities.h"
#include "CommonDefinitions.h"
#include "GeneralUtilities.h"
#include "StringUtilities.h"
#include "FileUtilities.h"
#include "DebuggingUtilities.h"
#include <ctype.h>






DataSetProperties analyze_data_set_properties(const char *filePathName)
{
	int lineCount = count_file_lines(filePathName, MAX_NUM_FILE_LINES);
	char **fileContents = read_file_contents(filePathName, lineCount);
	const char *delimiter = identify_delimiter(fileContents, lineCount);
	
	
	int fieldCount = count_data_fields(fileContents[0]);
	
	
	
	
	DataSetProperties dataSetProperties;
	dataSetProperties.entryCount = lineCount;
	dataSetProperties.fieldCount = fieldCount;
	dataSetProperties.delimiter = delimiter;
	dataSetProperties.header = fileContents[0];
	dataSetProperties.fieldNameTypePairs =  capture_data_set_header_for_plotting(fileContents[0], fileContents, delimiter);
	dataSetProperties.filePathName = filePathName;
	
	
	printf("\n\n\n\ndataSetProperties: ");
	printf("\n\ndataSetHeader: %s", dataSetProperties.header);
	printf("\ndelimiter: %s", dataSetProperties.delimiter);
	printf("\nentryCount: %d        fieldCount: %d      ", dataSetProperties.entryCount, dataSetProperties.fieldCount);
	
	print_string_array(dataSetProperties.fieldNameTypePairs, dataSetProperties.fieldCount, "fieldNameTypePairs");
	
	for (int i = 0; i < dataSetProperties.entryCount; i++)
	{
		//printf("\n%s", dataSetProperties.fieldNameTypePairs[i]);
		//printf("\n%s", dataSetProperties.fieldNameTypePairs[i]);
		
	}
	
	return dataSetProperties;
}






/**
 * parse_entire_file
 *
 * Function to parse the entire contents of a CSV file.
 * It categorizes and sorts all the values of each one of the dataset's fields and stores them in their own individual char** as a list of values.
 * The function takes in the file contents, line count, field count, and delimiter as parameters. It returns an array of arrays of strings,
 * where each of the ith subarrays of the highest level array is a representation of the ith data set fields and all the entries within it.
 *
 * @param fileContents Constant character pointer to the string containing the data.
 * @param lineCount Constant character pointer to the string containing the  data.
 * @param fieldCount Pointer to the integer value representing the number of paramaters a data set has.
 * @return An array of arrays of strings, where each of the ith subarrays of the highest level array is a representation of the ith data set fields and all the entries within it, while the
 */
char*** parse_entire_file(char **fileContents, int lineCount, int* fieldCount, const char *delimiter)
{
	// Capture the header of the data set for plotting
	char **fileHeader = capture_data_set_header_for_plotting(fileContents[0], fileContents, delimiter);
	int fieldCountCopy = count_data_fields(fileContents[0]);
	
	
	
	/// Capture the Data Field Names in a Single String Without the typeDelimiters
	const char *fieldNames[fieldCountCopy]; //The string of field names as they appear in the first element of the fileContents array of strings
	for (int i = 0; i < fieldCountCopy; i++)
	{
		const char *typeDelimiter = identify_delimiter(fileHeader, fieldCountCopy);
		
		// Split the token by the type delimiter to get the name
		char* typeDelimiterToken = strtok(fileHeader[i], typeDelimiter);
		if (typeDelimiterToken)
		{
			// Duplicate the token to the field names
			fieldNames[i] = strdup(typeDelimiterToken);
		}
	}
	
	
	// Allocate array of string arrays for each member
	char*** separatedData = malloc(fieldCountCopy * sizeof(char**));
	
	// Initialize arrays for each member
	for (int i = 0; i < fieldCountCopy; i++)
	{
		// Allocate memory for each member
		separatedData[i] = malloc((lineCount) * sizeof(char*)); // No need for +1, skipping header
		if (!separatedData[i])
		{
			// Free allocated memory in case of failure
			while (i--)
			{
				free(separatedData[i]);
			}
			free(separatedData);
			*fieldCount = 0;
			return NULL;
		}
		
		
		// Copy the key name
		separatedData[i][0] = strdup(fieldNames[i]);
		// Free fieldNames elements
		free((void*)fieldNames[i]);
	}
	
	
	// Parse the data
	for (int line = 1; line < lineCount; line++) // Start from line 1 to skip header
	{
		// Duplicate the data
		char* dataCopy = strdup(fileContents[line]);
		
		// Tokenize the data
		char* token = strtok(dataCopy, delimiter);
		for (int i = 0; i < fieldCountCopy && token; i++)
		{
			// Store the token in the separated data
			separatedData[i][line] = strdup(token);
			
			// Get the next token
			token = strtok(NULL, delimiter);
		}
		// Free the duplicated data
		free(dataCopy);
	}
	
	// Update the field count
	*fieldCount = fieldCountCopy;
	return separatedData;
}




/**
 * count_missing_values
 *
 * Counts the number of missing or incorrectly formatted values per field in the dataset.
 * Determines the data types of each field based on the second line of the file contents.
 * It iterates over each line in the dataset, tokenizes the line based on the delimiter, and checks each token.
 * If a token is a hyphen and the corresponding field is nonnumeric, it is considered a missing value and the count for that field is incremented.
 * If the determined data type of a token does not match the expected data type of the corresponding field, it is considered incorrectly formatted and the count for that field is incremented.
 * After all lines have been processed, it frees the memory allocated for the data types and returns the count of missing or incorrectly formatted values for each field.
 *
 *
 * @param fileContents Array of strings where each string is a line in the dataset.
 * @param lineCount Number of lines in the dataset.
 * @param fieldCount Number of fields in each line.
 * @param delimiter Delimiter used in the dataset.
 * @param headerLine Header line of the dataset to determine the data types of fields.
 * @return An array of integers where each integer represents the count of missing or incorrectly formatted values for a corresponding field.
 */
int* count_missing_values(char** fileContents, int lineCount, int fieldCount, const char *delimiter, const char *headerLine)
{
	if (!fileContents || lineCount <= 0 || fieldCount <= 0)
	{
		return NULL;
	}
	
	int *missingDataCount = calloc(fieldCount, sizeof(int)); // Initialize counts to zero
	if (!missingDataCount)
	{
		perror("Memory allocation failed for missingDataCount");
		return NULL;
	}
	
	// Analyze the data types based on the header line (assuming first line is the header)
	char **dataTypes = determine_data_entry_types(fileContents[1], fieldCount, delimiter);
	
	/// Iterate over each line in the dataset, tokenizing the line based on the delimiter, and checking each token.
	for (int i = 1; i < lineCount; i++) // Start from 1 to skip the header
	{
		char *dataCopy = strdup(fileContents[i]);
		char *token = strtok(dataCopy, delimiter);
		int fieldIndex = 0;
		
		
		/// Check each token
		while (token && fieldIndex < fieldCount)
		{
			while (isspace((unsigned char)*token)) token++; // Trim leading spaces from the token
			
			const char *typeDataEntry = determine_string_representation_type(token);
			
			
			
			/// Here the delimited value is nothing but a hyphen "-". This field is assumed to be a missing value(either N/A or not available)
			/// Furthermore, to ensure the value is not being used as a minus sign, a check is made to ensure the token is nonnumeric
			if (strcmp(token, "-") == 0 && strcmp(dataTypes[fieldIndex], "nonnumeric") == 0 )
			{
				missingDataCount[fieldIndex]++;  // This field is assumed to be a missing value(either N/A or not available)
			}
			else if (strcmp(typeDataEntry, dataTypes[fieldIndex]) != 0) //The determined data type does not match the expected data type
			{
				missingDataCount[fieldIndex]++;
			}
			
			
			token = strtok(NULL, delimiter);
			fieldIndex++;
		}
		
		free(dataCopy);
	}
	
	for (int i = 0; i < fieldCount; i++)
	{
		free(dataTypes[i]);
	}
	free(dataTypes);
	
	return missingDataCount; // Return the count of missing or incorrectly formatted values for each field
}




/**
 * capture_data_set_header_for_plotting
 *
 * Processes the header line of a dataset to pair each field with its corresponding data type
 * determined from the dataset's contents. Captures and formats the header information for
 * preparing data for plotting by knowing the type of each field.
 *
 *
 * @param headerContents A string containing the header line of the dataset.
 * @param fileContents An array of strings, each representing a line in the dataset.
 * @param delimiter A character used as a delimiter in the dataset.
 * @return An array of strings where each string is a field name from the header paired with its data type.
 */
char** capture_data_set_header_for_plotting(char* headerContents, char** fileContents, const char *delimiter)
{
	// Counts the number of fields in the header.
	int fieldCount = count_data_fields(headerContents);
	
	// Determines the data types of each field based on the second line of file contents.
	char **dataTypes = determine_data_entry_types(fileContents[1], fieldCount, delimiter);
	
	// Calculates total length needed for formatted data (field name + data type).
	int formatDataLength = 0;
	for (int i = 0; i < fieldCount; i++)
	{
		formatDataLength += strlen(dataTypes[i]) + 1;
	}
	
	
	// Allocates memory for storing formatted field names and types, indexing each field to capture all plotting relevant fields from the contents file
	char **fieldIndexing = allocate_memory_char_ptr_ptr(formatDataLength, fieldCount);
	
	
	
	// Duplicates the header string for tokenization.
	char* headerCopy = strdup(headerContents);
	
	
	//int primaryDelimiterCount = 1;
	//char **fileHeader = capture_data_set_header_for_plotting(headerCopy,  fileContents, delimiter);  // Capture header information for plotting.
	
	//const char *typeDelimiter = identify_delimiter_recursive(fileHeader, fieldCount, &delimiter, &primaryDelimiterCount);
	//printf("\n\ntypeDelimiter: %s\n", typeDelimiter);
	
	// Tokenize the header and pair each field with its data type.
	char* token = strtok(headerCopy, delimiter);
	int formatIndex = 0;
	while(token && formatIndex < fieldCount)
	{
		strcpy(fieldIndexing[formatIndex], token); // Copy the token into fieldIndexing
		strcat(fieldIndexing[formatIndex], ":");   // Append the type delimiter.
		strcat(fieldIndexing[formatIndex], dataTypes[formatIndex]); // Append the data type.
		
		
		// Get the next token
		formatIndex++;
		token = strtok(NULL, delimiter);
	}
	free(headerCopy);
	
	
	
	print_string_array(fieldIndexing, formatIndex, "capture_data_set_header_for_plotting");
	
	return fieldIndexing; // Returns formatted field names with data types.
}






/**
 * determine_data_entry_types
 *
 * Determines the data types of fields in a data entry.
 * Analyzes a line from the dataset to infer the data type of each field.
 * It helps in understanding the kind of data each field in the dataset holds.
 *
 * @param dataEntry A string containing a line from the dataset.
 * @param fieldCount The number of fields in the data entry.
 * @param delimiter The character used as a delimiter in the dataset.
 * @return An array of strings where each string represents the inferred data type of a field.
 */
char** determine_data_entry_types(const char *dataEntry, int fieldCount, const char *delimiter)
{
	// Increase the buffer size to accommodate additional characters
	size_t characterCount = (strlen(dataEntry) + 1) * 3;
	
	// Allocate memory for storing data types of each field.
	char** dataFields = allocate_memory_char_ptr_ptr(characterCount, fieldCount);
	
	// Initialize the dataFields array with empty strings.
	for(int i = 0; i < fieldCount; i++)
	{
		strcpy(dataFields[i], "\0");
	}
	
	
	int fieldIndex = 0;
	char* dataCopy = strdup(dataEntry); // Create a copy of dataEntry to use with strtok (as it modifies the string)
	char* token = strtok(dataCopy, delimiter); // Tokenize and analyze each field
	while(token && fieldIndex < fieldCount)
	{
		// Check if the token is a double to determine the type of the field
		const char* type = determine_string_representation_type(token);
		strncat(dataFields[fieldIndex], type, characterCount - strlen(dataFields[fieldIndex]) - 1);
		
		// Get the next token
		fieldIndex++;
		token = strtok(NULL, delimiter);
		
	}
	
	
	free(dataCopy);
	
	//print_string_array(dataFields, fieldCount, "determine_data_entry_types");
	
	return dataFields;
}




/**
 * determine_common_data_entry_types
 *
 * Determines the most common data types (numeric or non-numeric) for each field across all data entries,
 * providing a comprehensive analysis of the entire dataset to establish an expected data type for each field.
 * It achieves this by examining the value found at a specified field, for each one of the data entries,
 * and establishing an expected data type for this field based on the most-common(highest occurences) data type found at said field.
 * (numeric or nonnumeric ==> plottable or unplottable)
 *
 * This function is essentially a more rigorous way of determining the data types of fields, one that takes into account
 * the value(s) found at a specified field across the entire dataset, as opposed to the 'determine_data_entry_types' function,
 * which considers only a single data entry(namely the value found at the entry's specified field).
 * The resulting data typing of fields can be used to perform conditional checks on the
 * data, where they are treated as the expected typing and compared to the data typing
 * found by examining the current data entry in isolation, meaning the expected typing
 * can be effectively used as insurance for cases of disruptive data entries, which are any data entries with missing,
 * blank, and/or unexpected typing of fields, where the examination of a single entry for a field would not be representative
 * of the typing of the field if examined through a broader-scope analysis of the entire dataset(this function).
 *
 * The functionality to perform conditional checks on the data found at a specified field by cross-referencing the expected
 * data typing with the data typing found by independently examining a data entry massively aids in the preprocessing phase of
 * data analysis, which is heavily dependent on both the accuracy of the expected data typings established in this function as
 * well as the appropriate grouping and formatting of types of data to distincly separate the plottable data from the unplottable data.
 *
 *
 * @param dataEntries An array of strings where each string is a data entry.
 * @param entryCount The number of data entries.
 * @param fieldCount The number of fields in each data entry.
 * @param delimiter The character used as a delimiter in the dataset.
 * @return An array of strings where each string represents the most common data type of a field.
 */
char** determine_common_data_entry_types(const char **dataEntries, int entryCount, int fieldCount, const char *delimiter)
{
	// Allocate memory for storing data types of each field.
	char*** dataTypesPerField = (char***)malloc(fieldCount * sizeof(char**));
	
	// Loop through each data entry.
	for(int i = 0; i < entryCount; i++)
	{
		char* dataCopy = strdup(dataEntries[i]); // Create a copy to use with strtok (as it modifies the string)
		char* token = strtok(dataCopy, delimiter);
		int fieldIndex = 0;
		
		// Tokenize and analyze each field in the current data entry.
		while(token && fieldIndex < fieldCount)
		{
			const char* type = determine_string_representation_type(token);
			dataTypesPerField[fieldIndex] = (char**)realloc(dataTypesPerField[fieldIndex], (i+1) * sizeof(char*));
			dataTypesPerField[fieldIndex][i] = strdup(type);
			fieldIndex++;
			token = strtok(NULL, delimiter);
		}
		
		free(dataCopy);
	}
	
	// Prepare the output array.
	char** commonDataTypes = (char**)malloc(fieldCount * sizeof(char*));
	
	// Determine the most common data type for each field.
	for(int i = 0; i < fieldCount; i++)
	{
		commonDataTypes[i] = determine_most_common_string(dataTypesPerField[i], entryCount);
	}
	
	
	// Free the temporary storage.
	for(int i = 0; i < fieldCount; i++)
	{
		for(int j = 0; j < entryCount; j++)
		{
			free(dataTypesPerField[i][j]);
		}
		free(dataTypesPerField[i]);
	}
	free(dataTypesPerField);
	
	
	
	print_string_array(commonDataTypes, fieldCount, "determine_common_data_entry_types");
	
	
	return commonDataTypes; // Return the array of most common data types.
}








/**
 * format_data_entry_for_plotting
 *
 * Formats a data entry for plotting by ensuring each field is of the correct data type.
 * Primarily used for preparing data lines for plotting where data types need to be consistent.
 * The function processes a line from the dataset, converting fields to a default value if they don't match
 * the expected data type (i.e., replacing non-numeric values with '0.0' in numeric fields).
 *
 * @param headerLine The header line of the dataset.
 * @param dataEntry A line from the dataset.
 * @param fieldCount Number of fields in the data entry.
 * @param delimiter The delimiter used in the dataset.
 * @return A new string with the formatted data entry.
 */
char *format_data_entry_for_plotting(char *headerLine, char* dataEntry, int fieldCount, const char *delimiter)
{
	if (!dataEntry)
	{
		perror("\n\nError: NULL argument provided for dataEntry in 'format_data_entry_for_plotting'.");
		return NULL;
	}
	
	
	
	// Create a buffer for the formatted data entry, making it larger to accommodate potential changes
	size_t bufferSize = strlen(dataEntry) + fieldCount * 4; // Extra space for "0.0" and commas
	char* formattedDataEntry = allocate_memory_char_ptr(bufferSize);
	
	strcpy(&formattedDataEntry[0], "\0"); // Initialize the string empty
	char* dataCopy = strdup(dataEntry);
	char* token;
	char* saveptr = dataCopy;
	token = strtok_r(dataCopy, delimiter, &saveptr);
	int formatIndex = 0;
	
	
	// Determine the data types for each field in the header.
	char** dataTypes = determine_data_entry_types(headerLine, fieldCount, delimiter);
	if (!dataTypes)
	{
		perror("\n\nError: NULL dataTypes in 'format_data_entry_for_plotting'.");
		free(formattedDataEntry);
		return NULL;
	}
	
	
	// Process each field based on its data type.
	while(token && formatIndex < fieldCount)
	{
		// Trim leading spaces from the token
		while (isspace((unsigned char)*token)) token++;
		
		// Determine the type of data in the token.
		const char* typeDataEntry = determine_string_representation_type(token);
		
		
		/// Handling incorrectly formatted fields.
		if (strcmp(token, "-") == 0) // Incorrectly formatted field, effectively a missing/blank value.
		{
			// If the field/token is comprised of only a single character(between delimiters within a data entry) which is found to be a hyphen, "-", then it is treated as a blank or missing space
			// As a result of the missing value for the field being represented as a nonnumeric character, the hyphen, then only the expected data type is relevant.
			if (strcmp(dataTypes[formatIndex], "numeric") == 0) // If the missing field was expected to be a numeric value, append a default value in place of the missing field.
			{
				strcat(formattedDataEntry, "0.0"); // Replace with default numeric value.
			}
			else if(strcmp(dataTypes[formatIndex], "numeric") != 0 ) // This "missing" field should be a nonnumeric value, append the hyphen as is or append some sort of arbitrary substitute(such as 0.0), so long as the functions for extracting numeric plottable data are not affected.
			{
				strcat(formattedDataEntry, "0.0"); // Replace with default numeric value... temporary, ideally the entire data set should adjust itself to ensure data is consistent.
			}
		}
		else if (strcmp(typeDataEntry, dataTypes[formatIndex]) != 0) //The determined data type does not match the expected data type
		{
			if (strcmp(dataTypes[formatIndex], "numeric") == 0) // Will be true when the field should have a numeric value but instead has a non-numeric value
			{
				strcat(formattedDataEntry, "0.0"); // Replace non-numeric values in numeric fields.
			}
			// If it is expected to be a string but is not, we remove it (by doing nothing here)
		}
		else // The determined data types do match and the type is numeric
		{
			strcat(formattedDataEntry, token); // Append correct token.
		}
		
		
		
		// Append delimiter except for the last token
		if (formatIndex < fieldCount - 1)
		{
			strncat(formattedDataEntry, delimiter, bufferSize - strlen(formattedDataEntry) - 1);
		}
		
		// Increment the index for the current field on this dataEntry and move to the next token(the next delimiter separated field in the dataEntry)
		formatIndex++;
		token = strtok_r(saveptr, delimiter, &saveptr);
	}
	
	
	
	free(dataCopy);
	for (int i = 0; i < fieldCount; i++)
	{
		free(dataTypes[i]);
	}
	free(dataTypes);
	
	
	
	// Reallocate formattedDataEntry to fit the actual length.
	char *fitOutput = realloc(formattedDataEntry, strlen(formattedDataEntry) + 1);
	if (fitOutput)
	{
		formattedDataEntry = fitOutput;
	}
	
	return formattedDataEntry;
}




int* identify_plottable_fields(char** fieldNameTypePairs, int fieldCount, const char *typeDelimiter)
{
	// Allocate memory for the plottability status array
	int* plottabilityStatus = (int*)malloc(fieldCount * sizeof(int));
	if (plottabilityStatus == NULL)
	{
		perror("\n\nError: Memory allocation failed for plottabilityStatus in 'identify_plottable_fields'.\n");
		return NULL;
	}
	
	
	// Iterate through the fields and check data types
	for (int i = 0; i < fieldCount; i++)
	{
		// Check the data type is numeric
		char* typeDelimiterPosition = strchr(fieldNameTypePairs[i], typeDelimiter[0]);
		if (typeDelimiterPosition != NULL)
		{
			char* type = typeDelimiterPosition + 1; // Move past the delimiter to get the type
			if (strcmp(type, "numeric") == 0 || strcmp(determine_string_representation_type(type), "numeric") == 0)
			{
				plottabilityStatus[i] = 1; // Field is plottable
			}
			else
			{
				plottabilityStatus[i] = 0; // Field is not plottable
			}
		}
		else
		{
			plottabilityStatus[i] = 0; // Default to not plottable if parsing fails
		}
	}
	return plottabilityStatus;
}






/**
 * capture_plottable_fields
 *
 * Captures and formats fields from a data entry that are suitable for plotting.
 * This function processes a line from the dataset and extracts numeric fields,
 * which are typically the ones used in plotting.
 * Examines a single line from this data set and
 *
 * @param headerLine The header line of the dataset.
 * @param dataEntry A line from the dataset.
 * @param fieldCount Number of fields in the data entry.
 * @param delimiter The delimiter used in the dataset.
 * @return An array of strings containing only the numeric fields from the data entry.
 */
char** capture_plottable_fields(char *headerLine, char* dataEntry, int fieldCount, const char *delimiter)
{
	// Format the data entry for plotting.
	char *formattedData = format_data_entry_for_plotting(headerLine, dataEntry, fieldCount, delimiter);
	
	
	// Allocate memory for plottable fields.
	char** plottableFields = allocate_memory_char_ptr_ptr(0, fieldCount);
	
	// Duplicate formatted data for processing.
	char* dataCopy = strdup(formattedData);
	
	char* token = strtok(dataCopy, delimiter);
	int plottableIndex = 0;
	
	// Extract numeric fields for plotting.
	while (token)
	{
		if (string_is_numeric(token))  // Check if the field is numeric.
		{
			plottableFields[plottableIndex] = allocate_memory_char_ptr(strlen(token) + 1);      // Allocate memory for the plottable field
			strcpy(plottableFields[plottableIndex], token); // Copy the token to the plottable field
			plottableIndex++;
		}
		// Get the next token
		token = strtok(NULL, delimiter);
	}
	free(dataCopy);
	free(formattedData); // Free the formatted data, which is no longer needed
	
	
	return plottableFields;
}




/**
 * capture_data_set_for_plotting
 *
 * Processes a dataset for plotting by extracting and formatting numeric fields.
 * Goes through each line in the dataset, capturing only the numeric fields, and formats them for plotting.
 *
 * @param fileContents Array of strings representing the dataset.
 * @param lineCount The number of lines in the dataset.
 * @param delimiter The delimiter used in the dataset.
 * @return An array of strings, each representing a line in the dataset with only plottable fields.
 */
char** capture_data_set_for_plotting(char** fileContents, int lineCount, const char *delimiter)
{
	// Assume the first line is the header to determine the format
	char *headerLine = fileContents[0]; // The header line is the first line of the file that is assumed to contain the names of each data field seperated by the delimiter.
	char *formatLine = fileContents[1]; // The format line is the line with the first data entries and whose contents will be analyzed to serve as the basis for formatting.
	int fieldCount = count_data_fields(formatLine);
	
	// Allocate memory for the new dataset
	char** processedDataSet = allocate_memory_char_ptr_ptr(MAX_STRING_SIZE, lineCount + 1); // Assuming max length of processed line
	
	// Extracting solely the numeric values from all of the plottable fields
	for (int i = 0; i < lineCount; i++)
	{
		// Extract plottable fields from each line of the dataset.
		char** plottableFields = capture_plottable_fields(formatLine, fileContents[i], fieldCount, delimiter);
		
		// Concatenate plottable fields into a single string for each line.
		char* concatenatedFields = concatenate_string_array(plottableFields, fieldCount, delimiter);
		
		// Store the concatenated string in the processed dataset.
		processedDataSet[i] = concatenatedFields; // Shift index by 1 to leave space for the header
		
		// Free the array of plottable fields
		for (int j = 0; j < fieldCount && plottableFields[j] != NULL; j++)
		{
			free(plottableFields[j]);
		}
		free(plottableFields);
	}
	
	
	/// Capture the
	char **fileHeader = capture_data_set_header_for_plotting(headerLine, fileContents, delimiter);
	
	
	/// Identify plottable fields and store the plottability status in an integer array as 1 for plottable and 0 for unplottable
	int *plottabilityStatus = identify_plottable_fields(fileHeader, fieldCount, ":");
	
	
	
	
	/// Loop through all fields and count the number of plottable fields
	int plottableFieldCount = 0;
	for (int i = 0; i < fieldCount; i++)
	{
		if(plottabilityStatus[i] == 1)
		{
			plottableFieldCount++;
		}
	}
	printf("\n\n plottableFieldCount: %d\n\n\n\n", plottableFieldCount);
	
	
	int unplottableFieldCount = 0; // Start as 0 then as unplottable fields are encountered, increment by 1. Use this variable to offset the index of the fieldNames array when assigning the field names
	
	
	
	// Capture the Data Field Names in a Single String Without the typeDelimiters
	const char *fieldNames[fieldCount]; //The string of field names as they appear in the first element of the fileContents array of strings
	for (int i = 0; i < fieldCount; i++)
	{
		// Identify the delimiting character seperating each field name from it's data type, for example,  'mass:numeric'  --->  typeDelimiter = ':'  &&  typeDelimiterToken = 'mass'
		const char *typeDelimiter = identify_delimiter(fileHeader, fieldCount);
		char* typeDelimiterToken = strtok(fileHeader[i], typeDelimiter); // Split the token by the type delimiter to get the name
																		 //printf("\n\n\n field: %d \n typeDelimiter: %s \n typeDelimiterToken: %s", i, typeDelimiter, typeDelimiterToken);
		
		
		if (typeDelimiterToken)
		{
			if(plottabilityStatus[i] == 1) /// If the token field name is not NULL and the field is plottable, duplicate the token to the field names
			{
				// Offset the index of the fieldNames array by the current number of unplottable fields which preceded this iteration of the loop when assigning the field names
				// so that the ordering of the plottable field names will correspond to the ordering of the plottable fields in the processed dataset
				fieldNames[i - unplottableFieldCount] = strdup(typeDelimiterToken);  // Duplicate the token to the field names while omitting unplottable fieldsa
			}
			else if(plottabilityStatus[i] == 0) /// the token field name is not NULL and the field is NOT plottable
			{
				unplottableFieldCount++; // Increment the count of unplottable fields
				
				
				// Remove the invalid field name from the field names array, decreasing the dimension of the array by 1 and pushing the remaining elements to the left
				//for (int j = i - unplottableFieldCount; j < fieldCount; j++)
				//{
				//    fieldNames[j] = fieldNames[j + 1];
				//}
				
			}
			
		}
	}
	
	
	
	// Concatenate field names into a single string and set as the first line of the processed dataset
	char* headerString = concatenate_string_array(fieldNames, plottableFieldCount, delimiter);
	processedDataSet[0] = headerString;
	return processedDataSet;
}












/**
 * extract_plottable_data_field
 *
 * Extracts a specific plottable data field from the dataset for plotting purposes.
 * It allocates memory for storing the extracted numeric data and processes each line to capture the specified field.
 *
 * @param dataSetContents Array of strings representing the dataset.
 * @param fieldIndex Index of the field to be extracted.
 * @param fieldCount Number of fields in each data entry.
 * @param delimiter The delimiter used in the dataset.
 * @return An array of doubles representing the extracted data field.
 */
double *extract_plottable_data_field(char** dataSetContents, int fieldIndex, int fieldCount, const char *delimiter)
{
	// Count the number of lines in the dataset and allocate memory for plot data.
	int lineCount = count_array_strings(dataSetContents);
	double *plotData = allocate_memory_double_ptr(lineCount);
	
	// Process each line to extract the specified field.
	for (int i = 0; i < lineCount; i++)
	{
		char* dataEntry = dataSetContents[i]; // The current line of the data set.
		char* dataCopy = strdup(dataEntry);
		char* saveptr;
		char* token = strtok_r(dataCopy, delimiter, &saveptr);
		int currentIndex = 0;
		double fieldValue = 0.0;
		
		
		// Extract the field value at the specified index.
		while (token && currentIndex < fieldCount)
		{
			if (currentIndex == fieldIndex) // The field index is passed in to capture the specified data set field only.
			{
				fieldValue = atof(token); // Convert the nonnumeric token to a numeric representation as a double.
				plotData[i] = fieldValue;
				break;
			}
			
			// Not yet at the fieldIndex, iterate the currentIndex and get the next field with token.
			currentIndex++;
			token = strtok_r(NULL, delimiter, &saveptr);
		}
		
		
		// If after iterating through all field indexes, the specified fieldIndex of the data entry has not been found, set it to NULL 0.0.
		if (!token)
		{
			fieldValue = 0.0;
			plotData[i] = fieldValue;
		}
		free(dataCopy);
	}
	
	
	return plotData;
}




/**
 * extract_plottable_data
 *
 * Extracts all plottable data fields from the dataset and writes them into separate files.
 * It processes the dataset to extract and store each plottable field in a dedicated file for easy access and plotting.
 *
 * @param dataSetContents Array of strings representing the dataset.
 * @param fieldCount Number of fields in each data entry.
 * @param dataDirectory Directory where the extracted data will be stored.
 * @param delimiter The delimiter used in the dataset.
 * @return A 2D array of doubles representing the extracted data fields.
 */
double **extract_plottable_data(char** dataSetContents, int fieldCount, const char *dataDirectory, const char *delimiter)
{
	// Count lines and allocate memory for the dataset.
	int lineCount = count_array_strings(dataSetContents);
	double **plottableDataSet = (double**)malloc(sizeof(double*) * lineCount);
	
	
	
	// Assume the first line is the header to determine the format
	char *headerLine = dataSetContents[0]; // At this point the first line of the dataset is the header line, which at this point already contains the field names of only plottable fields.
	
	
	printf("\n\n\n\n headerLine: %s", headerLine);
	
	char **dataSetFieldNames = split_tokenized_string(headerLine, delimiter, fieldCount);
	print_string_array(dataSetFieldNames, fieldCount, "Data Set Field Names");
	
	
	
	
	// Extract each plottable field.
	for(int i = 0; i < fieldCount; i++)
	{
		plottableDataSet[i] = extract_plottable_data_field(dataSetContents, i, fieldCount, delimiter);
	}
	
	
	
	
	
	
	
	// -------------------------------------------- IGNORE FROM HERE --------------------------------------------
	/// The next 45-50 lines of this function's implementation(from the defining of the the field names array 'fileHeader' to the
	/// documenting of the plottability status array makeup a segment of code unrelated to the current implementation strategy
	/// and will be incorporated into the extraction process in future development.
	char **fileHeader = capture_data_set_header_for_plotting(dataSetContents[0], dataSetContents, delimiter);
	
	/// Identify plottable fields and store the plottability status in an integer array as 1 for plottable and 0 for unplottable
	int *plottabilityStatus = identify_plottable_fields(fileHeader, fieldCount, ":");
	printf("\n\n\n\n-----plottabilityStatus-----: \n");
	for (int i = 0; i < fieldCount; i++)
	{
		printf("\nplottabilityStatus[%d]: %d", i, plottabilityStatus[i]);
	} printf("\n\n\n-------------------------\n\n\n");
	
	
	/// Loop through all fields and count the number of plottable fields
	int plottableFieldCount = 0;
	for (int i = 0; i < fieldCount; i++)
	{
		if(plottabilityStatus[i] == 1)
		{
			plottableFieldCount++;
		}
	}
	
	int unplottableFieldCount = 0;
	/* This is kept here just for reference, this function is still a work in progress with regards to the indexing of plottable fields consistently across
	 the program, currently, the non-plottable fields are filtered out in the 'capture_data_set_for_plotting' function and the other fields are automatically pushed back, and while this implementation seems to work consistently, and in some ways even streamlines the logic flow, but a reliable and consistent count and indexing of data fields and entries would endow the program with the ability to dynamically adjust the various component parts of the process to be compatible with each other, as well as accomodating significantly improved flexibility and versatility.
	 if(plottabilityStatus[i] == 1) /// If the token field name is not NULL and the field is plottable, duplicate the token to the field names
	 {
	 fieldNames[i - unplottableFieldCount] = strdup(typeDelimiterToken);  // Duplicate the token to the field names while omitting unplottable fieldsa
	 }
	 else if(plottabilityStatus[i] == 0) /// the token field name is not NULL and the field is plottable
	 {
	 unplottableFieldCount++; // Increment the count of unplottable fields
	 }
	 //*/
	/// -------------------------------------------- TO HERE --------------------------------------------
	
	
	
	
	
	
	// Write data fields into separate files for each plottable field.
	const char *extractedDataDirectory = dataDirectory;
	for(int i = 0; i < fieldCount; i++)
	{
		// Construct file path name for each field.
		char *plottingDataFilePathName = extractedDataDirectory;
		char *fieldExtracted = allocate_memory_char_ptr(plottableFieldCount);
		sprintf(fieldExtracted, "_%d-%s.txt", i, dataSetFieldNames[i]);
		
		
		plottingDataFilePathName = combine_strings(plottingDataFilePathName, fieldExtracted);
		
		
		// Write the data to files.
		FILE *file = fopen(plottingDataFilePathName, "w+");
		write_file_numeric_data(plottingDataFilePathName, plottableDataSet[i], lineCount, dataSetFieldNames[i]);
		fclose(file);
		
		
		free(plottingDataFilePathName);
		free(fieldExtracted);
	}
	
	
	
	
	
	/*-----------   Write All Data Set Fields into a Single File   -----------*/
	const char *plottingDataFilePathName = combine_strings(extractedDataDirectory, ".txt");
	FILE *file = fopen(plottingDataFilePathName, "w+");
	for(int i = 0; i < plottableFieldCount; i++)
	{
		write_file_numeric_data(plottingDataFilePathName, plottableDataSet[i], lineCount, dataSetFieldNames[i]);
	}
	fclose(file);
	
	
	
	return plottableDataSet;
}








/**
 * write_plottable_data
 *
 * Writes the plottable data extracted from the dataset to files.
 * This function captures the dataset's plottable data and writes it into files for each field,
 * making it easier for later use in plotting or analysis.
 *
 * @param dataSetContents Array of strings representing the dataset.
 * @param headerLine The header line of the dataset.
 * @param filePathName Path of the file where the data will be written.
 * @param dataDirectory Directory where the extracted data will be stored.
 * @param delimiter The delimiter used in the dataset.
 * @return The path name of the file where the plottable data is written.
 */
const char *write_plottable_data(char** dataSetContents, char *headerLine, const char *filePathName, const char *dataDirectory, const char *delimiter)
{
	// Capture and process the dataset to extract plottable data
	int lineCount = count_array_strings(dataSetContents);
	int fieldCount = count_data_fields(dataSetContents[0]);
	double **plottableDataSet = extract_plottable_data(dataSetContents, fieldCount, dataDirectory, delimiter);
	
	
	
	
	
	/// For right now this portion of the code is redundant, does exactly what the second half of 'extract_plottable_data' function does, but also allows me to capture the pathname
	/// as a return value, which is not useful for now because they are all being put in the same directory that was created specifically for plottable values anyways
	/* -----------   Write Plottable Data Set Fields into a File   -----------*/
	char *fileName = find_name_from_path(filePathName);
	const char *plottingDataFilePathName = combine_strings(filePathName, combine_strings("/", fileName));
	plottingDataFilePathName = combine_strings(plottingDataFilePathName, ".txt");
	//FILE *file = fopen(plottingDataFilePathName, "w+");
	//for(int i = 0; i < fieldCount; i++)
	//{
	//write_file_numeric_data(plottingDataFilePathName, plottableDataSet[i], lineCount);
	//}
	//fclose(file);
	//*/
	return plottingDataFilePathName;
}




/**
 * write_data_set
 *
 * Processes and writes a dataset to files, categorizing data into plottable and non-plottable types.
 * The function structures the dataset, creating separate files for plottable fields to aid in analysis
 * and plotting.
 *
 * @param fileContents Array of strings representing the dataset.
 * @param filePathName Path of the original dataset file.
 * @param delimiter The delimiter used in the dataset.
 * @return The directory where the processed dataset files are stored.
 */
char *write_data_set(char** fileContents, const char *filePathName, const char *delimiter)
{
	// Create and capture field name-type pairs from the dataset header
	char *headerLine = fileContents[0]; // Get the header line of the dataset.
	int fieldCount = count_data_fields(fileContents[0]);  // Count the number of fields in the header line.
	char **fileHeader = capture_data_set_header_for_plotting(headerLine,  fileContents, delimiter);  // Capture header information for plotting.
																									 //const char *typeDelimiter = identify_delimiter(fileHeader, fieldCount);  // Identify the delimiter used in the fileHeader field name type pairs
																									 //int primaryDelimiterCount = 1;
																									 //const char *typeDelimiter = identify_delimiter_recursive(fileHeader, fieldCount, &delimiter, &primaryDelimiterCount);
																									 //printf("\n\ntypeDelimiter: %s\n", typeDelimiter);
	
	
	
	// Process the dataset for plotting
	/*-----------   Write the NON-Plottable Data to Files   -----------*/
	int lineCount = count_file_lines(filePathName, MAX_NUM_FILE_LINES);
	char **plottingData = capture_data_set_for_plotting(fileContents, lineCount, delimiter);  // Capture data suitable for plotting.
	
	printf("\n\n\n\n fileContents[0]: %s\n plottingData[0]: %s\n\n", fileContents[0], plottingData[0]);
	print_file_contents(plottingData, lineCount);
	
	
	
	
	
	/*-----------   Create the Name of a File for Plotting Data Set in Same Directory as Original File   -----------*/
	char *directoryPathName = find_file_directory_path(filePathName); // Get the directory path of the file.
	char *fileName = find_name_from_path(filePathName); // Extract the filename from the path.
	
	
	
	// Create a directory for plottable data fields
	char *dataDirectory = create_directory(filePathName, "_Plottable_Fields"); // Create a directory for this CSV file's plottable data fields.
	
	
	
	//The directory to be created at the same level and location as the passed in file pathname
	directoryPathName = combine_strings(directoryPathName, fileName);
	directoryPathName = combine_strings(directoryPathName, "_Plottable_Fields");
	
	
	
	
	// Path to the directory in which the plottable data fields will be located, the full pathnames of the data fields file's will be this string + the actual name of the file
	char *plottableDataFieldsDirectoryFilePath = combine_strings("/", combine_strings(fileName, "_Plottable_Field"));
	const char *plottableFieldsPathName = combine_strings(dataDirectory, plottableDataFieldsDirectoryFilePath); // Full path for plottable data fields.
	
	
	
	// Write plottable fields to files, Populate the Contents of the Plotting File with the Contents of the Array of Strings(i.e., the data entries)
	// Write plottable fields to the directory at 'directoryPathName' with pathnames 'plottableFieldsPathName'(to be followed by the index of the field and the .txt extension)
	write_plottable_data(plottingData, fileContents[0], directoryPathName, plottableFieldsPathName, delimiter);
	
	
	return dataDirectory;
}
