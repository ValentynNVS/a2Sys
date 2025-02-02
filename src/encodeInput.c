/*
*   FILE          : encodeInput.c
*   PROJECT       : encodeInput - A2
*   PROGRAMMER    : Valentyn, Juan Jose, Warren
*   FIRST VERSION : 01/31/2025
*   DESCRIPTION   :
*      This is the main file for encodeInput. It handles command-line arguments,
*      file input processing, and passes data to the appropriate encoding function.
*/


#include "../inc/encodeInput.h"

int main(int argc, char* argv[]) {
    // gotta make sure the user actually enters something useful
    if (argc < 3) {
        printUsage();
        return 1;
    }

    char *inputFile = NULL;
    char* outputFile = NULL;
    char *format = "ASM"; // default format

    if (parseArguments(argc, argv, &inputFile, &outputFile, &format) != 0) {
        return 1; // If there was an error parsing arguments, return early
    }

    // Open input and output files
    FILE* inFile = stdin;
    if (inputFile != NULL) {
        inFile = fopen(inputFile, "rb");
        if (!inFile) {
            printf("Error: Could not open input file '%s'\n", inputFile);
            return 1;
        }
    }

    FILE* outFile = stdout;
    if (outputFile != NULL) {
        outFile = fopen(outputFile, "w");
        if (!outFile) {
            printf("Error: Could not open output file '%s'\n", outputFile);
            if (inFile != stdin) fclose(inFile);
            return 1;
        }
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
  *  Params    : 
 *     int argc, char* argv[], char** inputFile, char** outputFile, char** format
 *  Return    :  
 *     integer.
 */
int parseArguments(int argc, char* argv[], char** inputFile, char** outputFile, char** format) {
    *outputFile = "output.txt";  // Default output file
    *format = "SREC";  // Default format

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-i", 2) == 0) {
            *inputFile = argv[i] + 2;  // Extract filename after -i
        }
        else if (strncmp(argv[i], "-o", 2) == 0) {
            *outputFile = argv[i] + 2;  // Extract filename after -o
        }
        else if (strcmp(argv[i], "-srec") == 0) {
            *format = "SREC";  // Set format to SREC
        }
        else if (strcmp(argv[i], "-asm") == 0) {
            *format = "ASM";  // Set format to ASM
        }
        else if (strcmp(argv[i], "-h") == 0) {
            printUsage();
            return 1;
        }
        else {
            printf("Error: Invalid argument '%s'\n", argv[i]);
            printUsage();
            return 1;
        }
    }

    if (*inputFile != NULL && *outputFile == NULL) {
        *outputFile = malloc(strlen(*inputFile) + 5);
        strcpy(*outputFile, *inputFile);
        if (strcmp(*format, "SREC") == 0) {
            strcat(*outputFile, ".srec");
        }
        else {
            strcat(*outputFile, ".asm");
        }
    }

    return 0;
}

/*
 *  Function  : processFile()
 *  Summary   : This function reads binary file and writes either an S-Record or Assembly output
 *  Params    :
 *     FILE *inFile, FILE *outFile, const char *format
 *  Return    :
 *     none.
 */
void processFile(FILE *inFile, FILE *outFile, const char *format) {
    unsigned char buffer[16];  // Buffer to hold bytes we're reading from the input file
    size_t bytesRead;  // Keep track of how much data we’ve read
    size_t address = 0;
    int s1Count = 0;

    if (strcmp(format, "SREC") == 0) {
        // Write S0 record (header)
        writeS0Record(outFile, "VAL-JUAN-WAR");

        // Process data and write S1 records
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), inFile)) > 0) {
            writeSRecord(outFile, buffer, bytesRead, address);
            address += bytesRead; // Increment address
            s1Count++; // Increment S1 record count
        }

        // Write S5 and S9 records (footer)
        writeS5Record(outFile, s1Count);
        writeS9Record(outFile);
    }
    else {
        // Process data and write Assembly records
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), inFile)) > 0) {
            writeAssembly(outFile, buffer, bytesRead);
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
    printf("  -h             Display this help message\n");
}
