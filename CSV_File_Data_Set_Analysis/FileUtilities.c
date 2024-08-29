//  FileUtilities.c
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02


#include "FileUtilities.h"
#include "CommonDefinitions.h"
#include "GeneralUtilities.h"
#include "StringUtilities.h"
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>






/**
 * determine_file_depth
 *
 * Calculates the depth of a file in a directory structure based on its path.
 * This function counts the number of '/' characters in the file path, assuming a Unix-like file system.
 * This function only works with string-based paths and assumes '/' as the directory separator.
 *
 *
 * @param filePathName A string representing the file path.
 * @return The depth of the file in the directory hierarchy.
 */
int determine_file_depth(const char* filePathName) //Only works for String-based path.
{
	//Simply count the number of '/'.
	int depthCount = 0;
	for(int i = 0; filePathName[i] != '\0'; i++)
	{
		if(filePathName[i] == '/')
		{
			depthCount++;
		}
	}
	return depthCount;
}


/**
 * identify_file_extension
 *
 * Identifies the file extension of a given file path.
 * This function uses the `strrchr` function to find the last occurrence of the '.' character,
 * which is assumed to be the start of the file extension. This function assumes that the file
 * path is a null-terminated string and that the extension is anything following the last '.'
 * in the path.
 *
 * @param filePathName A string representing the file path (i.e., "/home/user/file.txt").
 * @return A pointer to the file extension within the given file path, or NULL if no extension is found.
 */
char *identify_file_extension(const char* filePathName) //Only works for String-based path.
{
	if(count_character_occurrences(filePathName, '.') == 0) // If this pathname does not have a file extension(ex: a directory pathname)
	{
		return "\0";
	}
	
	
	//the strrchr function to get the position of the last occurrence of a character in a string.
	char *fileExtension = strrchr(filePathName, '.');
	
	
	return fileExtension;
}


/**
 * find_file_directory_path
 *
 * Extracts the directory path from a given full file path.
 * This function determines the directory path of a file by counting the characters up to the last '/'
 * character in the path. It dynamically allocates memory to store the directory path.
 * The function assumes Unix-like file paths and uses '/' as the directory separator.
 *
 * @param filePathName A string representing the full file path.
 * @return A dynamically allocated string containing the directory path.
 */
char* find_file_directory_path(const char* filePathName)
{
	int fileDepthLevel = determine_file_depth(filePathName);
	
	//Simply count the number of '/'.
	int directoryDepthLevel = 0;
	
	
	
	int directoryPathNameCharacterCount = 0; //before capturing the actual directory path name that this file is contained inside of, first need to determine how many
											 //characters the directory path name is from the full path name, i.e., need to count all characters except everything after the last '/'.
											 //Then, can use this number to allocate memory for a char* of an appropriate size and it will be straightforward to capture the all characters up
											 //until the cutoff point when the directory that this file is contained in is reached.
	
	
	for(int i = 0; filePathName[i] != '\0' && directoryDepthLevel != fileDepthLevel; i++) //for loop will terminate as soon as the last '/' is reached.
	{
		if(filePathName[i] == '/')
		{
			directoryDepthLevel++;
		}
		
		directoryPathNameCharacterCount++;
	}
	
	char* directoryPathName;
	directoryPathName = allocate_memory_char_ptr(directoryPathNameCharacterCount);
	for(int i = 0; i < directoryPathNameCharacterCount; i++)
	{
		directoryPathName[i] = filePathName[i];
	}
	
	return directoryPathName;
}




/**
 * find_name_from_path
 *
 * Extracts the filename from a given full file path.
 * This function isolates the filename from the full path by removing the directory path and the file extension.
 * It dynamically allocates memory for the filename. Assumes Unix-like string-based file paths.
 *
 * @param filePathName A string representing the full file path.
 * @return A dynamically allocated string containing the filename.
 */
char* find_name_from_path(const char* filePathName)
{
	char* fileExtension = identify_file_extension(filePathName);
	
	char* directoryPathName = find_file_directory_path(filePathName);
	
	
	unsigned long filePathLength = strlen(filePathName), fileExtensionLength = strlen(fileExtension), directoryPathLength = strlen(directoryPathName);
	unsigned long fileNameLength = filePathLength - (directoryPathLength + fileExtensionLength);
	
	
	
	char *fileName;
	fileName = allocate_memory_char_ptr(fileNameLength);
	
	for(int i = 0; i < fileNameLength; i++)
	{
		fileName[i] = filePathName[i+directoryPathLength];
	}
	
	
	return fileName;
}








/**
 * count_data_fields
 *
 * Counts the number of data fields in a CSV-style line.
 * This function processes a line from a CSV file and counts the number of fields based on commas.
 * It also handles empty fields (i.e., two consecutive commas) by temporarily replacing them with spaces.
 * The function assumes that the input string is null-terminated and that the fields are
 * separated by commas. It modifies the input string temporarily but restores its original state.
 *
 * @param lineContents A string representing a line from a CSV file.
 * @return The number of data fields in the line.
 */
int count_data_fields(char* lineContents)
{
	for(int i = 0; i < strlen(lineContents)+1;i++)
	{
		if(lineContents[i] == ',' && lineContents[i+1] == ',')
		{
			lineContents[i] = ' ';
		}
	}
	
	
	int count = 0;
	// If the header is not empty, start with count 1 (the first field before any comma).
	if (lineContents && strlen(lineContents) > 0)
	{
		count = 1;
	}
	
	// Iterate over each character in the string.
	for (int i = 0; lineContents[i] != '\0'; i++)
	{
		// Increment count at each comma, which indicates a new field.
		if (lineContents[i] == ',')
		{
			count++;
		}
	}
	return count;
}




/**
 * count_plot_data_fields
 *
 * Counts the number of fields in a data entry for plotting.
 * This function tokenizes a CSV-style line using commas as delimiters and counts the number of tokens.
 * It is specifically intended for counting fields in data entries for plotting purposes. This function
 * creates a copy of the input line for tokenization and frees it after counting. It prints the token count
 * to standard output. Assumes a null-terminated input string and that the data is CSV formatted.
 *
 * @param lineContents A string representing a line from a CSV file.
 * @return The number of fields in the data entry.
 */
int count_plot_data_fields(char* lineContents, const char *delimiter)
{
	// Counts the number of fields in a data entry.
	int count = 0;
	char* dataCopy = strdup(lineContents);
	char* token = strtok(dataCopy, delimiter);
	while (token)
	{
		count++;
		token = strtok(NULL, delimiter);
	}
	free(dataCopy);
	
	printf("\n\ncount_plot_data_fields: %d", count);
	return count;
}




/**
 * count_file_lines
 *
 * Counts the number of lines in a file.
 * This function opens a file specified by its path (filePathName) and counts
 * the number of lines it contains, up to a maximum specified by maxLines.
 * It uses fgets to read each line into a buffer and increments a counter
 * for each line read.
 *
 * @param filePathName A string representing the path to the file.
 * @param maxLines An integer specifying the maximum line length buffer.
 * @return The total number of lines in the file.
 */
int count_file_lines(const char* filePathName, int maxLines)
{
	printf("\nEntering: 'count_file_lines' function.\n");
	//Open the file at the specified path and ensure file is opened properly.
	FILE *tempFile = fopen(filePathName, "r");
	if (!tempFile)
	{
		perror("\n\nError: Unable to open file for 'count_file_lines'.\n");
		exit(1);
	}
	
	
	char line[maxLines]; // Buffer to hold each line from the file.
						 // Count the number of lines in the file from a starting point numLinesToSkip away from the first line.
	int count = 0;
	while((fgets(line, sizeof(line), tempFile)))
	{
		count++;
	}
	
	
	if (count >= maxLines)
	{
		printf("\n\nError: The number of lines in the file '%s' exceeded the expected number of maximum lines, %zu 'count_file_lines'.\n", filePathName, MAX_NUM_FILE_LINES);
		perror("\n\nError: File  'count_file_lines'.\n");
		exit(1);
	}
	fclose(tempFile);
	return count;
}




/**
 * count_file_lines_characters
 *
 * Counts the characters in each line of a file.
 * This function opens a file specified by filePathName and counts the number
 * of characters in each line, starting from a specified line (startLine) and
 * up to a total number of lines (lineCount). It uses getline for dynamic allocation
 * and handling of varying line lengths. The newline character is excluded from the count.
 *
 * @param filePathName A string representing the path to the file.
 * @param lineCount An integer specifying the number of lines to process.
 * @return A pointer to an array of integers, each representing the character count of a line.
 */
int* count_file_lines_characters(const char* filePathName, int lineCount)
{
	//Open the file at the specified path and ensure file is opened properly.
	FILE *tempFile = fopen(filePathName, "r");
	if (!tempFile)
	{
		perror("\n\nError: Unable to open file for 'count_file_lines_characters'.\n");
		exit(1);
	}
	
	
	int *fileCharCounts = allocate_memory_int_ptr(lineCount); //allocate memory to store the number of characters for each line.
															  // Initialize the array with zeros.
	for (int i = 0; i < lineCount; i++)
	{
		fileCharCounts[i] = 0;
	}
	
	
	// Declare variables for
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int currentLine = 0;
	
	// Read each line from the file.
	while ((read = getline(&line, &len, tempFile)) != -1 && currentLine < lineCount)
	{
		// Count characters in the line, excluding the newline character.
		for (int i = 0; i < read; i++)
		{
			if (line[i] != '\n')
			{
				fileCharCounts[currentLine]++;
			}
		}
		currentLine++;
	}
	
	free(line);
	fclose(tempFile);
	
	return fileCharCounts;
}




/**
 * count_characters_in_file_lines_range
 *
 * Counts the characters in each line of a file.
 * This function opens a file specified by filePathName and counts the number
 * of characters in each line, starting from a specified line (startLine) and
 * up to a total number of lines (lineCount). It uses getline for dynamic allocation
 * and handling of varying line lengths. The newline character is excluded from the count.
 *
 filePathName A string representing the path to the file.
 * @param lineCount An integer specifying the number of lines to process.
 * @param startLine An integer specifying the line number to start counting from.
 * @return A pointer to an array of integers, each representing the character count of a line.
 */
int* count_characters_in_file_lines_range(const char* filePathName, int lineCount, int startLine)
{
	//Open the file at the specified path and ensure file is opened properly.
	FILE *tempFile = fopen(filePathName, "r");
	if (!tempFile)
	{
		perror("\n\nError: Unable to open file for 'count_file_lines_characters'.\n");
		exit(1);
	}
	
	
	
	int *fileCharCounts = allocate_memory_int_ptr(lineCount); //allocate memory to store the number of characters for each line.
	
	
	// Initialize the array with zeros.
	for (int i = 0; i < lineCount; i++)
	{
		fileCharCounts[i] = 0;
	}
	
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int currentLine = 0;
	
	// Read each line from the file.
	while ((read = getline(&line, &len, tempFile)) != -1 && currentLine < lineCount)
	{
		// If it's a line that needs to be skipped, just increment the line index.
		if (currentLine < startLine)
		{
			currentLine++;
			continue;
		}
		
		// Count characters in the line, excluding the newline character.
		for (int i = 0; i < read; i++)
		{
			if (line[i] != '\n')
			{
				fileCharCounts[currentLine]++;
			}
		}
		currentLine++;
	}
	
	free(line);
	fclose(tempFile);
	
	return fileCharCounts;
}








/**
 * read_file_contents
 *
 * Reads the contents of a file into an array of strings.
 * This function opens a file and reads its contents line by line, counting the characters in each line,
 * and then reading each line into a dynamically allocated array of strings (char pointers).
 * Each line is stored in the array up to the specified lineCount.
 *
 * @param filePathName A string representing the path of the file to be read.
 * @param lineCount An integer specifying the number of lines to read from the file.
 * @return A pointer to an array of strings, each string holding the content of a line.
 */
char** read_file_contents(const char* filePathName, int lineCount)
{
	// Checking if the filePathName is NULL or if lineCount is less than or equal to 0.
	// If so, return NULL as it's not possible to read the file contents under these conditions.
	if (filePathName == NULL || lineCount <= 0)
	{
		perror("\n\nError:filePathName is NULL or if lineCount is less than or equal to 0 in 'read_file_contents'.");
		return NULL;
	}
	
	
	//Open the file at the specified path and ensure file is opened properly.
	FILE *tempFile = fopen(filePathName, "r");
	if (!tempFile)
	{
		perror("\n\nError: Unable to open file for 'read_file_contents'.");
		exit(1);
	}
	
	
	// First, count the characters in each line.
	int *charCounts = count_file_lines_characters(filePathName, lineCount);
	
	
	// Allocate memory for the array of strings.
	char **fileContents = allocate_memory_char_ptr_ptr(MAX_STRING_SIZE, lineCount);
	
	size_t len = 0;
	size_t read;
	char *line = NULL;
	int currentLine = 0;
	
	
	
	// Read the file line by line and store each line in the corresponding string.
	while ((read = getline(&line, &len, tempFile)) != -1 && currentLine < lineCount)
	{
		// Allocate memory for the current line in the array.
		fileContents[currentLine] = allocate_memory_char_ptr((charCounts[currentLine] + 1));
		
		
		
		// Copy the contents of the line into the allocated memory
		strncpy(fileContents[currentLine], line, charCounts[currentLine]);
		fileContents[currentLine][charCounts[currentLine]] = '\0'; // Null-terminate the string
		
		currentLine++;
	}
	free(line);
	free(charCounts);
	fclose(tempFile);
	
	
	
	// Replace double commas with a space to handle missing values.
	// This is meant to ensure that empty fields are accounted for.
	for (int i = 0; i < lineCount; i++)
	{
		char *tempData = fileContents[i];
		int extraSpaceNeeded = 0;
		
		// For each line, scan through the string and count how many times two commas appear consecutively. This will determine the additional space needed.
		for (int j = 0; tempData[j] != '\0'; j++)
		{
			if (tempData[j] == ',' && tempData[j + 1] == ',')
			{
				extraSpaceNeeded += strlen(",0.0,") - 1; // Subtract 1 because we replace two commas.
			}
		}
		
		
		// Allocate new memory for the modified string
		char *modifiedData = allocate_memory_char_ptr(strlen(tempData) + extraSpaceNeeded + 1);
		
		
		// Copy characters from the original string to the new string. When two consecutive commas are encountered, insert ", 0.0" and adjust the index accordingly.
		for (int j = 0, k = 0; tempData[j] != '\0'; j++, k++)
		{
			if (tempData[j] == ',' && tempData[j + 1] == ',')
			{
				strcpy(&modifiedData[k], ",0.0,");
				k += strlen(",0.0,") - 1; // Adjust index.
				j++; // Skip the next comma in the original string.
			}
			else
			{
				modifiedData[k] = tempData[j];
			}
		}
		modifiedData[strlen(tempData) + extraSpaceNeeded] = '\0'; // Null-terminate the new string.
		
		// Replace the original string with the modified string.
		free(fileContents[i]); // Free the memory allocated for the original string.
		fileContents[i] = modifiedData;
	}
	
	
	return fileContents;
}




/**
 * write_file_contents
 *
 * Writes an array of strings to a file.
 * This function opens a file for writing and writes each string from
 * the provided array (fileContents) to the file followed by a newline character.
 * If writing fails, an error is reported and the program exits.
 *
 * @note The function assumes that the array is NULL-terminated, and that the individual
 *       strings do not contain newline characters at the end.
 *
 * @param filename A string representing the name of the file to write to.
 * @param fileContents A pointer to an array of strings to be written.
 */
void write_file_contents(const char *filename, char **fileContents)
{
	// Open the file for append and update
	FILE *file = fopen(filename, "a+");
	
	if (file == NULL)
	{
		perror("\n\nError opening file for writing in 'write_file_contents'.");
		exit(1);
	}
	
	// Iterate through the array of strings
	for (int i = 0; fileContents[i] != NULL; i++)
	{
		// Write each string to the file
		if (fputs(fileContents[i], file) == EOF)
		{
			perror("\n\nError writing to file in 'write_file_contents'.");
			fclose(file);
			exit(1);
		}
		
		// Add a newline character to ensure the strings contain one
		if (fputc('\n', file) == EOF)
		{
			perror("\n\nError writing to file in 'write_file_contents'.");
			fclose(file);
			exit(1);
		}
	}
	
	fclose(file);
}


/**
 * write_file_numeric_data
 *
 * Writes an array of double values to a file.
 * This function opens a file in append mode and writes each double value from the provided
 * data array to the file, each on a new line. The precision of the double values
 * is maintained up to 17 significant digits. If writing fails, an error is reported
 * and the program exits.
 *
 * @param filename A string representing the name of the file to write to.
 * @param data A pointer to an array of double values.
 * @param countDataEntries An integer specifying the number of entries in the data array.
 */
void write_file_numeric_data(const char *filename, double *data, int countDataEntries, const char *dataFieldName)
{
	// Open the file for append and update
	FILE *file = fopen(filename, "a+");
	if (file == NULL)
	{
		perror("\n\nError opening file for writing in 'write_file_numeric_data'.");
		exit(1);
	}
	
	
	
	
	/// Write the name of the data field to the file... the two lines of code below are analogous, although fprintf is primarily used for writing formatted data to a file, whereas fputs is more generally used to write string(s) to a file.
	fprintf(file, "%s\n", dataFieldName);
	//fputs(dataFieldName, file); //fputc('\n', file);
	
	
	
	
	// Iterate through the array of strings
	for (int i = 1; i < countDataEntries; i++)
	{
		// Write each data entry to the file
		if (fprintf(file, "%.17g", data[i]) < 0)
		{
			perror("\n\nError writing to file in 'write_file_numeric_data'.");
			fclose(file);
			exit(1);
		}
		// Optionally add a newline character if the strings do not already contain one
		if (fputc('\n', file) == EOF)
		{
			perror("\n\nError writing to file in 'write_file_numeric_data'.");
			fclose(file);
			exit(1);
		}
		
		
		if(i == countDataEntries-1)
		{
			fputc('\n', file);
		}
	}
	
	
	fclose(file);
}








/**
 * generate_merged_filename
 *
 * Generates a merged filename based on two input file paths.
 * The merged filename is constructed by combining the base names of the input files,
 * appending "_merged" to the combined base names, and then prepending the directory path.
 *
 * @param filePath1 The path to the first file.
 * @param filePath2 The path to the second file.
 * @return A dynamically allocated string containing the new filename.
 */
char* generate_merged_filename(const char* filePath1, const char* filePath2)
{
	// Extract the base names of the files, without extension or path, to avoid directory paths in the merged filename.
	char* baseName1 = find_name_from_path(filePath1);
	char* baseName2 = find_name_from_path(filePath2);
	
	
	// Calculate the length needed for the new filename
	size_t mergedLength = string_length(baseName1) + string_length(baseName2) + string_length("_merged") + 2;  // +2 for underscore and null terminator
	char* mergedFilename = allocate_memory_char_ptr(mergedLength);
	if (mergedFilename == NULL)
	{
		perror("Memory allocation failed for merged filename");
		return NULL;
	}
	sprintf(mergedFilename, "%s_%s_merged", baseName1, baseName2); // Construct the new filename
	
	
	char *directoryPathName1 = find_file_directory_path(filePath1);
	char *directoryPathName2 = find_file_directory_path(filePath2);
	size_t characterCountMergedPathname = mergedLength;
	characterCountMergedPathname += (directoryPathName1 == directoryPathName2) ? string_length(directoryPathName1) : max(string_length(directoryPathName1), string_length(directoryPathName2)); // If they are contained within the same enclosing directory, append the first directory pathname, otherwise, use the longer pathname.
	
	
	char *mergedFilePathName = allocate_memory_char_ptr(characterCountMergedPathname); // Allocate memory for the merged file path name
	sprintf(mergedFilePathName, "%s%s", directoryPathName1, mergedFilename); // Construct the full path name for the merged file
	
	
	// Clean up
	free(baseName1);
	free(baseName2);
	free(mergedFilename);
	free(directoryPathName1);
	free(directoryPathName2);
	
	return mergedFilePathName; // Return the full path name of the merged file
}


/**
 * merge_two_files
 *
 * This function merges the contents of two files into a new file.
 * It reads the contents of the two input files, combines them, and writes the merged content to a new file.
 * The new file's name is generated based on the names of the input files.
 *
 * @param filePath1: The path to the first input file.
 * @param filePath2: The path to the second input file.
 * @return The path to the merged file.
 */
char* merge_two_files(const char* filePath1, const char* filePath2)
{
	// Count lines in each file
	int lineCount1 = count_file_lines(filePath1, MAX_NUM_FILE_LINES);
	int lineCount2 = count_file_lines(filePath2, MAX_NUM_FILE_LINES);
	
	char** contents1 = read_file_contents(filePath1, lineCount1);
	char** contents2 = read_file_contents(filePath2, lineCount2);
	
	
	// Create filename based on input filenames, "merged" string literal, and enclosing directory of the files
	char* mergedFilename = generate_merged_filename(filePath1, filePath2);  // Combines the base names of the files(extracted from full path) then appends this to the path to the enclosing directory of the files
	
	
	// Allocate memory for the merged contents array of strings
	char** mergedContents = allocate_memory_char_ptr_ptr(MAX_STRING_SIZE, lineCount1 + lineCount2 + 1);  // +1 for safety/null termination
	
	
	
	// Copy contents from both files into the merged array
	int i, j;
	for (i = 0; i < lineCount1; i++)
	{
		mergedContents[i] = duplicate_string(contents1[i]); // Copy contents from the first file
	}
	for (j = 0; j < lineCount2; j++, i++)
	{
		mergedContents[i] = duplicate_string(contents2[j]); // Copy contents from the second file
	}
	mergedContents[i] = NULL;  // Null-terminate the array
	
	
	
	// Write the merged contents to the new file
	write_file_contents(mergedFilename, mergedContents);
	
	// Clean up memory
	deallocate_memory_char_ptr_ptr(contents1, lineCount1);
	deallocate_memory_char_ptr_ptr(contents2, lineCount2);
	deallocate_memory_char_ptr_ptr(mergedContents, lineCount1 + lineCount2);
	
	return mergedFilename; // Return the path to the merged file
}












/**
 * create_file_header
 *
 * Generates a file header string for a given file path.
 * It extracts the file name and extension from the file path and combines them with the program's standard title format string.
 * It also gets the current date and time, formats it into a string, and includes it in the header.
 *
 * @param filePathName A string representing the full path of the file.
 * @return A string representing the generated file header.
 */
char* create_file_header(const char *filePathName)
{
	//Use the name of the file and the file extension(both pulled from full pathname) in addition to the program's standard title format string to create a title for the file used in the header line
	char *fileTitle = find_name_from_path(filePathName);
	char *fileExtension = identify_file_extension(filePathName);
	
	
	time_t currentTime;
	struct tm timeInfoStorage;
	struct tm *timeInfo = &timeInfoStorage; // Obtain a pointer to the struct tm.
	
	time(&currentTime); // Obtain current time as time_t structure
	timeInfo = thread_safe_localtime(&currentTime, &timeInfoStorage); // Convert time_t to tm as Local Time
	
	
	
	
	char dateTimeString[MAX_STRING_SIZE]; // Buffer to hold the resulting string
										  // Use sprintf to format the string and store it in dateTimeString, Note: tm_year is years since 1900
	sprintf(dateTimeString, "%s %s %02d, %04d at %02d:%02d", weekDays[timeInfo->tm_wday], months[timeInfo->tm_mon], timeInfo->tm_mday, 1900 + timeInfo->tm_year, timeInfo->tm_hour, timeInfo->tm_min);
	
	
	char *fileHeaderTitle;
	fileHeaderTitle = combine_strings(fileTitle, fileExtension);
	fileHeaderTitle = combine_strings(fileHeaderTitle, stdFileHeaderFormat);
	fileHeaderTitle = combine_strings(fileHeaderTitle, dateTimeString);
	return fileHeaderTitle;
}




/** create_txt_file_from_existing
 *
 * Automates and standardizes the creation of files(in my program) for data operations
 * 1. Automatically creates, names, and places a file into a directory based on the passed in file name and location
 * 2. Writes an automatically generated header line to the file
 * 3. Returns a char* that is the pathname of the created file
 */
char* create_txt_file_from_existing(const char* dataSetFilePathName)
{
	/* Find the existing file's extension, directory pathname, and the actual name of the file */
	char *dataSetFileExtension = identify_file_extension(dataSetFilePathName);
	char *directoryPathName = find_file_directory_path(dataSetFilePathName);
	char *dataSetFileName = find_name_from_path(dataSetFilePathName);
	
	
	/* Combine the existing file's extension, directory pathname, and the actual name with the newTxtFileStringIdentifier to create the pathname of the new file*/
	char *newTxtFilePathName = combine_strings(directoryPathName, dataSetFileName);
	newTxtFilePathName = combine_strings(newTxtFilePathName, *stdNewTxtFileIdentifier);
	newTxtFilePathName = combine_strings(newTxtFilePathName, dataSetFileExtension);
	
	
	/* Create the file and write a header to it  */
	FILE *newTxtDataFile = fopen(newTxtFilePathName, "w"); //open the file at 'newTxtFilePathName' for writing
	char* newFileHeader = create_file_header(newTxtFilePathName); //create the file header
	fprintf(newTxtDataFile, "%s", newFileHeader); //write the header to the file
	
	
	// Return the pathname of the file
	return newTxtFilePathName;
}




/**
 * create_directory
 *
 * Creates a directory for storing dataset files. This function first determines
 * the directory path based on the provided file path, then checks if a directory
 * with the intended name already exists. If it does, the existing directory is
 * deleted to ensure a fresh start. Finally, a new directory is created.
 *
 * @param filePathName The path of the file for which the directory is to be created.
 * @return A pointer to a string containing the path of the newly created dataset directory.
 */
char *create_directory(const char *filePathName, const char *directoryName)
{
	// Extracts the directory path and dataset file name from the provided file path.
	char *directoryPathName = find_file_directory_path(filePathName);
	char *dataSetFileName = find_name_from_path(filePathName);
	dataSetFileName = combine_strings(dataSetFileName, directoryName);
	char *dataDirectoryPathName = combine_strings(directoryPathName, dataSetFileName);
	
	
	// Check if the directory already exists, and delete it if it does
	struct stat st;
	if (stat(dataDirectoryPathName, &st) == 0)
	{
		// If it exists, use delete_directory to remove it along with its contents.
		delete_directory(dataDirectoryPathName);
	}
	
	
	// Create the new directory with read, write, and execute permissions for the owner.
	int status = mkdir(dataDirectoryPathName, 0700);
	if (status == 0)
	{
		//printf("\n\nDirectory created in: '%s'.", dataDirectoryPathName); // Commented out the message for sucessful directory creation, annoying
	}
	else
	{
		perror("\n\nError: mkdir failed in 'directoryPathName'.");
	}
	
	
	free(directoryPathName);
	free(dataSetFileName);
	return dataDirectoryPathName;
}




/**
 * delete_directory
 *
 * Recursively deletes a directory and all its contents. This function opens the
 * specified directory, iterates through all its entries, and deletes each one.
 * If an entry is a subdirectory, the function calls itself recursively.
 * It is designed to handle normal files and directories but not special file types
 * like symbolic links.
 *
 * @param path The path of the directory to be deleted.
 * @return 0 on successful deletion, -1 on failure with errno set appropriately.
 */
int delete_directory(const char *path)
{
	// Open the directory and prepare for iteration over its contents.
	DIR *dir = opendir(path);
	size_t path_len = strlen(path);
	int r = -1;
	
	
	if (dir)
	{
		struct dirent *p;
		r = 0;
		
		
		// Iterate over each entry in the directory.
		while (!r && (p=readdir(dir)))
		{
			int r2 = -1;
			char *buf;
			size_t len;
			
			// Skip the names "." and ".." as we don't want to recurse on them.
			if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
			{
				continue;
			}
			
			
			// Prepare the full path of the entry.
			len = path_len + strlen(p->d_name) + 2;
			buf = malloc(len);
			
			if (buf)
			{
				struct stat statbuf;
				snprintf(buf, len, "%s/%s", path, p->d_name);
				
				// Delete the directory entry based on its type (file or directory).
				if (!stat(buf, &statbuf))
				{
					if (S_ISDIR(statbuf.st_mode))
					{
						r2 = delete_directory(buf);
					}
					else
					{
						r2 = remove(buf);
					}
				}
				free(buf);
			}
			r = r2;
		}
		closedir(dir);
	}
	
	
	// Remove the directory itself after its contents have been cleared.
	if (!r)
	{
		r = rmdir(path);
	}
	
	
	return r;
}






