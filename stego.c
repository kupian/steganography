//
// Created by Alex Carr on 30/01/2026.
//

#include "stego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encode(char* vesselName) {
    FILE* fd = fopen(vesselName, "r");
    if (fd == NULL) {
        printf("Error opening %s\n", vesselName);
        return 1;
    }

    fseek(fd, 0, SEEK_END);
    const size_t size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char* header = malloc(HEADER_SIZE);
    if (fread(header, 1, HEADER_SIZE, fd) != HEADER_SIZE ) {
        printf("Error reading header\n");
        free(header);
        return 1;
    };

    const size_t imageSize = size-HEADER_SIZE;
    char* image = malloc(imageSize);
    if (fread(image, 1, imageSize, fd) != imageSize) {
        printf("Error reading image\n");
        free(header);
        free(image);
        return 1;
    }

    fclose(fd);

    char* payload = malloc(MAX_PAYLOAD_SIZE);
    printf("Payload: ");
    fgets(payload, MAX_PAYLOAD_SIZE, stdin);

    printf("Payload: ");
    for (size_t i = 0; i < strlen(payload); i++) {
        printf("%02x", payload[i]);
    }
    printf("\n");

    /* INSERT PAYLOAD */
    /* for each byte in payload, computing (BYTE & 0x1) gives 0x1 if the LSB
     * is 1, and 0x0 if the LSB is 0.
     * so we compute this, and overwrite the LSB of a byte in the vessel.
     * then bitshift right by 1 and do the same.
     * repeating this 8 times for each byte allows us to encode the payload byte across
     * bytes in the vessel
     */

    size_t payloadSize = strlen(payload)+1;
    size_t imgOffset = 0;
    for (size_t i = 0; i < payloadSize; i++) {
        for (size_t j = 0; j < 8; j++) {
            /* logical AND then bitshift right */
            if (payload[i] & 0x1) {
                image[imgOffset] = image[imgOffset] | 0x1;
            } else {
                image[imgOffset] = image[imgOffset] & ~0x1;
            }
            payload[i] = payload[i] >> 0x1;
            imgOffset++;
        }
    }


    fd = fopen("out.bmp", "wb");
    if (fd == NULL) {
        printf("Error creating out file\n");
        free(header);
        free(image);
        free(payload);
        return 1;
    }
    if (fwrite(header, 1, HEADER_SIZE, fd) != HEADER_SIZE) {
        printf("Error writing header\n");
        free(header);
        free(image);
        free(payload);
        return 1;
    }
    if (fwrite(image, 1, imageSize, fd) != HEADER_SIZE && ferror(fd)) {
        printf("Error writing image\n");
        free(header);
        free(image);
        free(payload);
        return 1;
    }

    printf("Image written to out.bmp\n");

    fclose(fd);
    free(payload);
    free(header);
    free(image);

    return 0;
}

int decode(char* vesselName) {
    FILE* fd = fopen(vesselName, "r");
    if (fd == NULL) {
        printf("Error opening %s\n", vesselName);
        return 1;
    }

    fseek(fd, 0, SEEK_END);
    const size_t size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char* header = malloc(HEADER_SIZE);
    if (fread(header, 1, HEADER_SIZE, fd) != HEADER_SIZE ) {
        printf("Error reading header\n");
        free(header);
        return 1;
    };

    const size_t imageSize = size-HEADER_SIZE;
    char* image = malloc(imageSize);
    if (fread(image, 1, imageSize, fd) != imageSize) {
        printf("Error reading image\n");
        free(header);
        free(image);
        return 1;
    }

    char* payload = malloc(MAX_PAYLOAD_SIZE);
    memset(payload, 0, MAX_PAYLOAD_SIZE);

    size_t imgOffset = 0;
    for (size_t i = 0; i < MAX_PAYLOAD_SIZE; i++) {
        imgOffset = 8*(i+1) - 1;
        for (size_t j = 0; j < 8; j++) {
            if (image[imgOffset] & 0x1) {
                payload[i] = payload[i] | 0x1;
            } else {
                payload[i] = payload[i] & ~0x1;
            }
            if (j < 7) payload[i] = payload[i] << 0x1;
            imgOffset--;
        }
    }

    fd = fopen("payload.txt", "wb");
    if (fd == NULL) {
        printf("Error creating payload file\n");
        free(header);
        free(image);
        free(payload);
        return 1;
    }
    if (fwrite(payload, 1, MAX_PAYLOAD_SIZE, fd) != MAX_PAYLOAD_SIZE && ferror(fd)) {
        printf("Error writing payload\n");
        free(header);
        free(image);
        free(payload);
        return 1;
    }

    printf("Payload written to payload.txt\n");

    fclose(fd);
    free(header);
    free(image);
    return 0;
}