#ifndef _DEMO_SYSV_IPC_HEADER
#define _DEMO_SYSV_IPC_HEADER

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <pthread.h>

#define IPC_DEMO_QUE_KEY 1234
#define IPC_DEMO_SHM_KEY 5678
#define IPC_DEMO_SEM_KEY 91011

struct stDemoeMsg {
    long lMtype;
    char caClientID[32];
    char caMsg[128];
    char cType; /* 'E' for encryption, 'D' for decryption */
};

struct stDisplayInfo {
    char caClientID[32];
    char caResult[128];
} stdef_DisplayInfo;

/* function protype for libtool */
void Cleanup(int iSigno);
void DemoEnc(char *caInput, char *caOutput); 
void DemoDec(char *caInput, char *caOutput); 
void PrintHex(const char * caLabel, const char * caData, size_t iLength);

/* function protype for libipc */
int GetShmSize();
int CreateSemaphore();
void LockSemaphore(int iSemId);
void UnlockSemaphore(int iSemId);
void RemoveSemaphore(int iSemId);

#endif /* _DEMO_SYSV_IPC_HEADER End */
