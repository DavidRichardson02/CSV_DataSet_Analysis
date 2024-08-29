//  DebuggingUtilities.h
//  CSV_File_Data_Set_Analysis
//  DavidRichardson02
/**
 * DebuggingUtilities code: Provides a number of utility functions for debugging and testing purposes.
 * These functions facilitate the printing of various data types, such as strings, arrays, and files, to the console.
 */


#ifndef DebuggingUtilities_h
#define DebuggingUtilities_h


#include <stdio.h>




// ------------- Helper Functions for Printing Various Representations of String and Arrays for Debugging -------------
/// \{
void print_string(char* string, char*label); // Prints a single string.
void print_string_array(char** stringArray, int stringCount, char* label); // Prints an array of strings.
void print_string_array_array(char*** stringArrayArray, int stringArraysCount, int stringSubArraysCount, char* label); // NOTE: assumes the dimension of all sub-arrays is the same
void print_array(int n, double *data, char* label); // Prints an array of doubles.
void print_array_array(double **data, int rows, int columns, char* label); // Prints a 2D array of doubles.
void print_char_ptr_array(char *charPtrArr[], int stringCount, char* label); // Prints an array of char pointers.
/// \}






// ------------- Helper Functions for File Display -------------
/// \{
void print_file(const char* filePathName); // Prints key information about a file as well as its contents.
void print_file_contents(char** fileContents, int countFileLines); // Prints file contents from an array of strings.
/// \}






#endif /* DebuggingUtilities_h */
