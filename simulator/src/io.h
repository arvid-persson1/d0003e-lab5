#ifndef _IO_H
#define _IO_H

#include <stdint.h>

int openPort(const char *const port);
void initStdin(void);

void send(const int, const uint8_t);

typedef struct {
    uint8_t msg;
    bool set;
} Response;

#define RESPONSE(msg) (Response){ msg, true }
#define NO_RESPONSE   (Response){ .set = false }

typedef struct {
    int fd;
    Response (*function)(void*, const uint8_t);
    void *arg;
} Handler;

void *recv(void*);

#endif
