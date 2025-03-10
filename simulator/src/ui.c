#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "bridge.h"

static inline char *toString(Light light) {
    return light ? "\033[1;32mG\033[0m" : "\033[1;31mR\033[0m";
}

// `Bridge` is small; copying it is preferable to locking the mutex.
void print(const Bridge bridge) {
    printf(
        "%2d %s %2d %s %2d\n",
        bridge.northQueue,
        toString(bridge.northLight),
        bridge.onBridge,
        toString(bridge.southLight),
        bridge.southQueue
    );
}

void input(int fd, State *const state) {
    while (true) {
        switch (getchar()) {
            case 'n':
                pthread_mutex_lock(&state->lock);
                state->bridge.northQueue++;
                pthread_mutex_unlock(&state->lock);
                send(fd, NORTH_ARRIVAL);
                break;
            case 's':
                pthread_mutex_lock(&state->lock);
                state->bridge.southQueue++;
                pthread_mutex_unlock(&state->lock);
                send(fd, SOUTH_ARRIVAL);
                break;
            case 'e':
                close(fd);
                exit(0);
            default:
                perror("unknown command");
                break;
        }
    }
}
