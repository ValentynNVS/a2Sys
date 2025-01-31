/*
*   FILE          : encodeInput.c
*   PROJECT       : encodeInput - A2
*   PROGRAMMER    : Valentyn, Juan Jose, Warren
*   FIRST VERSION : 01/31/2025
*   DESCRIPTION   :
*      This is the main file for encodeInput. It handles command-line arguments,
*      file input processing, and passes data to the appropriate encoding function.
*/


#include "encodeInput.h"

int main(int argc, char* argv[]) {
    // gotta make sure the user actually enters something useful
    if (argc < 3) {
        printUsage();
        return 1;
    }

    char *inputFile = NULL;
    char *outputFile = "output.txt"; // default output file
    char *format = "SREC"; // default format

    if (parseArguments(argc, argv, &inputFile, &outputFile, &format) != 0) {
        return 1; // If there was an error parsing arguments, return early
    }

    // checking if we actually got an input file
    if (inputFile == NULL) {
        printf("Error: No input file specified\n");
        printUsage();
        return 1;
    }

    // making sure format is something we actually support
    if (strcmp(format, "SREC") != 0 && strcmp(format, "ASM") != 0) {
        printf("Error: Invalid format '%s'. Use 'SREC' or 'ASM'.\n", format);
        return 1;
    }

    // Open input and output files
    FILE *inFile = fopen(inputFile, "rb");
    if (!inFile) {
        printf("Error: Could not open input file '%s'\n", inputFile);
        return 1;
    }

    FILE *outFile = fopen(outputFile, "w");
    if (!outFile) {
        printf("Error: Could not open output file '%s'\n", outputFile);
        fclose(inFile);
        return 1;
    }

    // now this is the part for you guys, it should be 
    // implemented ideally in the other file, try to make our code modular
    processFile(inFile, outFile, format);


        
    if (fclose(inFile) != 0) {
        printf("Error: Could not close input file '%s'\n", inputFile);
        return 1;
    }

    // Close output file with error checking
    if (fclose(outFile) != 0) {
        printf("Error: Could not close output file '%s'\n", outputFile);
        return 1;
    }
    return 0;
}



/*
 *  Function  : parseArguments()
 *  Summary   : Parses command-line arguments and assigns values accordingly.
 */
int parseArguments(int argc, char* argv[], char** inputFile, char** outputFile, char** format) {
    *outputFile = "output.txt";  // Default output file
    *format = "SREC";  // Default format

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            *inputFile = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            *outputFile = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            *format = argv[++i];
        } else {
            printf("Error: Invalid argument '%s'\n", argv[i]);
            printUsage();
            return 1;
        }
    }

    if (!*inputFile) {
        printf("Error: No input file provided.\n");
        printUsage();
        return 1;
    }

    return 0;
}

void processFile(FILE *inFile, FILE *outFile, const char *format) {
    unsigned char buffer[16];  // Buffer to hold bytes we're reading from the input file
    size_t bytesRead;  // Keep track of how much data we’ve read

    // So, we’re reading chunks of 16 bytes at a time from the input file
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inFile)) > 0) {
        // If we've successfully read data, we need to process it
        // Now check what format the user wants: SREC or ASM
        if (strcmp(format, "SREC") == 0) {
            // For S-Record format, pass the buffer to the appropriate function
            writeSRecord(outFile, buffer, bytesRead);
        } else if (strcmp(format, "ASM") == 0) {
            // For ASM format, pass the buffer to the corresponding function
            writeAssembly(outFile, buffer, bytesRead);
        } else {
            // If format isn't supported, tell the user and stop processing
            printf("Error: Unsupported format '%s'\n", format);
            return;
        }
    }

    // We don't need to do anything if bytesRead is 0 because that means we're done reading
    // But, in case the loop exits early (i.e., no data was read), we could add some error handling
    if (bytesRead == 0 && ferror(inFile)) {
        // If there was an error while reading, we should let the user know
        printf("Error: There was an issue reading the input file.\n");
        return;
    }
}


/*
 *  Function  : printfUsage()
 *  Summary   : Displays usage instructions for the cryptoMagic utility.
 *  Params    : 
 *     none.
 *  Return    :  
 *     none.
 */
void printUsage() {
    printf("Usage: encodeInput -i <inputFile> [-o <outputFile>] [-f <format>]\n");
    printf("  -i <inputFile>   Specify the input file\n");
    printf("  -o <outputFile>  (Optional) Specify the output file\n");
    printf("  -f <format>      (Optional) Choose format: 'srec' (default) or 'asm'\n");
}
