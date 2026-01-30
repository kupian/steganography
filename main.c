#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "stego.h"

int main(int argc, char* argv[]) {
    /* -- VALIDATE ARGS AND READ IN BITMAP */
    /* first 54 bytes are header and info - don't change them */


    if (argc != 3) {
        printf("Usage: ./steganography -<e/d> <vessel.bmp>\n");
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        return encode(argv[2]);
    }
    else if (strcmp(argv[1], "-d") == 0) {
        return decode(argv[2]);
    }

    return 1;
}
