
#include "StateMachine.h"
#include "IoCtrl.h"
#include "WIFI_Connect.h"
#include "PcSerialProtocal.h"

static TB_STATE_t TestBoardStatus = TB_STATE_INIT;


TB_STATE_t GetTbState()
{
    return TestBoardStatus;
}

int SetTbState(TB_STATE_t state)
{
    TestBoardStatus = state;
    WiFiClient client;
    if(TB_STATE_STOP == state)
    {
        DisconnectWifi();
        DisconnectTCP(&client);
        ClearRidecamSSID();
        PowerOffRidecam();
        PowerOffBuzzerAlarm();
        delay(22000);
    }
    else if(TB_STATE_RUN == state)
    {
        PowerOnRidecam();
    }
    return 0;
}


static RC_BOOTSTEP_t g_rcBootStep = RC_BSTEP_HALT;
RC_BOOTSTEP_t GetRcBootStep()
{
    return g_rcBootStep;
}

int SetRcBootStep(RC_BOOTSTEP_t bStep)
{
    g_rcBootStep = bStep;
}


