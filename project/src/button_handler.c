// button_handler.c

#include "button_handler.h"
#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>

#define btn1 23
#define btn2 24

extern int share_var;
extern pthread_mutex_t mutex;

void *BtnconFunc(void *arg)
{
    pinMode(btn1, INPUT);
    int prevSwitchState = LOW;
    while (1)
    {
        int switchState = digitalRead(btn1);
        if (switchState == HIGH && prevSwitchState == LOW)
        {
            pthread_mutex_lock(&mutex);
            share_var++;
            printf("btn1 click: %d\n", share_var);
            pthread_mutex_unlock(&mutex);
        }
        prevSwitchState = switchState;
        delay(10);
    }
}

void *BtnconFunc1(void *arg)
{
    pinMode(btn2, INPUT);
    int prevSwitchState = LOW;
    while (1)
    {
        int switchState = digitalRead(btn2);
        if (switchState == HIGH && prevSwitchState == LOW)
        {
            pthread_mutex_lock(&mutex);
            share_var--;
            printf("btn2 click: %d\n", share_var);
            pthread_mutex_unlock(&mutex);
        }
        prevSwitchState = switchState;
        delay(10);
    }
}