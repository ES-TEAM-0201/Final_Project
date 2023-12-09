

#include "alret.h"
#include <wiringPi.h>
#include <stdio.h>

#define PWM0 18
#define LED 21

void initMyTone()
{
    pinMode(LED, OUTPUT);
    pinMode(PWM0, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1000);
}

void alret()
{
    while (1)
    {
        pwmSetClock(19200000 / (1000 * 440));
        digitalWrite(LED, HIGH);
        pwmWrite(PWM0, 500);
        delay(1000);
        pwmSetClock(19200000 / (1000 * 293));
        digitalWrite(LED, LOW);
        pwmWrite(PWM0, 500);
        delay(1000);
    }
}
