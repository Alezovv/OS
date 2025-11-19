#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#include <stdio.h>

#define SHM_SIZE 4096

char SHM_NAME[1024];
char SEM_EMPTY[1024];
char SEM_FULL[1024];

bool is_prime(int n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

int main()
{
    char unique[64];
    snprintf(unique, sizeof(unique), "%d_%ld", getpid(), (long)time(NULL));
    snprintf(SHM_NAME, sizeof(SHM_NAME), "/shm_%s", unique);
    snprintf(SEM_EMPTY, sizeof(SEM_EMPTY), "/sem_empty_%s", unique);
    snprintf(SEM_FULL, sizeof(SEM_FULL), "/sem_full_%s", unique);

    int shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (shm_fd == -1)
    {
        const char msg[] = "error: failed to create SHM\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        _exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        const char msg[] = "error: failed to resize SHM\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        _exit(EXIT_FAILURE);
    }

    int *shared_data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED)
    {
        const char msg[] = "error: failed to map SHM\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        _exit(EXIT_FAILURE);
    }

    sem_t *sem_empty = sem_open(SEM_EMPTY, O_CREAT | O_EXCL, 0600, 1);
    sem_t *sem_full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0600, 0);
    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED)
    {
        const char msg[] = "error: failed to create semaphore\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        _exit(EXIT_FAILURE);
    }

    pid_t child = fork();
    if (child == 0)
    {
        char *args[] = {"Child", SHM_NAME, SEM_EMPTY, SEM_FULL, NULL};
        execv("./Child", args);
        const char msg[] = "error: failed to exec child\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        _exit(EXIT_FAILURE);
    }
    else if (child < 0)
    {
        const char msg[] = "error: failed to fork\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        _exit(EXIT_FAILURE);
    }

    bool running = true;
    char buffer[128];
    while (running)
    {
        const char prompt[] = "Enter number: ";
        write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

        ssize_t bytes = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (bytes <= 0)
            break;
        buffer[bytes - 1] = '\0';

        int number = atoi(buffer);

        sem_wait(sem_empty);
        shared_data[0] = number;
        sem_post(sem_full);

        if (number < 0 || is_prime(number))
            running = false;
    }

    sem_wait(sem_empty);
    shared_data[0] = -2;
    sem_post(sem_full);

    waitpid(child, NULL, 0);

    sem_close(sem_empty);
    sem_close(sem_full);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);

    munmap(shared_data, SHM_SIZE);
    shm_unlink(SHM_NAME);
    close(shm_fd);

    return 0;
}
