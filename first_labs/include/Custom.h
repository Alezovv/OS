#ifndef __HOME_VBOXUSER_TEST_CUSTOM
#define __HOME_VBOXUSER_TEST_CUSTOM

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum
{
    SUCCESS = 0,
    ERROR_PIPE,
    ERROR_FORK,
    ERROR_WAIT,
    ERROR_WRITE,
    ERROR_CLOSE,
    ERROR_OPEN,
    ERROR_READ
} StatusCode;

const char *status_string(StatusCode status);

StatusCode CustomFork(pid_t *pid);

StatusCode CustomPipe(int *pfd);

StatusCode CustomWait(int a);

StatusCode CustomClose(int fd);

StatusCode CustomOpen(int *fd, const char *filename, int access, unsigned mode);

StatusCode CustomRead(int fd, void *buf, size_t count);

StatusCode CustomWrite(int fd, const void *buf, size_t count);

StatusCode read_number(int *data, int *quit);

void log_error(StatusCode code, const char *context);

#endif //__HOME_VBOXUSER_TEST_CUSTOM
