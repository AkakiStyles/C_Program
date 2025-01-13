#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

int main(int argc, char *argv[]) {
    int iMsgId = 0;
    char caLocalClientID[32]={0};
    char caLocalMsg[128]={0};
    char cLocalType;
    struct stDemoeMsg sMessage;

    iMsgId = msgget(IPC_DEMO_QUE_KEY, IPC_CREAT | 0666);
    if (iMsgId == -1) {
        perror("msgget failed");
        exit(1);
    }
    sMessage.lMtype = 1;

    printf("Enter Client ID: ");
    scanf("%31s", caLocalClientID);
    printf("Enter Message: ");
    scanf("%127s", caLocalMsg);
    printf("Enter Type (E for Encrypt, D for Decrypt): ");
    scanf(" %c", &cLocalType);

    memcpy(sMessage.caClientID, caLocalClientID, sizeof(caLocalClientID));
    memcpy(sMessage.caMsg, caLocalMsg, sizeof(caLocalMsg));
    sMessage.cType = cLocalType;

    if (msgsnd(iMsgId, &sMessage, sizeof(struct stDemoeMsg) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Request sent successfully.\n");
}
