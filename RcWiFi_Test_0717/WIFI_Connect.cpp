#include <WiFi.h>
#include "WIFI_Scan.h"
#include "WIFI_Connect.h"
#include "IoCtrl.h"
#include "AlarmCtrl.h"
#include "RcSerialProtocal.h"
#include "StateMachine.h"
#include "PcSerialProtocal.h"

const char *    WifiPassword = "78932100";
const uint16_t  RcTcpPort = 5677;
const char *    RcIpAddr = "192.168.1.202";
char* Find_Version = "";

char* SaveRidecamVersion(const char* pStr);

int ConnectWiFi(const char* wifiSSID, const char* wifiPassword)
{
    //unsigned long CurrentTime = millis();
    //static unsigned long OldTime = CurrentTime;
    WiFi.begin(wifiSSID, wifiPassword);
    while (WiFi.status() != WL_CONNECTED)
    {
        //CurrentTime = millis();        
        //if(CurrentTime - OldTime > 30000)
        //{      
            //Serial.println("CONNECT WIFI Failed");
            //OldTime = CurrentTime;
           // return -1;
        //} 
        delay(10000); 
        WiFi.begin(wifiSSID, wifiPassword);  
    }
    //Serial.println("CONNECT WIFI Successful");
    return 0;
}

int ConnectTCP(WiFiClient* pClient, const char* pIpAddr, const unsigned int port)
{
    if(NULL == pClient)
    {
        return -1;
    }

    if(!pClient->connect(pIpAddr, port))
    {
        //Serial.println("Tcp Connect Failed");
        return -1;
    }
    //Serial.println("Tcp Connect Successful");
    return 0;
}

int GetRcVersion(WiFiClient* pClient)
{
    char* ret = NULL;
    WiFiClient client;  
    pClient->write("<message><head><version>1.1.1.1</version><id>1010</id></head><body><type>0</type></body></message>");   
    String ResultLine = pClient->readStringUntil('\r');
    const char* ResultLineChangeType = ResultLine.c_str();
    SaveRidecamVersion(ResultLineChangeType);
#if 0
    ret = strstr(ResultLineChangeType,Find_Version);
    if(NULL != ret)
    {
        return 0;
    }
    else
    {
        Serial.println("Wrong Version");
        return -1;
    }
#endif    
}

static char rcVersion[64] = {0};
char* SaveRidecamVersion(const char* pStr)
{
    if(NULL == pStr)
    {
        return NULL;
    }

    //AUTHMODE=WPA2PSK, ENCRYPT=AES,APSSID=RIDECAM_003518CC000000D7,PASSWORD=78932100
    char* pVersion = NULL;
    pVersion = strstr(pStr, "<swversion>");
    if(pVersion)
    {
        bzero(rcVersion, sizeof(rcVersion));
        memcpy(rcVersion, pVersion + 11,14);
        //Serial.println(rcSsid);
        SendSsidToPc(rcVersion);
        return rcVersion;
    }

    return NULL;
}

int DoTcpTask()
{
    WiFiClient client;
    if(0 > ConnectTCP(&client, RcIpAddr, RcTcpPort))
    {
        PowerOnBuzzerAlarm();
        Serial.println("Tcp Connect Failed");
        return -1;
    }
    if(0 > GetRcVersion(&client))
    {
      return -1;
    } 
    return 0;  
}

int DoWiFiTask()
{   
    if(NULL == GetRidecamSSID())
    {
        return -1;
    }
    if(0 != ConnectWiFi(GetRidecamSSID(), WifiPassword))
    {
        PowerOnBuzzerAlarm();
        return -1;
    }
    DoTcpTask();
    return 0;
    
}

int DisconnectWifi()
{
    WiFi.disconnect(true);
    return -1;
}

int DisconnectTCP(WiFiClient* pClient)
{
    if(pClient->connected())
    {
        pClient->stop();
    }
    return -1;
}
















