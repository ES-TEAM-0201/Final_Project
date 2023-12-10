#ifndef DISTANCE_H
#define DISTANCE_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>


void *rotaryThread(void *arg);

#endif
