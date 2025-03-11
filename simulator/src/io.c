#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// TODO: many of these are unnecessary and/or already set.
void setFlags(__attribute__((unused)) struct termios *const tty) {
    tty->c_cflag &= ~CSIZE;
    tty->c_cflag &= ~PARENB;
    tty->c_cflag &= ~CSTOPB;
    tty->c_cflag |= CS8;
    tty->c_cflag |= B9600;
    tty->c_cflag |= CREAD;
    tty->c_cflag |= CLOCAL;
    tty->c_cflag |= HUPCL;
    tty->c_cflag |= INPCK;
    
    tty->c_iflag &= ~IGNBRK;
    tty->c_iflag &= ~BRKINT;
    tty->c_iflag &= ~ICRNL;
    tty->c_iflag &= ~INLCR;
    tty->c_iflag &= ~PARMRK;
    tty->c_iflag &= ~INPCK;
    tty->c_iflag &= ~ISTRIP;
    tty->c_iflag &= ~IXON;
    
    tty->c_oflag = 0;
    
    tty->c_lflag &= ~ ECHO;
    tty->c_lflag &= ~ ECHONL;
    tty->c_lflag &= ~ ICANON;
    tty->c_lflag &= ~ IEXTEN;
    tty->c_lflag &= ~ ISIG;
    
    tty->c_cc[VMIN]  = 1;
    tty->c_cc[VTIME] = 0; // 10?
}

int openPort(const char *const port) {
    int fd = open(port, O_RDWR | O_NOCTTY /*| O_NDELAY */);
    if (fd < 0) {
        perror("failed to open port");
        exit(1);
    }

    if (!isatty(fd)) {
        perror("port is not a terminal");
        close(fd);
        exit(1);
    }
 
    struct termios tty;
    int err;

    err = tcgetattr(fd, &tty);
    // `fd` is opened, file is a terminal.
    assert(err == 0); 

    setFlags(&tty);

    /*
    err = tcflush(fd, TCIFLUSH);
    // `fd` is opened, file is a terminal, TCIFLUSH is valid.
    assert(err == 0);
    */
    
    err = cfsetispeed(&tty, B9600);
    assert(err == 0);
    err = cfsetospeed(&tty, B9600);
    assert(err == 0);

    /*
    err = fcntl(fd, F_SETFL, O_NONBLOCK);
    if (err == -1) {
        perror("file control io error");
        close(fd);
        exit(1);
    }
    */

    if (tcsetattr(fd, TCSANOW /* TCSAFLUSH */, &tty) == -1) {
        perror("signal interrupt on port");
        close(fd);
        exit(1);
    }

    return fd;
}

// Accept single character, no echo, don't wait for <ENTER>.
void initStdin(void) {
    struct termios tty;

    int err = tcgetattr(STDIN_FILENO, &tty);
    assert(err == 0);

    tty.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("signal interrupt on stdin");
        exit(1);
    }
}

void send(const int fd, const uint8_t data) {
    int res = write(fd, &data, sizeof(data));
    assert(res == sizeof(data));
}

void *recv(void *arg) {
    Handler handler = *(Handler*)arg;
    uint8_t buf;

    while (true) {
        int res = read(handler.fd, &buf, sizeof(buf));
        assert(res == sizeof(buf));

        Response response = handler.function(handler.arg, buf);
        if (response.set) {
            send(handler.fd, response.msg);
        }
    }
}
