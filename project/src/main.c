#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include "button_handler.h"
#include "alret.h"

int share_var = 0;
pthread_mutex_t mutex;

int main()
{
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi\n");
        return 1;
    }

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