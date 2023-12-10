#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>

#include "distance.h"
#include "alret.h"
#include "server.h"
#include "servo.h"

int share_val = 0;
int alret_val = 0;
pthread_mutex_t mutex;

int main()
{
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi\n");
        return 1;
    }
    

    pthread_t Camera, server;
    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&Camera, NULL, getDistance , NULL) != 0) {
        fprintf(stderr, "Failed to create thread for btn1\n");
        return 1;
    }

    if (pthread_create(&server, NULL, runServer , NULL) != 0) {
        fprintf(stderr, "Failed to create thread for btn1\n");
        return 1;
    }

    alret_init(); 
    servo_init();
    
    while (1)
    {
        printf("main shared_value, alret_value : %d %d\n", share_val, alret_val);
        
        // 실시간으로 종료 조건 확인
        if (share_val == 3) {
            printf("hello");
            servo();
            break; // 프로그램 종료
        }
        if (alret_val == 3) {
            alret();
            break; // 프로그램 종료
        }

        // 일정 간격으로 확인
        sleep(1);
    }

    // 메인 스레드에서 스레드 종료
    pthread_cancel(Camera);
    pthread_cancel(server);

    pthread_join(Camera, NULL);
    pthread_join(server, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}