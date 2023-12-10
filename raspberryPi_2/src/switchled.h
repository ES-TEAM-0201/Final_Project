// switchled.h
#ifndef SWITCHLED_H
#define SWITCHLED_H

#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <pthread.h>

#define BUTTON_PIN1 15
#define BUTTON_PIN2 27
#define BUTTON_PIN3 10
#define BUTTON_PIN4 9
#define BUTTON_PIN5 11
#define LED_PIN1 4
#define LED_PIN2 17
#define LED_PIN3 22
#define LED_PIN4 5
#define LED_PIN5 6

extern int light_on1;
extern int light_on2;
extern int light_on3;
extern int light_on4;
extern int light_on5;

extern int light_on(int pin);
extern int check_password();
extern void toggle_led(int pin);

extern void button_callback1();
extern void button_callback2();
extern void button_callback3();
extern void button_callback4();
extern void button_callback5();

extern void* LEDThread(void* arg);

#endif  // SWITCHLED_H
