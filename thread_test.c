#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> /* atoi( ) */
#include <pthread.h>

#define btn1 23
#define btn2 24
#define PWM0 18
#define LED 21

int share_var = 0;
pthread_mutex_t mutex;

void *BtnconFunc(void *);
void *BtnconFunc1(void *);
void initMyTone();
void alret();

int main()
{   
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi\n");
        return 1;
    }
    initMyTone();

    pthread_t ptBtn1, ptBtn2;
    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&ptBtn1, NULL, BtnconFunc, NULL) != 0) {
        fprintf(stderr, "Failed to create thread for btn1\n");
        return 1;
    }

    if (pthread_create(&ptBtn2, NULL, BtnconFunc1, NULL) != 0) {
        fprintf(stderr, "Failed to create thread for btn2\n");
        return 1;
    }

    while (1)
    {
        // 실시간으로 종료 조건 확인
        if (share_var == 3) {
            printf("Open\n");
            break; // 프로그램 종료
        }
        if (share_var == -3) {
            alret();
            break; // 프로그램 종료
        }

        // 일정 간격으로 확인
        sleep(1);
    }

    // 메인 스레드에서 스레드 종료
    pthread_cancel(ptBtn1);
    pthread_cancel(ptBtn2);

    pthread_join(ptBtn1, NULL);
    pthread_join(ptBtn2, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}

void *BtnconFunc(void *arg)
{
    pinMode(btn1, INPUT);
    while (1)
    {
        int switchState = digitalRead(btn1);
        if (switchState == 1)
        {
            pthread_mutex_lock(&mutex);
            share_var++;
            printf("btn1 click: %d\n", share_var);
            pthread_mutex_unlock(&mutex);
            delay(50);
        }
    }
}

void *BtnconFunc1(void *arg)
{
    pinMode(btn2, INPUT);
    while (1)
    {
        int switchState = digitalRead(btn2);
        if (switchState == 1)
        {
            pthread_mutex_lock(&mutex);
            share_var--;
            printf("btn2 click: %d\n", share_var);
            pthread_mutex_unlock(&mutex);
            delay(50);
        }
    }
}


void initMyTone()
{
    pinMode(LED, OUTPUT);

    pinMode(PWM0, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1000);
}


void alret(){
    while (1){
    pwmSetClock(19200000 / (1000*440));
    digitalWrite(LED, HIGH);
    pwmWrite(PWM0, 500);
    delay(1000);
    pwmSetClock(19200000 / (1000*293));
    digitalWrite(LED, LOW);
    pwmWrite(PWM0, 500);
    delay(1000);
    }
}
