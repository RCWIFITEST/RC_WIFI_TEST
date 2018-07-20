#include "AlarmCtrl.h"
#include "esp32-hal-gpio.h"

#define POWER_CTRL_PIN_1  12


int InitAlarmCtrl()
{
    pinMode(POWER_CTRL_PIN_1, OUTPUT);
    PowerOffBuzzerAlarm();
    return 0;
}

int PowerOnBuzzerAlarm()
{
    digitalWrite(POWER_CTRL_PIN_1, HIGH);
    return 0;
}

int PowerOffBuzzerAlarm()
{
    digitalWrite(POWER_CTRL_PIN_1, LOW);
    return 0;
}




