#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "switchled.h"
#include "rotary.h"

int share_val = 0;
int alret_val = 0;
pthread_mutex_t mutex;
void SendMessage(int socket, const char* message);
void ErrorHandling(char* message);

int main() {
    int hSocket;
    struct sockaddr_in servAddr;
    char message[30];
    int strLen;

    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == -1)
        ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("172.20.10.5");
    servAddr.sin_port = htons(8080);

    if (connect(hSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
        ErrorHandling("connect() error");

    // 메시지 전송
    //SendMessage(hSocket, "Hello from client!");

    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi\n");
        return 1;
    }

    pthread_t rotary, switchled;
    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&rotary, NULL, rotaryThread, NULL) != 0) {
        fprintf(stderr, "Failed to create thread for rotary\n");
        return 1;
    }

    if (pthread_create(&switchled, NULL, LEDThread, NULL) != 0) {
        fprintf(stderr, "Failed to create thread for rotary\n");
        return 1;
    }


    char val_str[30];

    sprintf(val_str, "%d %d", share_val, alret_val);
    printf("%d %d", share_val, alret_val);
    // Declare a global variable to store the previous share value
    int prev_share_value = 0;
    int prev_alret_value = 0;
// ...

    while (1) {
        // 동기화
        pthread_mutex_lock(&mutex);
        int current_share_val = share_val;
        int current_alret_val = alret_val;
        pthread_mutex_unlock(&mutex);

        // Check if the current_share_val is different from the previous share value
        if (current_share_val != prev_share_value) {
            sprintf(val_str, "%d %d", current_share_val, current_alret_val);
            SendMessage(hSocket, "1 0");
            printf("Open\n");

            // Update the previous share value
            prev_share_value = current_share_val;
        }

        printf("%d", current_alret_val);
        // Check if the current_alret_val is different from the previous alret value
        if (current_alret_val != prev_alret_value) {
            sprintf(val_str, "%d %d", current_share_val, current_alret_val);
            SendMessage(hSocket, "0 1");
            printf("alert_Value+\n");

            // Update the previous alret value
            prev_alret_value = current_alret_val;
        }
        sleep(1);
}


    // pthread_cancel이 필요한 경우 사용
    // pthread_cancel(switchled);

    // Cancel and join the rotary thread
    pthread_join(rotary, NULL);
    pthread_join(switchled, NULL);

    pthread_mutex_destroy(&mutex);
    close(hSocket);
    return 0;
}

void ErrorHandling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void SendMessage(int socket, const char* message) {
    // 메시지 전송
    if(send(socket, message, strlen(message), 0) == -1){
        perror("send() 오류 ");
    }
}
