#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFi.h>
#include "WIFI_Scan.h"
#include "IoCtrl.h"
#include "AlarmCtrl.h"

//int JoinWifi();
//int GetRcVersion();

int DoWiFiTask();
int DoTcpTask();
int DisconnectWifi();
int DisconnectTCP(WiFiClient* pClient);

#endif //WIFI_SCAN_H


