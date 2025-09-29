#include "../include/Custom.h"

const char *status_string(StatusCode status)
{
    switch (status)
    {
    case SUCCESS:
        return "success";
    case ERROR_PIPE:
        return "pipe error";
    case ERROR_FORK:
        return "fork error";
    case ERROR_WAIT:
        return "wait error";
    case ERROR_WRITE:
        return "write error";
    case ERROR_CLOSE:
        return "close error";
    case ERROR_OPEN:
        return "open error";
    case ERROR_READ:
        return "read error";
    default:
        return "unknown error";
    }
}

StatusCode CustomFork(pid_t *pid)
{
    *pid = fork();
    if (*pid == -1)
    {
        return ERROR_FORK;
    }
    return SUCCESS;
}

StatusCode CustomPipe(int *pfd)
{
    if (pipe(pfd) == -1)
    {
        return ERROR_PIPE;
    }
    return SUCCESS;
}

StatusCode CustomWait(pid_t pid)
{
    if (waitpid(pid, NULL, 0) == -1)
    {
        return ERROR_WAIT;
    }
    return SUCCESS;
}

StatusCode CustomClose(int fd)
{
    if (close(fd) == -1)
    {
        return ERROR_CLOSE;
    }
    return SUCCESS;
}

StatusCode CustomWrite(int fd, const void *buf, size_t count)
{
    if (write(fd, buf, count) == -1)
        return ERROR_WRITE;
    return SUCCESS;
}

StatusCode CustomRead(int fd, void *buf, size_t count)
{
    if (read(fd, buf, count) == -1)
        return ERROR_READ;
    return SUCCESS;
}

StatusCode CustomOpen(int *fd, const char *filename, int access, unsigned mode)
{
    *fd = open(filename, access, mode);
    if (*fd == -1)
    {
        return ERROR_OPEN;
    }
    return SUCCESS;
}

void log_error(StatusCode code, const char *context)
{
    char message[101];
    const char *err = status_string(code);
    snprintf(message, sizeof(message), "Error in %s: %s\n", context, err);
    CustomWrite(2, message, strlen(message));
}