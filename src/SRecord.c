/*
*   FILE          : cryptoMagic.h
*   PROJECT       : cryptoMagic - A1
*   PROGRAMMER    : Valentyn, Juan Jose, Lukas, Warren
*   FIRST VERSION : 01/31/2025
*   DESCRIPTION   :
*      This is the file where all the functionality for SRecord files is implemented.
*/

#include "encodeInput.h"

/*
 *  Function  : writeSRecord()
 *  Summary   : This function writes an S-Record formatted line from binary input file.
 *  Params    :
 *     FILE *outFile, unsigned char *buffer, size_t length
 *  Return    :
 *     none.
 */
void writeSRecord(FILE *outFile, unsigned char *buffer, size_t length) {
    
    size_t address = 0; // this would be a placeholder for the address
    size_t index = 0; // this is the index which will help me to make the checksum later

    while (index < length) {
        size_t chunckSize = (length - index >= 16) ? 16 : (length - index); // this line what does isthat it determines the number of bytes (the chunkSize) to be written in the S-Record,
                                                                            // the maximum here is 16 because we are using maximum S1 that would be 16 bytes, If its more than 16 or equal
        unsigned char count = chunckSize + 3; // This counts the address (2 bytes) + Data + Checksum (1 byte) [it is +3 because of the addition of what we already know
                                              // the number of bytes for the address and the checksum]
        unsigned char P1 = 0x00;// Here i initialize two variables that will help me to calculate teh checksum for the S-Record
                                // P1 stores the cumulative checksum of the odd-indexed data bytes
        unsigned char P2 = 0x00;// and P2 stores the cumulative checksum of the even-indexed data bytes

        fprintf(outFile, "S1%02X%04X", count, (unsigned int)address); // what this line does is that it uses a format("S1%02X%04X") 
                                                                      // and writes a part of the S-Record to the output file
                                                                      // S1 has 2 digit hexadecimal(including total number of bytes including address and checksum)this would be the (%02X)
                                                                      // and the addres that would be the 4 digit hexadecimal number (%04X)

        unsigned char checksum = countField;  // Here I moved the initialization of the Checksum
        checksum += (address >> 8) & 0xFF;    // here is were I added the high byte of the address
        checksum += address & 0xFF;           // And here is where I add the low byte of the address

        for (size_t i = 0; i < chunkSize; i++) {
            unsigned char dataByte = buffer[index + i]; // In this line what I do is that in each extracted byte (dataByte) is then written to the output file and added to the checksum
            fprintf(outFile, "%02X", dataByte); // this line writes data in hex format

            checksum += dataByte;  // This line adds to checksum
        }

        // 1's complement (this means flipping all the bytes)
        checksum = ~checksum & 0xFF;

        // IT writes the checksum to the file
        fprintf(outFile, "%02X\n", checksum);


        fprintf(outFile, "%02X%02X\n", P1, P2);// what this line does is that first it writes formatted output to the outFile file pointer, then it writes the values of P1 and P2
                                               // how the %02X works is really simple the 0 is so if the number is less than 2 digits then the first number is 0 the 2 is so it reserves 
                                               // 2 digits for the value and the X prints the value as a hexadecimal number

        index += chunckSize;    // in this part I just move to the next chunk
        address += 16;          // and here I increment the address by 16 (the value of S1)
                            
    }



}