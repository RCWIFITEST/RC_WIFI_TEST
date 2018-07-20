#ifndef RC_SERIAL_PROTOCAL_H
#define RC_SERIAL_PROTOCAL_H

typedef enum
{
    RCLOG_SYSTEM_LOGIN = 0,     //"(none) login:"   //
    RCLOG_SYSTEM_PASSWD,        //"Password:"       //�
    RCLOG_SYSTEM_SHELL,         //"~ #"             //
    RCLOG_PROBLEM_LOG,          //ERROR!!! MT7601_BBP_read.(300ms) 
    RELOG_CHECK_RESTART,        //[19700
    RCLOG_SSID_PASSWD,          //"AUTHMODE=WPA2PSK, ENCRYPT=AES,APSSID=" 
    RCLOG_AP_STARTED,           //"config AP successful"      //config AP successful
    RCLOG_DHCPD_STARTED,        //"udhcpd (v1.20.2) started"  //udhcpd (v1.20.2) started
    RCLOG_RA0_INET,             //ra0 Inet
    RCLOG_CHECK_7601,           //7601
      
    //FELOG_PROBLEMS_LOG          //[19700111-125306][02][process] EVENT_OP_TASK [ttxnet] 0
    RCLOG_NUM_MAX
}RCLOG_TYPE_t;

int ParseRidecamSerialData(char* pBuf, int bufLen);
char* GetRidecamSSID();
int ClearRidecamSSID();
//char* checkRa0Ip(char* pBuf);



#endif //RC_SERIAL_PROTOCAL_H



