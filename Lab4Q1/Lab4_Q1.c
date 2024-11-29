/*
Notes:
    5 invalids are present in data.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // File pointers
    FILE *inputFile, *outputFile;
    float next_value;
    int invalidcount = 0;
    char lineBuffer[100];

    // Ensure that no errors occur when opening or creating files
    inputFile = fopen("data.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Unable to open input file\n"); 
        return -1; // Returning -1 specifically indicates that the data.txt file couldn't be opened
    }    
    outputFile = fopen("dataOut.txt", "w");
    if (outputFile == NULL){
        printf("Error: Unable to create output file\n"); 
        fclose(inputFile); // Close input file 
        return -2; // Returning -2 signals that the dataOut.txt file couldn't be opened for writing
    } 

    // fgets reads one line at a time from inputFile and stores it in lineBuffer. 
    // The while loop continues until the end of the file is reached (fgets returns NULL)
    while (fgets(lineBuffer, sizeof(lineBuffer), inputFile) != NULL) {
        // strtok retrieves the first token from the line
        char *token = strtok(lineBuffer, " \n");
        
        // Process each token
        while (token != NULL) {
            // Attempt float conversion. If successful, sscanf returns 1, and the value is stored in next_value
            if (sscanf(token, "%f", &next_value) == 1) {
                // Valid float: write to output file
                fprintf(outputFile, "%f\n", next_value);
            } else {
                // Invalid float: increment counter
                invalidcount++;
            }
            
            // Move to the next token
            token = strtok(NULL, " \n");
        }
    }

    // Check for hardware failure
    if (ferror(inputFile)) {
        printf("Hardware failure during file processing\n");
        invalidcount = -1; // Indicate processing failure
    }

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    // Display results
    printf("Total invalid values found: %d\n", invalidcount);

    return 0;
}