#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

void DemoEnc(char *caInput, int iInputSize, char *caOutput, int iOutputSize) {
    if (iOutputSize < iInputSize) {
        fprintf(stderr, "Error: Output buffer too small\n");
        return;
    }

#ifdef IPCDEMO_DEBUG_FLAG
    PrintHex("Before Encrypt:\n", 16, caInput, iInputSize);
#endif

    for (int i = 0; i < iInputSize; i++) {
        caOutput[i] = caInput[i] + 1;
    }

#ifdef IPCDEMO_DEBUG_FLAG
    PrintHex("After Encrypt:\n", 15, caOutput, iOutputSize);
#endif
}

void DemoDec(char *caInput, int iInputSize, char *caOutput, int iOutputSize) {
    if (iOutputSize < iInputSize) {
        fprintf(stderr, "Error: Output buffer too small\n");
        return;
    }

#ifdef IPCDEMO_DEBUG_FLAG
    PrintHex("Before Decrypt:\n", 16, caInput, iInputSize);
#endif
    for (int i = 0; i < iInputSize; i++) {
        caOutput[i] = caInput[i] - 1;
    }
#ifdef IPCDEMO_DEBUG_FLAG
    PrintHex("After Decrypt:\n", 15, caOutput, iOutputSize);
#endif
}

void PrintHex(const char * caLabel, int iLabelSize, const char * caData, int iDataSize) {
    if (iLabelSize > 0)
        /* Although you can use %.*s, some code scan programs don't like you to do this. */
        printf("%.20s", caLabel);
    
    if (iDataSize < 0)
        printf("DataSize=%d, Nothing to do.\n",iDataSize);

    for (int i = 0; i < iDataSize; i++) {
        printf("%02X ", (unsigned char)caData[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");

}
