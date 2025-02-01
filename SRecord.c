#include "encodeInput.h"

void writeSRecord(FILE *outFile, unsigned char *buffer, size_t length) {
    
    unsigned char checksum = 0;
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

        for (size_t i = 0; i < chunckSize; i++) { // this is a pretty simple for loop
            unsigned char dataByte = buffer[index + i]; // this line accesses a specific byte from the buffer and uses index + i to get the current bytes position
            fprintf(outFile, "%02X", dataByte);// Im using this so I can write the value of dataByte to the output file (outFile) in an hexadecimal format

            //and here I will apply an algorithm that I found on this website https://www.ibm.com/docs/en/zos/2.4.0?topic=calculations-checksum-algorithm 
            if (i % 2 == 0) {
                P1 += dataByte;
                P1 &= 0xFF;
            }
            else {
                P2 += dataByte;
                P2 &= 0xFF;
            }
            
        }

        fprintf(outFile, "%02X%02X\n", P1, P2);// what this line does is that first it writes formatted output to the outFile file pointer, then it writes the values of P1 and P2
                                               // how the %02X works is really simple the 0 is so if the number is less than 2 digits then the first number is 0 the 2 is so it reserves 
                                               // 2 digits for the value and the X prints the value as a hexadecimal number

        index += chunckSize;    // in this part I just move to the next chunk
        address += 16;          // and here I increment the address by 16 (the value of S1)
                            
    }



}