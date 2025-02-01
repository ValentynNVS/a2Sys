#include "encodeInput.h"

/*
 *  Function  : writeAssembly()
 *  Summary   : Converts binary data into Assembly format and writes to file.
 */
void writeAssembly(FILE *outFile, unsigned char *buffer, size_t length) {
    // TODO: Implement conversion logic
    size_t index = 0;
    while (index < length) {
        fprintf(outFile, "dc.b\t");
        for (size_t i = 0; i < 16 && index < length; i++, index++) {
            fprintf(outFile, "$%02X%s", buffer[index], (i < 15 && index < length - 1) ? ", " : "");
        }
        fprintf(outFile, "\n");
    }    
}