#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "demoipc.h"

int ReadInput(char *caBuffer, size_t iSize) {
    int iTmpChar=0;
    if (fgets(caBuffer, iSize, stdin) == NULL) {
        fprintf(stderr, "Error: Input failed.\n");
        return -1;
    }

    size_t stLen = strcspn(caBuffer, "\n");
    if (caBuffer[stLen] == '\n') {
        caBuffer[stLen] = '\0';
    } else {
        while ((iTmpChar = getchar()) != '\n' && iTmpChar != EOF);
    }
    return (int)stLen;
}

int main(int argc, char *argv[]) {
    
    int iRtn = 0;
    /* QueMsg */
    int iMsgId = 0;
    char caLocalClientID[32]={0};
    char caLocalMsg[128]={0};
    char caLocalType[2]={0};
    struct stDemoeMsg sMessage;

    iMsgId = msgget(IPC_DEMO_QUE_KEY, IPC_CREAT | 0666);
    if (iMsgId == -1) {
        perror("msgget failed");
        exit(1);
    }
    sMessage.lMtype = 1;
    /*
    printf("Enter Client ID: ");
    scanf("%31s", caLocalClientID);
    */
    snprintf(caLocalClientID, sizeof(caLocalClientID), "%d", getpid());
    printf(" Client ID: [%.32s] \n",caLocalClientID);
    
    printf("Enter Message: ");
    iRtn = ReadInput(caLocalMsg, sizeof(caLocalMsg));
    if(iRtn<0){
        perror("fail when get Message!");
        exit(1);
    }
    sMessage.iRawMsgSize = iRtn;

    printf("Enter Type (E for Encrypt, D for Decrypt): ");
    iRtn = ReadInput(caLocalType, sizeof(caLocalType));
    if(iRtn<0){
        perror("Enter Type fail! Enter 'E' or 'D' please.");
        exit(1);
    }

    sMessage.cType = toupper(caLocalType[0]);
    if( (sMessage.cType!='E')&&(sMessage.cType!='D') ){
        perror("fail when get work type!");
        exit(1);
    }
    memcpy(sMessage.caClientID, caLocalClientID, sizeof(caLocalClientID));
    memcpy(sMessage.caMsg, caLocalMsg, sizeof(caLocalMsg));


    if (msgsnd(iMsgId, &sMessage, sizeof(struct stDemoeMsg) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Request sent successfully.\n");
}
