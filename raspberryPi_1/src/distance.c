#include "distance.h"

int distance = 9999; // Initialize distance

extern int share_val;
extern int alret_val;

extern pthread_mutex_t mutex;

#define TRIG_PIN 15
#define ECHO_PIN 18


void *getDistance(void *arg) {
    wiringPiSetupGpio(); /* wiringPi */
    pinMode(TRIG_PIN, OUTPUT);  // TRIG pin mode setting
    pinMode(ECHO_PIN, INPUT);   // ECHO pin mode setting

    while (1)
    {
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);

        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        while (digitalRead(ECHO_PIN) == LOW);

        long startTime = micros();
        while (digitalRead(ECHO_PIN) == HIGH);
        long travelTime = micros() - startTime;

        distance = travelTime / 58.0;

        printf("Distance: %dcm\n", distance);
        delay(200);

        if (distance <= 15) {
            system("python3 src/camRecog.py");
            pthread_mutex_lock(&mutex);
        
            FILE *file;
            char buffer[100]; // 적절한 버퍼 크기로 조절

            // 파일 열기
            file = fopen("example.txt", "r"); // "r"은 읽기 모드

            if (file == NULL) {
                perror("파일 열기 실패");
                return 1;
            }

            // 파일에서 데이터 읽기
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s\n제발 되라", buffer);
            }

            // 파일 닫기
            fclose(file);
            if (buffer[0] == '0'){
                share_val += 1;
            }
            if (buffer[0] == '1'){
                alret_val += 1;
            }
            
            printf("shared value: %d\n", share_val);
            printf("alret %d\n", alret_val);
            
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    return NULL;
}
