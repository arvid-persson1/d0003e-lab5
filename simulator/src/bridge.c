#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include "bridge.h"
#include "ui.h"

#define DRIVE_TIME 5

static void *leave(void *arg) {
    State *state = (State*)arg;
    
    printf("car enters bridge\n");
    sleep(DRIVE_TIME);
    printf("car leaves bridge\n");

    pthread_mutex_lock(&state->lock);
    state->bridge.onBridge--;
    Bridge bridge = state->bridge;
    pthread_mutex_unlock(&state->lock);

    print(bridge);

    return NULL;
}

static void deferLeave(const State *const state) {
    pthread_t thread;
    pthread_attr_t attr;

    int err;
    err = pthread_attr_init(&attr);
    assert(err == 0);
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(err == 0);
    err = pthread_create(&thread, &attr, leave, (void*)state);
    // `attr` is valid. Assume system resources are sufficient.
    assert(err == 0);

    pthread_attr_destroy(&attr);
}

Response process(void *arg, const uint8_t data) {
    State *state = (State*)arg;
    typedef struct {
        bool northGreen : 1,
             northRed   : 1,
             southGreen : 1,
             southRed   : 1;
        // TODO: remove.
        unsigned int _pad : 4;
    } LightStatus;

    union {
        uint8_t raw;
        LightStatus status;
    } pun = { data };
    LightStatus status = pun.status;
    assert(status._pad == 0);

    assert(!(status.northGreen && status.southGreen));
    assert(status.northGreen ^ status.northRed);
    assert(status.southGreen ^ status.southRed);

    Response response;

    int *queue;
    if (status.northGreen) {
        printf("north light green\n");
        queue = &state->bridge.northQueue;
        response = RESPONSE(NORTH_ENTRY);
        deferLeave(state);
    } else if (status.southGreen) {
        printf("south light green\n");
        queue = &state->bridge.southQueue;
        response = RESPONSE(SOUTH_ENTRY);
        deferLeave(state);
    } else {
        printf("lights red\n");
        queue = NULL;
        response = NO_RESPONSE;
    }

    pthread_mutex_lock(&state->lock);
    state->bridge.northLight = (Light)status.northGreen;
    state->bridge.southLight = (Light)status.southGreen;
    if (queue != NULL) {
        (*queue)--;
        state->bridge.onBridge++;
    }
    Bridge bridge = state->bridge;
    pthread_mutex_unlock(&state->lock);

    print(bridge);

    return response;
}
