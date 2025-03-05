#include <stdio.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT "/dev/ttyS0"

int main() {
    int fd = open(PORT, O_WRONLY | O_NOCTTY);
    if (fd < 0) {
        perror("failed to open port");
        return 1;
    }

    if (!isatty(fd)) {
        perror("port is not a terminal");
        return 1;
    }

    struct termios tty;
    int attr = tcgetattr(fd, &tty);
    // `fd` is opened, file is a terminal; this never fails.
    // Omit closing fd here.
    assert(attr == 0);

    cfsetospeed(&tty, B9600);
    tty.c_cflag = CS8 | CLOCAL | CREAD;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("signal interrupt");
        close(fd);
        return 1;
    }

    while (1) {
        char c = getchar();
        if (c == 'q')
            break;

        if (write(fd, &c, 1) != 1) {
            perror("failed to write to port");
            return 1;
        }
    }

    close(fd);
    return 0;
}
