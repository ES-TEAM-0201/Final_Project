#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>
#include "switchled.h"

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

int light_on1 = 0;
int light_on2 = 0;
int light_on3 = 0;
int light_on4 = 0;
int light_on5 = 0;

extern int share_val;
extern int alret_val;
extern pthread_mutex_t mutex;

int light_on(int pin) {
    switch (pin) {
        case 1: return light_on1;
        case 2: return light_on2;
        case 3: return light_on3;
        case 4: return light_on4;
        case 5: return light_on5;
        default: return 0;
    }
}

int check_password() {
    return (light_on1) && (light_on2) && (light_on5) && (!light_on3) && (!light_on4);
}

void toggle_led(int pin) {
    switch (pin) {
        case 1: 
            digitalWrite(LED_PIN1, light_on1 = !light_on1);
            break;
        case 2: 
            digitalWrite(LED_PIN2, light_on2 = !light_on2);
            break;
        case 3: 
            digitalWrite(LED_PIN3, light_on3 = !light_on3);
            break;
        case 4: 
            digitalWrite(LED_PIN4, light_on4 = !light_on4);
            break;
        case 5: 
            digitalWrite(LED_PIN5, light_on5 = !light_on5);
            break;
    }
}

void button_callback1() {
    toggle_led(1);
    printf("LED %s1!\n", light_on1 ? "ON" : "OFF");
}

void button_callback2() {
    toggle_led(2);
    printf("LED %s2!\n", light_on2 ? "ON" : "OFF");
}

void button_callback3() {
    toggle_led(3);
    printf("LED %s3!\n", light_on3 ? "ON" : "OFF");
}

void button_callback4() {
    toggle_led(4);
    printf("LED %s4!\n", light_on4 ? "ON" : "OFF");
}

void button_callback5() {
    toggle_led(5);
    printf("LED %s5!\n", light_on5 ? "ON" : "OFF");
}

void* LEDThread(void* arg)
{
    wiringPiSetupGpio();
    
    pinMode(BUTTON_PIN1, INPUT);
    pullUpDnControl(BUTTON_PIN1, PUD_DOWN);
    pinMode(LED_PIN1, OUTPUT);
    
    pinMode(BUTTON_PIN2, INPUT);
    pullUpDnControl(BUTTON_PIN2, PUD_DOWN);
    pinMode(LED_PIN2, OUTPUT);
    
    pinMode(BUTTON_PIN3, INPUT);
    pullUpDnControl(BUTTON_PIN3, PUD_DOWN);
    pinMode(LED_PIN3, OUTPUT);
    
    pinMode(BUTTON_PIN4, INPUT);
    pullUpDnControl(BUTTON_PIN4, PUD_DOWN);
    pinMode(LED_PIN4, OUTPUT);
    
    pinMode(BUTTON_PIN5, INPUT);
    pullUpDnControl(BUTTON_PIN5, PUD_DOWN);
    pinMode(LED_PIN5, OUTPUT);

    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
    digitalWrite(LED_PIN4, LOW);
    digitalWrite(LED_PIN5, LOW);

    wiringPiISR(BUTTON_PIN1, INT_EDGE_RISING, &button_callback1);
    wiringPiISR(BUTTON_PIN2, INT_EDGE_RISING, &button_callback2);
    wiringPiISR(BUTTON_PIN3, INT_EDGE_RISING, &button_callback3);
    wiringPiISR(BUTTON_PIN4, INT_EDGE_RISING, &button_callback4);
    wiringPiISR(BUTTON_PIN5, INT_EDGE_RISING, &button_callback5);

    while (1) {
        if (check_password()) {
            pthread_mutex_lock(&mutex);
            share_val += 1;
            pthread_mutex_unlock(&mutex);

            
            break;
        }
    }

    return NULL;
}

