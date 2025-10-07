#include "include/Custom.h"

int main()
{
    char buffer[101];
    ssize_t bytes_read;
    int number;
    int pipefd[2];
    pid_t pid;
    StatusCode result;
    char filename[100];

    CustomWrite(1, "Enter filename: ", 16);
    bytes_read = read(0, filename, sizeof(filename) - 1);
    if (bytes_read <= 0)
    {
        CustomWrite(2, "Failed to read filename\n", 24);
        return 1;
    }

    int filename_len = 0;
    while (filename_len < bytes_read && filename[filename_len] != '\n' && filename[filename_len] != '\0')
    {
        filename_len++;
    }
    filename[filename_len] = '\0';

    if (filename_len == 0)
    {
        CustomWrite(2, "Empty filename\n", 15);
        return 1;
    }

    result = CustomPipe(pipefd);
    if (result != SUCCESS)
    {
        log_error(result, "pipe creation");
        return 1;
    }

    result = CustomFork(&pid);
    if (result != SUCCESS)
    {
        log_error(result, "fork");
        CustomClose(pipefd[0]);
        CustomClose(pipefd[1]);
        return 1;
    }

    if (pid == 0)
    {
        CustomClose(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        CustomClose(pipefd[0]);

        execl("./Child", "Child", filename, NULL);
        CustomWrite(2, "Exec failed\n", 12);
        _exit(1);
    }
    else
    {
        CustomClose(pipefd[0]);

        while (1)
        {
            CustomWrite(1, "Enter number: ", 14);

            int status;
            if (waitpid(pid, &status, WNOHANG) == pid)
            {
                CustomWrite(1, "Child finished after receiving number\n", 38);
                break;
            }

            bytes_read = read(0, buffer, sizeof(buffer) - 1);
            if (bytes_read <= 0)
                break;
            
            int len = 0;
            while (len < bytes_read && buffer[len] != '\n' && buffer[len] != '\0')
            {
                len++;
            }
            buffer[len] = '\0';

            if (len == 0)
                continue;

            number = 0;
            int i = 0;
            int is_negative = 0;

            if (buffer[0] == '-')
            {
                is_negative = 1;
                i = 1;
            }

            for (; i < len && buffer[i] >= '0' && buffer[i] <= '9'; i++)
            {
                number = number * 10 + (buffer[i] - '0');
            }

            if (is_negative)
            {
                number = -number;
            }
            
            result = CustomWrite(pipefd[1], &number, sizeof(number));
            if (result != SUCCESS)
            {
                log_error(result, "write to child");
                break;
            }

            if (waitpid(pid, &status, WNOHANG) == pid)
            {
                CustomWrite(1, "Child finished after receiving number\n", 38);
                break;
            }

            if (number < 0)
            {
                CustomWrite(1, "Negative number, exiting\n", 25);
                break;
            }
        }

        CustomClose(pipefd[1]);

        wait(NULL);
    }

    return 0;
}