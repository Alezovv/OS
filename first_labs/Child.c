#include "include/Custom.h"

int is_prime(int n)
{
    if (n < 2)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;

    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

void int_to_string(int num, char *str)
{
    int i = 0;
    do
    {
        str[i++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0);

    for (int j = 0; j < i / 2; j++)
    {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    str[i] = '\0';
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        CustomWrite(2, "Filename not provided\n", 22);
        _exit(1);
    }

    char *filename = argv[1];
    int number;
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, &number, sizeof(number))) > 0)
    {

        if (number < 0)
        {
            _exit(0);
        }

        if (is_prime(number))
        {
            _exit(0);
        }

        int fd;
        StatusCode result = CustomOpen(&fd, filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (result == SUCCESS)
        {
            char num_str[20];
            int_to_string(number, num_str);

            CustomWrite(fd, num_str, strlen(num_str));
            CustomWrite(fd, "\n", 1);
            CustomClose(fd);
        }
    }

    return 0;
}