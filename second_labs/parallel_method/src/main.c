#include "../include/Custom.h"
#include "../include/Validation.h"
#include "../include/Centroids.h"

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

void print_usage()
{
    printf("\nUsage: <program> <max count threads>(int)\n");
    printf("Example: main 4\n\n");
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

    int count_points = 20;
    int count_clusters = 3;
    Point points[count_points];

    // Создание случайных точек
    Create(points, count_points);

    // Инициализация центроидов
    Centroid *centroids = Initial_Centroids(points, count_points, count_clusters);

    for (int i = 0; i < MAX_ITER; i++)
    {
        // Распределение точек по центроидам параллельно
        parallel_set_centroid(points, count_points, centroids, count_clusters);

        // Пересчет центроидов параллельно
        parallel_change_centroids(centroids, count_clusters);

        // Вывод текущих центроидов и точек
        printf("Итерация %d:\n", i + 1);
        print_centroids(centroids, count_clusters);
        print_points(centroids, count_clusters);
    }

    // Очистка памяти
    for (int i = 0; i < count_clusters; i++)
    {
        free(centroids[i].points);
    }
    free(centroids);

    return 0;
}