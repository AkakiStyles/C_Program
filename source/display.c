#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

int iShmId = 0;
int iSemId = 0;
int iShmSize = 0;

void Cleanup(int iSigno) {
    printf("\nCleaning up IPC resources...\n");
    shmctl(iShmId, IPC_RMID, NULL);
    RemoveSemaphore(iSemId);
    printf("Resources cleaned up. Exiting.\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    int iLoop = 0;
    void *pcTmp = NULL;
    int  *piRecord = NULL;
    struct stDisplayInfo *psShmData;

    iShmSize = GetShmSize();
    iShmId = shmget(IPC_DEMO_SHM_KEY, iShmSize, IPC_CREAT | 0666);
    iSemId = CreateSemaphore();

    pcTmp = shmat(iShmId, NULL, 0);
    piRecord = pcTmp;
    *piRecord = 0;
    psShmData = (struct stDisplayInfo *)(pcTmp + sizeof(int));

    signal(SIGINT, Cleanup);

    while (1) {
        LockSemaphore(iSemId);
        system("clear");
        printf("%-4s|%-16s|%-32s|\n", "No", "Client ID", "Result");
        printf("---- ---------------- --------------------------------\n");
        
        for( iLoop = 0; iLoop < *piRecord ; iLoop++){
           printf("%-4d %-16s %-32s\n", iLoop, (psShmData+iLoop)->caClientID, (psShmData+iLoop)->caResult); 
        }
        UnlockSemaphore(iSemId);
        sleep(1);
    }
}
