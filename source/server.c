#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

int gl_iMsgId = 0;
int gl_iShmId = 0;
int gl_iSemId = 0;

void Cleanup(int iSigno) {
    printf("\nCleaning up IPC resources...\n");
    msgctl(gl_iMsgId, IPC_RMID, NULL);
    shmctl(gl_iShmId, IPC_RMID, NULL);
    RemoveSemaphore(gl_iSemId);
    printf("Resources cleaned up. Exiting.\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    /* Shm */
    void *pcTmp = NULL;
    int  *piRecord = NULL;
    struct stDisplayInfo *psShmData, *psData;
    int iShmSize = GetShmSize();

    /* QueMsg */
    struct stDemoeMsg sMessage;
    int iDefaultQueMsgSize = sizeof(struct stDemoeMsg) - sizeof(long);

    /* DecEnc */
    int iInputSize = sizeof(sMessage.caMsg);
    char caProcessed[128] = {0};
    int iOutPutSize = sizeof(caProcessed);

    gl_iShmId = shmget(IPC_DEMO_SHM_KEY, iShmSize, IPC_CREAT | 0666);
    gl_iSemId = CreateSemaphore();
    gl_iMsgId = msgget(IPC_DEMO_QUE_KEY, IPC_CREAT | 0666);

    pcTmp = shmat(gl_iShmId, NULL, 0);
    piRecord = pcTmp;
    *piRecord = 0;
    psShmData = (struct stDisplayInfo *)(pcTmp + sizeof(int));

    signal(SIGINT, Cleanup);

    while (1) {
        /* clean buffer */
        memset(caProcessed,0,iOutPutSize);

        if (msgrcv(gl_iMsgId, &sMessage, iDefaultQueMsgSize, 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        if (sMessage.cType == 'E') {
            DemoEnc(sMessage.caMsg, sMessage.iRawMsgSize, caProcessed, iOutPutSize);
        } else if (sMessage.cType == 'D') {
            DemoDec(sMessage.caMsg, sMessage.iRawMsgSize, caProcessed, iOutPutSize);
        }

        LockSemaphore(gl_iSemId);
        psData = psShmData + *piRecord;
        strncpy(psData->caResult, caProcessed, sizeof(psData->caResult) - 1);
        strncpy(psData->caClientID, sMessage.caClientID, sizeof(psData->caClientID) - 1);
        *piRecord = *piRecord +1;
        UnlockSemaphore(gl_iSemId);
    }

}
