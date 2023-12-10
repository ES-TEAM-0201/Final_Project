#ifndef DISTANCE_H
#define DISTANCE_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


extern int distance; // Declare distance as an external variable

void *getDistance(void *arg);

#endif
