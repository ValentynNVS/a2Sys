/*
*   FILE          : cryptoMagic.h
*   PROJECT       : cryptoMagic - A1
*   PROGRAMMER    : Valentyn, Juan Jose, Warren
*   FIRST VERSION : 01/31/2025
*   DESCRIPTION   :
*      This is the header file where we put all the function prototypes, libraries
*      variable declaration, etc
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef ENCODEINPUT_H
#define ENCODEINPUT_H

// Function to handle command-line arguments
int parseArguments(int argc, char* argv[], char** inputFile, char** outputFile, char** format);

// Function to print usage instructions
void printUsage();

// Function to process the file and convert it based on format
void processFile(FILE *inFile, FILE *outFile, const char *format);  


void writeSRecord(FILE* outFile, unsigned char* buffer, size_t length, size_t address);

void writeAssembly(FILE *outFile, unsigned char *buffer, size_t length);

void writeS0Record(FILE* outFile, const char* name);
void writeS5Record(FILE* outFile, int s1Count);
void writeS9Record(FILE* outFile);

#endif // ENCODEINPUT_H
