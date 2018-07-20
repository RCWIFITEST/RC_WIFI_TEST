#ifndef PC_SERIAL_PROTOCAL_H
#define PC_SERIAL_PROTOCAL_H

#define PC_PKG_HEADER              0x4657 // W , F


//MCU->PC
#define PC_GET_LOG                  'A' //Log info from Testboard to PC.
#define PC_GET_SSID                 'S' //SSID FROM RC
#define PC_GET_INET                 'I' // INET ADDRESS
#define PC_GET_RESULT               'R' // TestResult


//int parsePcSerialData(char* pBuf, int bufLen);
int SendLogToPc(char* pLog);
int SendSsidToPc(char* pSsid);
int SendInetStaToPc(char* pInetStat);
int SendTestResultToPC(char* pResult);

#endif //PC_SERIAL_PROTOCAL_H



