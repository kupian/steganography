//
// Created by Alex Carr on 30/01/2026.
//

#include "stego.h"

size_t getSize(FILE* fd) {
    fseek(fd, 0, SEEK_END);
    const size_t size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    return size;
}

char* readHeader(FILE* fd) {
    char* header = malloc(HEADER_SIZE);
    if (fread(header, 1, HEADER_SIZE, fd) != HEADER_SIZE ) {
        printf("Error reading header\n");
        return NULL;
    };
    return header;
}

char* readImage(FILE* fd, size_t size) {
    const size_t imageSize = size-HEADER_SIZE;
    char* image = malloc(imageSize);
    if (fread(image, 1, imageSize, fd) != imageSize) {
        printf("Error reading image\n");
        return NULL;
    }
    return image;
}

int writeOut(char* filename, char* header, char* data, size_t dataSize) {
    FILE* fd = fopen(filename, "wb");
    if (fd == NULL) {
        printf("Error creating out file\n");
        return 1;
    }
    if (header) {
        if (fwrite(header, 1, HEADER_SIZE, fd) != HEADER_SIZE) {
            printf("Error writing header\n");
            return 1;
        }
    }
    if (fwrite(data, 1, dataSize, fd) != dataSize && ferror(fd)) {
        printf("Error writing data\n");
        return 1;
    }
    return 0;
}

int encode(char* vesselName) {
    FILE* fd = fopen(vesselName, "r");
    if (fd == NULL) {
        printf("Error opening %s\n", vesselName);
        return 1;
    }

    const size_t size = getSize(fd);
    char* header = readHeader(fd);

    const size_t imageSize = size-HEADER_SIZE;
    char* image = readImage(fd, imageSize);

    fclose(fd);

    char* payload = malloc(MAX_PAYLOAD_SIZE);
    printf("Payload: ");
    fgets(payload, MAX_PAYLOAD_SIZE, stdin);

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


    if (writeOut("out.bmp", header, image, imageSize)) {
        printf("Error writing out.bmp\n");
        return 1;
    };

    printf("Image written to out.bmp\n");

    fclose(fd);
    return 0;
}

int decode(char* vesselName) {
    FILE* fd = fopen(vesselName, "r");
    if (fd == NULL) {
        printf("Error opening %s\n", vesselName);
        return 1;
    }

    const size_t size = getSize(fd);
    char* header = readHeader(fd);

    const size_t imageSize = size-HEADER_SIZE;
    char* image = readImage(fd, imageSize);

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

    if (writeOut("payload.txt", NULL, payload, MAX_PAYLOAD_SIZE)) {
        printf("Error writing payload.txt\n");
        return 1;
    }

    printf("Payload written to payload.txt\n");

    fclose(fd);
    return 0;
}