//
// Created by Alex Carr on 30/01/2026.
//

#ifndef STEGANOGRAPHY_STEGO_H
#define STEGANOGRAPHY_STEGO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int encode(char* vesselName);
int decode(char* vesselName);
char* readImage(FILE* fd, size_t size);
int writeOut(char* filename, char* header, char* data, size_t dataSize);
#define HEADER_SIZE 54
#define MAX_PAYLOAD_SIZE 1024
#endif //STEGANOGRAPHY_STEGO_H