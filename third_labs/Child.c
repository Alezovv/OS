#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

#include <stdio.h>

#define SHM_SIZE 4096

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

int main(int argc, char *argv[])
{
    if (argc < 4)
        _exit(EXIT_FAILURE);

    char *shm_name = argv[1];
    char *sem_empty_name = argv[2];
    char *sem_full_name = argv[3];

    int shm_fd = shm_open(shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
        _exit(EXIT_FAILURE);

    int *shared_data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED)
        _exit(EXIT_FAILURE);

    sem_t *sem_empty = sem_open(sem_empty_name, 0);
    sem_t *sem_full = sem_open(sem_full_name, 0);
    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED)
        _exit(EXIT_FAILURE);

    int fd_output = open("output.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd_output == -1)
        _exit(EXIT_FAILURE);

    bool running = true;
    while (running)
    {
        sem_wait(sem_full);

        int number = shared_data[0];
        if (number == -2 || number < 0 || is_prime(number))
        {
            sem_post(sem_empty);
            break;
        }

        char buf[32];
        int n = snprintf(buf, sizeof(buf), "%d\n", number);
        write(fd_output, buf, n);

        sem_post(sem_empty);
    }

    close(fd_output);
    sem_close(sem_empty);
    sem_close(sem_full);
    munmap(shared_data, SHM_SIZE);
    close(shm_fd);

    return 0;
}
