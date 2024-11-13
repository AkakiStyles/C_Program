#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include "sysvipc.h"
#include "sysvque.h"
#include <math.h>

static int s_iQueueId = -1;
int InitSysVQue(){
    char caLogMsg[500]={0};

    if( s_iQueueId < 0 ){
        s_iQueueId = msgget(_DEMO_QUE_KEY, IPC_CREAT | 0666);
        if (s_iQueueId < 0) {
            snprintf(caLogMsg,sizeof(caLogMsg),
                    "%s%s()fail! can't create sysv queuu id.\n",
                    LOG_LEVEL_ERROR,__FUNCTION__);
            perror(caLogMsg);
            s_iQueueId = -1;
            return (IPC_CREATE_QUE_FAIL);
        }
    }
    return (SUCC);
}

int CloseSysVQue(){
    char caLogMsg[500]={0};

    if( s_iQueueId > 0 ){
        if (msgctl(s_iQueueId, IPC_RMID, NULL) < 0) {
            snprintf(caLogMsg,sizeof(caLogMsg),
                    "%s%s()fail! can't delete sysv queuu id.\n",
                    LOG_LEVEL_ERROR,__FUNCTION__);
            perror(caLogMsg);
            return (FAIL);
        }
    }
    return (SUCC);
}

int SendSysVQue(long lType, char* pcMsg, int iMsgLen){
    int iResult = 0;
    int iCopyLen = 0;
    char caLogMsg[500]={0};
    stQueueBuffer st_qBuf;
//    int iLogCount = 0 ;

//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    iResult = InitSysVQue();
    if( iResult < 0 ){
        snprintf(caLogMsg,sizeof(caLogMsg),
                "%s%s() fail! Result code = %d\n",
                LOG_LEVEL_ERROR,__FUNCTION__, iResult);
        perror(caLogMsg);
        return (IPC_CREATE_QUE_FAIL);
    }

//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    memset((char*) &st_qBuf, 0, sizeof(stQueueBuffer));
    st_qBuf.lType = lType;
    iCopyLen = fmin( _DEMO_QUEBUFFER_SIZE, iMsgLen);
    memcpy((char*)&st_qBuf.caMsg, pcMsg, iCopyLen);

//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    iResult = msgsnd( s_iQueueId, &st_qBuf, _DEMO_QUEBUFFER_SIZE, 0); 
    if (iResult < 0) {
        snprintf(caLogMsg,sizeof(caLogMsg),
                "%s%s() Send message fail! Result code = %d",
                LOG_LEVEL_ERROR,__FUNCTION__, iResult);
        perror(caLogMsg);
        return(iResult);
    }
//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    return (SUCC);
}

int ReceiveSysVQue(long lType, char* pcBuffer, int iBufSize){
    int iResult = 0;
    int iCopyLen = 0;
    char caLogMsg[500]={0};
    stQueueBuffer st_qBuf;
//    int iLogCount = 0 ;

//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    iResult = InitSysVQue();
    if( iResult < 0 ){
        snprintf(caLogMsg,sizeof(caLogMsg),
                "%s%s() fail! Result code = %d\n",
                LOG_LEVEL_ERROR,__FUNCTION__, iResult);
        perror(caLogMsg);
        return (IPC_CREATE_QUE_FAIL);
    }

//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    iResult = msgrcv( s_iQueueId, &st_qBuf, _DEMO_QUEBUFFER_SIZE, lType, 0);
    if (iResult < 0) {
        snprintf(caLogMsg,sizeof(caLogMsg),
                "%s%s() Receive fail! Result code = %d\n",
                LOG_LEVEL_ERROR,__FUNCTION__, iResult);
        perror(caLogMsg);
        return(iResult);
    }
    iCopyLen = fmin( iBufSize, sizeof(st_qBuf.caMsg));
    memcpy(pcBuffer, (char *)st_qBuf.caMsg, iCopyLen);
//    printf("%s() step %d\n",__FUNCTION__, iLogCount++);
    return (SUCC);
}
