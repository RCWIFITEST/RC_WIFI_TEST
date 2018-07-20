#include "IoCtrl.h"
#include "esp32-hal-gpio.h"

#define POWER_CTRL_LED_PIN  25
#define POWER_CTRL_PIN      26

int InitPowerIoCtrl()
{
    pinMode(POWER_CTRL_LED_PIN, OUTPUT);
    pinMode(POWER_CTRL_PIN, OUTPUT);
    PowerOffRidecam();
    return 0;
}

int PowerOnRidecam()
{
    digitalWrite(POWER_CTRL_PIN, LOW);
    digitalWrite(POWER_CTRL_LED_PIN, HIGH);
    return 0;
}

int PowerOffRidecam()
{
    digitalWrite(POWER_CTRL_PIN, HIGH);
    digitalWrite(POWER_CTRL_LED_PIN, LOW);
    return 0;
}


