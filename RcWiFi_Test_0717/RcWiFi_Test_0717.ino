#include <WiFi.h>
#include "HardwareSerial.h"
#include "RcSerialProtocal.h"
#include "WIFI_Scan.h"
#include "WIFI_Connect.h"
#include "PcSerialProtocal.h"
#include "StateMachine.h"
#include "IoCtrl.h"
#include "AlarmCtrl.h"

#define S_RCVBUF_SIZE   256
char RcRecvBuf[S_RCVBUF_SIZE] = {0};   //
int CountTimes = 0;

HardwareSerial SerialRidecam(2);

int SerialReadline(HardwareSerial* pSerial, char* buf, int bufSize)
{
    if(NULL == pSerial || NULL == buf)
    {
        return -1;
    }

    if(!pSerial->available())
    {
        return -1;
    }

    char* wPos = buf;
    char rcv = '\0';
    int rcvCount = 0;
    while(pSerial->available())
    {
        rcv = pSerial->read();
        if(rcv != -1)
        {
            if(bufSize <= rcvCount)
            {
                return -1;
            }

            wPos[rcvCount++] = rcv;
            if(rcv == '\n')
            {
                return rcvCount;
            }
        }

        //Wait until read a complete line.
        while(pSerial->available() == 0)
        {
            ;//
        }
    }

    return -1;
}

int ProcessSerialRidecam()
{
    if(SerialRidecam.available())
    {
        int count = 0;
        bzero(RcRecvBuf, sizeof(RcRecvBuf));
        count = SerialReadline(&SerialRidecam, RcRecvBuf, sizeof(RcRecvBuf));
        ParseRidecamSerialData(RcRecvBuf, count);
    }

    return 0;
}


int ProcessWiFiTask()
{
    if(RC_BSTEP_RESTART == GetRcBootStep())
    {
        SendTestResultToPC("System Have To Restart");
        SetTbState(TB_STATE_STOP);
    }
    else
    {
        if(RC_BSTEP_7601 == GetRcBootStep())
        {  
            if(NULL != GetRidecamSSID())
            {    
                if(ScanWifi() == 0)
                {    
                    PowerOffBuzzerAlarm(); 
                    if(DoWiFiTask() == 0)
                    {
                        SendTestResultToPC("Test Successful");
                        CountTestTimes();
                        SetTbState(TB_STATE_STOP);
                        return 0;  
                    }
                    else
                    {
                        SendTestResultToPC("Test Failed");
                        return -1;
                    }
                }
                else
                {
                    PowerOnBuzzerAlarm();
                    return -1;
                }
            }
        }
        else
        {
            return -1;   
        }
    }
    return 0;
}
int CountTestTimes()
{
        CountTimes = CountTimes +1;
        Serial.println(millis());
        Serial.print("测试成功次数：");
        Serial.println(CountTimes);
}

void setup() 
{
    Serial.begin(115200);
    SerialRidecam.begin(115200);
    InitPowerIoCtrl();
    InitAlarmCtrl();
    delay(20000);
}

void loop() 
{
    SetTbState(TB_STATE_RUN);    
    ProcessSerialRidecam(); 
    ProcessWiFiTask();          
}



