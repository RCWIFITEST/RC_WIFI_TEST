#include <WiFi.h>
#include "WIFI_Scan.h"
#include "WIFI_Connect.h"
#include "IoCtrl.h"
#include "AlarmCtrl.h"
#include "RcSerialProtocal.h"


int ScanWifi()
{   
    char* ret = NULL;
    for (int loops = 20; loops > 0; loops--)
    { 
        int n = WiFi.scanNetworks(); 
        for (int i = 0; i < n; ++i)
        {
            const char* SsidTurnChar = WiFi.SSID(i).c_str();
            ret = strstr(SsidTurnChar,GetRidecamSSID());
            if (NULL != ret)
            {
                break; 
            }
        }     
        if(NULL != ret)
        { 
            //Serial.print("扫描WiFi次数");
            //Serial.println(loops);
            break;  
        }
        else
        {
              //Serial.println(loops);
        }
    }
    if(NULL != ret)
    { 
        //Serial.println("FIND SSID ");
        //Serial.println(ret);
        //ScanWifiTimes = ScanWifiTimes + 1;
        //Serial.print("扫到WiFi次数");
        //Serial.println(ScanWifiTimes);
        return 0;
    }
    else
    {
        Serial.println("Scan SSID Failed.");
        PowerOnBuzzerAlarm();
        return -1;
    }
} 


