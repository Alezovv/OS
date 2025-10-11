#include "../include/Custom.h"
#include "../include/Validation.h"
#include "../include/Utils.h"
#include <stdlib.h>

#define CHECK_STATUS(status, success, handler) \
    do                                         \
    {                                          \
        if (status != success)                 \
        {                                      \
            print_usage();                     \
            handler(status);                   \
            return status;                     \
        }                                      \
    } while (0)

void print_usage()
{
    printf("\nUsage: <program> <max count threads>(int)\n");
    printf("Example: main 4\n\n");
}

void validation_handle_error(int status)
{
    switch (status)
    {
    case ERROR_INVALID_COUNT_ARGUMENTS:
        printf("Неверное количество аргументов!\n");
        break;
    case ERROR_INVALID_COUNT_THREADS:
        printf("Неправильно введено число потоков!\n");
        break;
    case ERROR_COUNT_CORES:
        printf("Недостаточно ядер!\n");
        break;
    default:
        printf("Неизвестная ошибка!\n");
        break;
    }
}

void custom_handle_error(int status)
{
    switch (status)
    {
    case ERROR_THREAD_CREATE:
        printf("Не удалось создать поток!\n");
        break;
    case ERROR_THREAD_JOIN:
        printf("Не удалось дождаться поток!\n");
        break;
    default:
        printf("Неизвестная ошибка!\n");
        break;
    }
}

void *simple_thread(void *arg)
{
    printf("Поток успешно создан и работает!\n");
    return NULL;
}

int main(int argc, char *argv[])
{

    ValidationStatus status = ValidArgument(argc, argv);
    CHECK_STATUS(status, VALIDATION_SUCCESS, validation_handle_error);

    pthread_t thread1,
        thread2;
    char *msg1 = "Поток 1";
    char *msg2 = "Поток 2";

    status = custom_pthread_create(&thread1, NULL, simple_thread, NULL);
    CHECK_STATUS(status, CUSTOM_SUCCESS, custom_handle_error);

    status = custom_pthread_create(&thread2, NULL, simple_thread, NULL);
    CHECK_STATUS(status, CUSTOM_SUCCESS, custom_handle_error);

    status = custom_pthread_join(thread1, NULL);
    CHECK_STATUS(status, CUSTOM_SUCCESS, custom_handle_error);

    status = custom_pthread_join(thread2, NULL);
    CHECK_STATUS(status, CUSTOM_SUCCESS, custom_handle_error);

    printf("Все потоки завершили работу\n");
    return 0;
}