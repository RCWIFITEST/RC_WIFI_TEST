#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "RcSerialProtocal.h"
#include "HardwareSerial.h"
#include "PcSerialProtocal.h"
#include "StateMachine.h"

extern HardwareSerial SerialRidecam;

static char* ridecamLog[]
{
    "(none) login:",                                      //RCLOG_SYSTEM_LOGIN
    "Password:",                                          //RCLOG_SYSTEM_PASSWD
    "~ #",                                                //RCLOG_SYSTEM_SHELL
    "ERROR!!! MT7601_BBP_read.",                          //RCLOG_PROBLEM_LOG
    "Wait for command response timeout(300ms)",           //RELOG_CHECK_RESTART
    "AUTHMODE=WPA2PSK, ENCRYPT=AES,APSSID=",              //RCLOG_SSID_PASSWD
    "config AP successful",                               //RCLOG_AP_STARTED
    "udhcpd (v1.20.2) started",                           //RCLOG_DHCPD_STARTED,
    "          inet addr:192.168.1.202",                  //RCLOG_RA0_INET
    "Bus 001 Device 002: ID 148f:7601",                   //RCLOG_CHECK_7601   
};

int DoCmdInputUsername()
{
    SerialRidecam.printf("root\r");
    return 0;  
}

int DoCmdInputPassword()
{
    SerialRidecam.printf("l150626\r");
    return 0;  
}

int DoCmdGetInetStatus()
{
    SerialRidecam.printf("ifconfig |grep inet\r");
    return 0;
}

int DoCmdCheck7601()
{   
    SerialRidecam.printf("lsusb\r");
    return 0;
}

int DoCmdCheckRa0Stat()
{
     SerialRidecam.printf("iwpriv ra0 stat\r");
}

int DoCmdCheckUdhcpd()
{
    SerialRidecam.printf("ps -e |grep udhcpd\r");
}

static char rcSsid[64] = {0};
char* SaveRidecamSSID(char* pStr)
{
    if(NULL == pStr)
    {
        return NULL;
    }

    //AUTHMODE=WPA2PSK, ENCRYPT=AES,APSSID=RIDECAM_003518CC000000D7,PASSWORD=78932100
    char* pSsid = NULL;
    pSsid = strstr(pStr, ",APSSID=");
    if(pSsid)
    {
        bzero(rcSsid, sizeof(rcSsid));
        memcpy(rcSsid, pSsid + 8, 24);
        //Serial.println(rcSsid);
        SendSsidToPc(rcSsid);
        return rcSsid;
    }

    return NULL;
}

char* GetRidecamSSID()
{
    if(rcSsid[0] != 0)
    {
        return rcSsid;
    }

    return NULL;
}
int ClearRidecamSSID()
{
    bzero(rcSsid, sizeof(rcSsid));
    return 0;
}
#if 1
char* checkRestart(char* pBuf)
{
    if(NULL == pBuf)
    {
        return NULL;
    }
    char* pInet = NULL;
    pInet = strstr(pBuf,"[netmng] ra0 invalid, ");
    if(pInet)
    {
       Serial.println(pInet);
    }
    else
    {
      Serial.println("....");
    }
    return NULL;
}
#endif 
int ParseRidecamSerialLog(RCLOG_TYPE_t logType, char* pBuf, int bufLen)
{
    if(NULL == pBuf || 0 > bufLen)
    {
        return -1;
    }
    switch(logType)
    {
        case RCLOG_SYSTEM_LOGIN:
        {
            SetRcBootStep(RC_BSTEP_LOGIN);
            DoCmdInputUsername();
            //Serial.println("Input uname Successful");
            break;
        }
        case RCLOG_SYSTEM_PASSWD:
        {
            DoCmdInputPassword();
            //Serial.println("Input pwd Successful");
            break;
        }
        case RCLOG_SYSTEM_SHELL:
        {
            //Serial.println("login Successful");
            break;
        }
        case RCLOG_PROBLEM_LOG:
        {
            Serial.println("RTUSB_VendorRequest failed");
            SetRcBootStep(RC_BSTEP_RESTART);
            //SetTbState(TB_STATE_STOP);  
            break;
        }
        case RELOG_CHECK_RESTART:
        {
            Serial.println("SYSTEM RESTART");
            //checkRestart(pBuf);
            break;
        }
        case RCLOG_SSID_PASSWD:
        {
            SaveRidecamSSID(pBuf);
            break;
        }
        case RCLOG_AP_STARTED:
        {
            SetRcBootStep(RC_BSTEP_APOK);
            //DoCmdCheck7601();
            break;
        }
        case RCLOG_DHCPD_STARTED:
        {    
            SetRcBootStep(RC_BSTEP_DHCPDOK); 
            DoCmdGetInetStatus();      
            break;
        }
        case RCLOG_RA0_INET:
        {
            SendInetStaToPc("Ra0 get Inet Addr");
            SetRcBootStep(RC_BSTEP_INET); 
            DoCmdCheck7601();
            break;   
        }
        case RCLOG_CHECK_7601:
        {
            SendInetStaToPc("Get 7601 Device");
            //DoCmdCheckRa0Stat();
            DoCmdCheckUdhcpd();
            //checkUdhcpd(pBuf);
            SetRcBootStep(RC_BSTEP_7601); 
            break;
        }    
        default:
        {
            ;
        }
    }
    return 0;
}

int ParseRidecamSerialData(char* pBuf, int bufLen)
{
    if(NULL == pBuf || 0 > bufLen)
    {
        return -1;
    }

    int i = 0;
    int ret = 0;
    char* pStr = NULL;
    for(i = RCLOG_SYSTEM_LOGIN; i < RCLOG_NUM_MAX; i++)
    {
        pStr = strstr(pBuf, ridecamLog[i]);
        if(NULL != pStr)
        {
            ret = strncmp((char*)pBuf, ridecamLog[i], strlen(ridecamLog[i]));
            if(ret == 0)
            {
                ParseRidecamSerialLog((RCLOG_TYPE_t)i, pBuf, bufLen);
                break;
            }
        }
    }
    return 0;
}



