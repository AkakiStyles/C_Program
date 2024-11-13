#ifndef _DEMO_SYSV_IPC_QUEUE
#define _DEMO_SYSV_IPC_QUEUE

int InitSysVQue();
int CloseSysVQue();
int SendSysVQue(long lType, char* pcMsg, int iMsgLen);
int ReceiveSysVQue(long lType, char* pcBuffer, int iBufSize);

#endif
