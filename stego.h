//
// Created by Alex Carr on 30/01/2026.
//

#ifndef STEGANOGRAPHY_STEGO_H
#define STEGANOGRAPHY_STEGO_H
int encode(char* vesselName);
int decode(char* vesselName);
#define HEADER_SIZE 54
#define MAX_PAYLOAD_SIZE 1024
#endif //STEGANOGRAPHY_STEGO_H