/*
*   FILE          : SRecord.c
*   PROJECT       : encodeInput - A2
*   PROGRAMMER    : Valentyn, Juan Jose, Lukas, Warren
*   FIRST VERSION : 01/31/2025
*   DESCRIPTION   :
*      This is the file where all the functionality for SRecord files is implemented.
*/

#include "../inc/encodeInput.h"

/*
 *  Function  : writeSRecord()
 *  Summary   : This function writes an S-Record formatted line from binary input file.
 *  Params    :
 *     FILE* outFile, unsigned char* buffer, size_t length, size_t address
 *  Return    :
 *     none.
 */
void writeSRecord(FILE* outFile, unsigned char* buffer, size_t length, size_t address) {

    size_t index = 0;

    while (index < length) {
        size_t chunkSize = (length - index >= 16) ? 16 : (length - index); // Max 16 bytes per S1 record
        unsigned char count = chunkSize + 3; // Count includes address (2 bytes) and checksum (1 byte)
        fprintf(outFile, "S1%02X%04X", count, (unsigned int)address); // Write S1 record header

        unsigned char checksum = count + (address >> 8) + (address & 0xFF); // Add count and address to checksum
        for (size_t i = 0; i < chunkSize; i++) {
            unsigned char dataByte = buffer[index + i];
            fprintf(outFile, "%02X", dataByte); // Write data in hex
            checksum += dataByte; // Add data bytes to checksum
        }
        checksum = ~checksum & 0xFF; // Calculate checksum
        fprintf(outFile, "%02X\n", checksum); // Write checksum

        index += chunkSize; // Move to next chunk
        address += chunkSize; // Increment address
    }
}

/*
 *  Function  : writeS0Record()
 *  Summary   : This function writes the S0 header record to the output file.
 *  Params    :
 *     FILE* outFile, const char* name
 *  Return    :
 *     none.
 */
void writeS0Record(FILE* outFile, const char* name) {
    unsigned char checksum = 0;
    fprintf(outFile, "S0%02X%04X", (unsigned int)(strlen(name) + 3), 0); // S0 record with name
    checksum += (strlen(name) + 3) + 0 + 0; // Add count and address to checksum
    for (size_t i = 0; i < strlen(name); i++) {
        fprintf(outFile, "%02X", name[i]); // Write name in hex
        checksum += name[i]; // Add name bytes to checksum
    }
    checksum = ~checksum & 0xFF; // Calculate checksum
    fprintf(outFile, "%02X\n", checksum); // Write checksum
}

/*
 *  Function  : writeS5Record()
 *  Summary   : This function writes the S5 count record to the output file.
 *  Params    :
 *     FILE* outFile, int s1Count
 *  Return    :
 *     none.
 */
void writeS5Record(FILE* outFile, int s1Count) {
    unsigned char checksum = 3 + (s1Count >> 8) + (s1Count & 0xFF);
    checksum = ~checksum & 0xFF; // Calculate checksum
    fprintf(outFile, "S503%04X%02X\n", s1Count, checksum); // Write S5 record
}

/*
 *  Function  : writeS9Record()
 *  Summary   : This function writes the S9 record to the output file.
 *  Params    :
 *     FILE* outFile
 *  Return    :
 *     none.
 */
void writeS9Record(FILE* outFile) {
    unsigned char checksum = 3 + 0 + 0;
    checksum = ~checksum & 0xFF; // Calculate checksum
    fprintf(outFile, "S9030000%02X\n", checksum); // Write S9 record
}