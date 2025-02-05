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
#define IPC_DEMO_ITEM_NUM 50

struct stDemoeMsg {
    long lMtype;
    char caClientID[32];
    char caMsg[128];
    int iRawMsgSize;
    char cType; /* 'E' for encryption, 'D' for decryption */
};

struct stDisplayInfo {
    char caClientID[32];
    char caResult[128];
} stdef_DisplayInfo;

/* function protype for libtool */
void Cleanup(int iSigno);
void DemoEnc(char *caInput, int iInputSize, char *caOutput, int iOutputSize); 
void DemoDec(char *caInput, int iInputSize, char *caOutput, int iOutputSize);
/* Debug function */ 
void PrintHex(const char * caLabel, int iLabelSize, const char * caData, int iDataSize);

/* function protype for libipc */
int GetShmSize();
int CreateSemaphore();
void LockSemaphore(int iSemId);
void UnlockSemaphore(int iSemId);
void RemoveSemaphore(int iSemId);

#endif /* _DEMO_SYSV_IPC_HEADER End */
