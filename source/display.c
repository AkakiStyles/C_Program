#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

int gl_iShmId = 0;
int gl_iSemId = 0;

void Cleanup(int iSigno) {
    printf("\nCleaning up IPC resources...\n");
    shmctl(gl_iShmId, IPC_RMID, NULL);
    RemoveSemaphore(gl_iSemId);
    printf("Resources cleaned up. Exiting.\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    int iLoop = 0;
    int iShmSize = 0;
    /* Read Share data */
    void *psShmData = NULL;
    // int  *piRecord = NULL;
    void *psLocalData = NULL;
    int iLocalRecord = 0;
    struct stDisplayInfo *psDisplayInfo = NULL;

    /* Prepare IPC  */
    iShmSize = GetShmSize();
    gl_iShmId = shmget(IPC_DEMO_SHM_KEY, iShmSize, IPC_CREAT | 0666);
    gl_iSemId = CreateSemaphore();

    psShmData = shmat(gl_iShmId, NULL, 0);
    if (psShmData == NULL) {
        perror("Shared memory attach failed\n");
        return 1;
    }
    // piRecord = pcTmp;
    // *piRecord = 0;
    // psShmData = (struct stDisplayInfo *)(pcTmp + sizeof(int));

    /* Local info buffer */
    psLocalData = malloc(iShmSize);
    if (psLocalData == NULL) {
        perror("Memory allocation failed\n");
        return 1;
    }

    signal(SIGINT, Cleanup);

    while (1) {

        /* Lock with get share data */
        memset( (char *)psLocalData, 0, iShmSize);
        LockSemaphore(gl_iSemId);
        memcpy((char *)psLocalData, (char *)psShmData, iShmSize);
        UnlockSemaphore(gl_iSemId);

        /* get msg count and start of display info struct. */
        iLocalRecord = *((int*)psLocalData);
        psLocalData+=sizeof(int);

        /* print label. */
        system("clear");
        printf("%-4s|%-16s|%-32s|\n", "No", "Client ID", "Result");
        printf("---- ---------------- --------------------------------\n");
        
        for( iLoop = 0; iLoop < iLocalRecord ; iLoop++){
            psDisplayInfo = ((struct stDisplayInfo *)psLocalData) + iLoop;
            printf("%-4d %-16s %-32s\n", iLoop, psDisplayInfo->caClientID, psDisplayInfo->caResult); 
        }

        sleep(1);
    }
}
