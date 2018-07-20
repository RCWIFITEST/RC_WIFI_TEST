#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum
{
    TB_STATE_INIT = 0,
    TB_STATE_RUN,
    TB_STATE_STOP
}TB_STATE_t;

TB_STATE_t GetTbState();
int SetTbState(TB_STATE_t state);


typedef enum
{
    RC_BSTEP_HALT = 0,
    RC_BSTEP_LOGIN,
    RC_BSTEP_APOK,
    RC_BSTEP_DHCPDOK,
    RC_BSTEP_INET,
    RC_BSTEP_7601,
    RC_BSTEP_RESTART,
}RC_BOOTSTEP_t;
RC_BOOTSTEP_t GetRcBootStep();
int SetRcBootStep(RC_BOOTSTEP_t bStep);


#endif//STATE_MACHINE_H

