#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void DemoEnc(char *caInput, char *caOutput) {
    for (int i = 0; caInput[i] != '\0'; i++) {
        caOutput[i] = caInput[i] + 1;
    }
}

void DemoDec(char *caInput, char *caOutput) {
    for (int i = 0; caInput[i] != '\0'; i++) {
        caOutput[i] = caInput[i] - 1;
    }
}

void PrintHex(const char * caLabel, const char * caData, size_t iLength) {
    printf("%s:\n", caLabel);
    for (size_t i = 0; i < iLength; i++) {
        printf("%02X ", (unsigned char)caData[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (iLength % 16 != 0) {
        printf("\n");
    }
}
