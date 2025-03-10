#ifndef _BRIDGE_H
#define _BRIDGE_H

#include <pthread.h>
#include <stdint.h>
#include "io.h"

typedef enum {
    RED,
    GREEN
} Light;

typedef struct {
    int northQueue;
    int southQueue;
    int onBridge;
    Light northLight;
    Light southLight;
} Bridge;

#define INIT_BRIDGE { }

typedef struct {
    Bridge bridge;
    pthread_mutex_t lock;
} State;

Response process(void*, const uint8_t);

typedef struct {
    bool northArrival : 1,
         northEntry   : 1,
         southArrival : 1,
         southEntry   : 1;
    // TODO: remove.
    unsigned int _pad : 4;
} SensorStatus;

union _SendPun {
    SensorStatus ss;
    uint8_t arg;
};

#define NORTH_ARRIVAL ((union _SendPun){ { .northArrival = true } }).arg
#define NORTH_ENTRY   ((union _SendPun){ { .northEntry   = true } }).arg
#define SOUTH_ARRIVAL ((union _SendPun){ { .southArrival = true } }).arg
#define SOUTH_ENTRY   ((union _SendPun){ { .southEntry   = true } }).arg

#endif
