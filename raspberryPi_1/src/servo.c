#include "servo.h"

#define PWM1 13

void servo_init()
{
    wiringPiSetupGpio();

    float duty = 5; // 초기 위치 0도에서 시작 (5는 0도, 25는 180도)
    softPwmCreate(PWM1, duty, 25); //pin번호, 초기값, 주기(pwm 범위 설정)
}

void servo()  {
    float duty = 15; // 90도로 이동
    softPwmWrite(PWM1, duty);
    delay(200); // 각도 변화 후 일정 시간 기다림

    return;
}