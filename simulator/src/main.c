#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "io.h"
#include "bridge.h"
#include "ui.h"

int main(const int argc, const char *const argv[]) {
    if (argc != 2) {
        perror("invalid arguments");
        exit(1);
    }
    // "/dev/ttyS0"
    int fd = openPort(argv[1]);

    initStdin();

    printf("NQ N BR S SQ\n");
    Bridge bridge = INIT_BRIDGE;
    print(bridge);

    State state = { bridge, PTHREAD_MUTEX_INITIALIZER };

    pthread_t receiverThread;
    Handler handler = { fd, process, (void*)&state };
    int err = pthread_create(&receiverThread, NULL, &recv, (void*)&handler);
    // No attributes, assume system resources are sufficient.
    assert(err == 0);

    input(fd, &state);

    return 0;
}
