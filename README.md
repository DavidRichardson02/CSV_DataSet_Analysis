# CSV File Data Set Analysis

## Overview
This repository provides a comprehensive suite of C utilities and functions for standardizing, automating data extraction, and preprocessing CSV file contents for analysis. The program captures and formats CSV file contents, generates custom directories of files, extracts data, performs analysis, and generates MATLAB scripts for further visualization.
<br/>

## Usage

Include the necessary header files in your C project. Below is a basic example to get you started:

```c
#include "GeneralUtilities.h"
#include "StringUtilities.h"
#include "FileUtilities.h"
#include "DataSetUtilities.h"
#include "Integrators.h"
#include "StatisticalMethods.h"


int main() {
    const char *filePath = "path/to/your/csvfile.csv";
    char **fileContents = capture_and_prepare_data_set_contents(filePath);
    DataSetAnalysis dataSet = configure_data_set_analysis(filePath);
    run_data_set(filePath, fileContents, dataSet.lineCount, dataSet.delimiter);
    return 0;
}
```

<br/>

## Header Files

### GeneralUtilities.h

Provides a collection of general utility functions for memory allocation, date/time conversions, bitwise operations, and sorting algorithms optimized for handling large datasets.
<br/>

### StringUtilities.h

Offers utilities for string manipulation, analysis, and formatting. Useful for data parsing and preprocessing in data integration and analysis.
<br/>

### FileUtilities.h

Includes functions for file-related operations such as analyzing file paths, counting data fields and characters, reading/writing file contents, merging file contents, and managing directories of files.
<br/>

### DataSetUtilities.h

Contains functions for processing, analyzing, and formatting datasets stored in CSV format, facilitating data visualization and plotting by preparing datasets in a suitable format.
<br/>

### Integrators.h

Defines functions for numerical integration, including methods like the Trapezoidal rule, Simpson's rule, Midpoint rule, Romberg's method, Euler's method, Verlet's method, and the Runge-Kutta method.
<br/>

### StatisticalMethods.h

Provides functions for generating and sampling datasets, computing statistical properties, integrating statistical distributions, and performing statistical tests to assess the normality of datasets.
<br/>





<br/>
<br/>

NOTE: 

1. The attached 'physics_particles.txt' and 'weather_measurements.csv' files are the two csv files I was using for testing in my program, and I wrote the functions for parsing these files as practice for making the standardized version of the parsing function to work with all csv files. Also, this is not for work or school but is just a freetime project.

2. The methods of this program are primarily designed for datasets where the first line in the CSV formatted file is the header, and the following lines contain data entries.

3. The vast vast vast majority of the time spent working on this coding exercise for me was spent on trying to standardize data preprocessing to ensure compatibility throughout the entire program, namely, the Utilities files. This is worth being aware of simply due to how under-developed the other parts of the program are(the integrators and statistical methods are mostly exempt, not much of a computational challenge, just math challenge).

<br/>

![Screenshot 2024-02-20 at 12 59 02 AM](https://github.com/DavidRichardson02/CSV-File-Data-Set-Analysis/assets/144840390/da25c0f9-ca64-4e64-96b1-d33eb89e974c)
<br/>

The screenshot attached directly above this text is a picture of the directory my XCode c program is contained in, the portion of the screen circled in red displays the directory and files that were automatically generated for the data set 'physics_particles'. The name of each file in the directory is a combination of the name of the data set file(extracted from pathname), the category these files belong to(Plottable_Field), the order in which they appear in the data set when all non-plottable fields have been omitted and the plottable fields are pushed back to take their place, and the final portion of the naming comes from the name of the data field itself. Lastly, the generated plottable data file with no number or name is a combination of all of the plottable data fields lined up in order and written to this file with a line skipped between each one. 

<br/>

The original file of the example data set used by me, 'physics_particles.csv', as well as the generated file of all of the formatted plottable data fields, 'physics_particles_Plottable_Field.txt', can be found on the main page of this repository, also there is a generated file for the unplottable data fields that I plan to incorporate into the MATLAB script creation later in development but it is incomplete as of yet(and even when it's finished it will only really be noticeable for it's cosmetic affect in providing labels and/or units to create the plots with).




