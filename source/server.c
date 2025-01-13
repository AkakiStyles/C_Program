#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

int iMsgId = 0;
int iShmId = 0;
int iSemId = 0;

void Cleanup(int iSigno) {
    printf("\nCleaning up IPC resources...\n");
    msgctl(iMsgId, IPC_RMID, NULL);
    shmctl(iShmId, IPC_RMID, NULL);
    RemoveSemaphore(iSemId);
    printf("Resources cleaned up. Exiting.\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    void *pcTmp = NULL;
    int  *piRecord = NULL;
    struct stDisplayInfo *psShmData, *psData;
    char caProcessed[128] = {0};
    int iShmSize = GetShmSize();

    iShmId = shmget(IPC_DEMO_SHM_KEY, iShmSize, IPC_CREAT | 0666);
    iSemId = CreateSemaphore();
    iMsgId = msgget(IPC_DEMO_QUE_KEY, IPC_CREAT | 0666);

    pcTmp = shmat(iShmId, NULL, 0);
    piRecord = pcTmp;
    *piRecord = 0;
    psShmData = (struct stDisplayInfo *)(pcTmp + sizeof(int));

    struct stDemoeMsg sMessage;

    signal(SIGINT, Cleanup);

    while (1) {
        if (msgrcv(iMsgId, &sMessage, sizeof(struct stDemoeMsg) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        if (sMessage.cType == 'E') {
            DemoEnc(sMessage.caMsg, caProcessed);
        } else if (sMessage.cType == 'D') {
            DemoDec(sMessage.caMsg, caProcessed);
        }

        LockSemaphore(iSemId);
        psData = psShmData + *piRecord;
        strncpy(psData->caResult, caProcessed, sizeof(psData->caResult) - 1);
        strncpy(psData->caClientID, sMessage.caClientID, sizeof(psData->caClientID) - 1);
        *piRecord = *piRecord +1;
        UnlockSemaphore(iSemId);
    }

}
