#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "demoipc.h"

int GetShmSize() {
    return (sizeof(int) + sizeof(stdef_DisplayInfo) * IPC_DEMO_ITEM_NUM);    
}

int CreateSemaphore() {
    int iSemId = semget( IPC_DEMO_SEM_KEY, 1, IPC_CREAT | 0666);
    if (iSemId == -1) {
        perror("semget failed");
        exit(1);
    }

    if (semctl(iSemId, 0, SETVAL, 1) == -1) {
        perror("semctl failed");
        exit(1);
    }

    return iSemId;
}

void LockSemaphore(int iSemId) {
    struct sembuf sBuf;
    sBuf.sem_num = 0;
    sBuf.sem_op = -1; 
    sBuf.sem_flg = 0;

    if (semop(iSemId, &sBuf, 1) == -1) {
        perror("semop lock failed");
        exit(1);
    }
}

void UnlockSemaphore(int iSemId) {
    struct sembuf sBuf;
    sBuf.sem_num = 0;
    sBuf.sem_op = 1; 
    sBuf.sem_flg = 0;

    if (semop(iSemId, &sBuf, 1) == -1) {
        perror("semop unlock failed");
        exit(1);
    }
}

void RemoveSemaphore(int iSemId) {
    if (semctl(iSemId, 0, IPC_RMID) == -1) {
        perror("semctl remove failed");
        exit(1);
    }
}

