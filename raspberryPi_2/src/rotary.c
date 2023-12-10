#include "rotary.h"


#define CLK_PIN 20
#define DT_PIN 21
#define BUTTON_PIN 16

extern int share_val;
extern int alret_val;
extern pthread_mutex_t mutex;

int target_count = 1;
int target_anticount = 2;

int counter;
int anti_counter;

int last_clk_state;
int last_dt_state;
int last_button_state;

int clk_state;
int button_state;

int pass_flag = 0;

unsigned long last_button_press =0;


void rota()
{
    clk_state = digitalRead(CLK_PIN);
    button_state = digitalRead(BUTTON_PIN);

    if (clk_state != last_clk_state && clk_state == 1 ) {
        if (digitalRead(DT_PIN) != clk_state) {
           counter++;
           printf("회전 횟수: %d, 반회전 횟수: %d\n", counter, anti_counter);
        } 
        else {
            anti_counter++;
            printf("회전 횟수: %d, 반회전 횟수: %d\n", counter, anti_counter);
        }
        
    } 
    
    if (button_state == 0 && (millis() - last_button_press) > 5000)
    {
        if (counter == target_count && anti_counter == target_anticount) {
            pthread_mutex_lock(&mutex);
            share_val += 1; /* value from rotary */;
            pthread_mutex_unlock(&mutex);
        }
        else if (counter > target_count || anti_counter > target_anticount)
        {
            pthread_mutex_lock(&mutex);
            alret_val += 1; /* value from rotary */;
            pthread_mutex_unlock(&mutex);
            counter = 0;
            anti_counter = 0;
        }
    }

    last_clk_state = clk_state;

    if (button_state == 0)
        last_button_press = millis();

    delay(1);   
}



void* rotaryThread(void* arg) {
    

    wiringPiSetupGpio();

    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT);

    last_clk_state = digitalRead(CLK_PIN);
    clk_state = last_clk_state;

    counter = 0;
    anti_counter = 0;

    printf("회전 횟수 : %d, 반회전 횟수 : %d, 5초 이상 누르기\n", target_count, target_anticount);

    while (1) {
        rota();
    }

    return NULL;
}
