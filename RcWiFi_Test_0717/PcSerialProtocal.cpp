#include "PcSerialProtocal.h"
#include "HardwareSerial.h"


#define PC_SENDPKT_SIZE  128
static char SendPkgBuf[PC_SENDPKT_SIZE] = {0};

int CheckCmdPkg(char* pBuf, int bufLen)
{
    unsigned short* pLeader = (unsigned short*)pBuf;
    if(PC_PKG_HEADER != *pLeader)
    {
        return -1;
    }

    return 0;
}


char* MakeLogPkg(char* pLogBuf, int logLen)
{
    bzero(SendPkgBuf, PC_SENDPKT_SIZE);
    unsigned short* pLeader = (unsigned short*)SendPkgBuf;
    *pLeader = PC_PKG_HEADER;
    char* pC = SendPkgBuf + 2;
    *pC = PC_GET_LOG;
    pC++;
    *pC = '#';
    pC++;
    sprintf(pC, "%s\n", pLogBuf);
    return SendPkgBuf;
}

char* MakeRa0StaPkg(char* pRa0Buf, int Ra0Len)
{
    bzero(SendPkgBuf, PC_SENDPKT_SIZE);
    unsigned short* pLeader = (unsigned short*)SendPkgBuf;
    *pLeader = PC_PKG_HEADER;

    char* pC = SendPkgBuf + 2;
    *pC = PC_GET_INET;
    pC++;
    *pC = '#';
    pC++;
    sprintf(pC, "%s\n", pRa0Buf);
    return SendPkgBuf;
}

char* MakeResultPkg(char* pResultBuf, int ResultLen)
{
    bzero(SendPkgBuf, PC_SENDPKT_SIZE);
    unsigned short* pLeader = (unsigned short*)SendPkgBuf;
    *pLeader = PC_PKG_HEADER;
    char* pC = SendPkgBuf + 2;
    *pC = PC_GET_RESULT;
    pC++;
    *pC = '#';
    pC++;
    sprintf(pC, "%s\n", pResultBuf);
    return SendPkgBuf;
}

char* MakeSsidPkg(char* pSsidBuf, int SsidLen)
{
    bzero(SendPkgBuf, PC_SENDPKT_SIZE);
    unsigned short* pLeader = (unsigned short*)SendPkgBuf;
    *pLeader = PC_PKG_HEADER;

    char* pC = SendPkgBuf + 2;
    *pC = PC_GET_SSID;
    pC++;
    *pC = '#';
    pC++;
    sprintf(pC, "%s\n", pSsidBuf);
    return SendPkgBuf;  
}

int SendPcPkg(char* pBuf, int len)
{
    Serial.printf("%s", pBuf);
    return 0;
}

int SendLogToPc(char* pLog)
{
    char* pSend = MakeLogPkg(pLog, strlen(pLog));
    SendPcPkg(pSend, strlen(pSend));
    return 0;
}

int SendSsidToPc(char* pSsid)
{
    char* pSend = MakeSsidPkg(pSsid, strlen(pSsid));
    SendPcPkg(pSend, strlen(pSend));
    return 0;
}


int SendInetStaToPc(char* pInetSta)
{
    char* pSend = MakeRa0StaPkg(pInetSta, strlen(pInetSta));
    SendPcPkg(pSend, strlen(pSend));
    return 0;
}

int SendTestResultToPC(char* pResult)
{
    char* pSend = MakeResultPkg(pResult, strlen(pResult));
    SendPcPkg(pSend, strlen(pSend));
    return 0;
}





